/* HTTPClient.cpp */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
// RTOS for Thread definition
#include "rtos.h"

//Debug is disabled by default
#if 0
//Enable debug
#include <cstdio>
#define DBG(x, ...) std::printf("[HTTPClient : DBG] "x"\r\n", ##__VA_ARGS__);
#define WARN(x, ...) std::printf("[HTTPClient : WARN] "x"\r\n", ##__VA_ARGS__);
#define ERR(x, ...) std::printf("[HTTPClient : ERR] "x"\r\n", ##__VA_ARGS__);

#else
//Disable debug
#define DBG(x, ...)
#define WARN(x, ...)
#define ERR(x, ...)

#endif

#define HTTP_PORT 80
#define HTTPS_PORT 443

#define OK 0

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

#include <cstring>

#include  <../CyaSSL/cyassl/ctaocrypt/settings.h>
#include <../CyaSSL/cyassl/ctaocrypt/types.h>
#include <../CyaSSL/cyassl/internal.h>
#include <../CyaSSL/cyassl/ssl.h>

#include "HTTPClient.h"



// ************ should be a better way to adjust for platform limitations 

#if defined (TARGET_K64F)
    // can use larger buffers since we have (lots) more memory
    #define BIG_MEMORY_CHUNK        4096
    #define CHUNK_SIZE              768
    #define SEND_BUF_SIZE           BIG_MEMORY_CHUNK
    #define MAX_URL_HOSTNAME_LENGTH 256 
    #define MAX_URL_PATH_LENGTH     BIG_MEMORY_CHUNK
    #define MAX_HEADER_VALUE_LENGTH BIG_MEMORY_CHUNK
    #define MAX_HEADER_KEY_LENGTH   40
    #define HEADER_SCANF_FORMAT     "%40[^:]: %4096[^\r\n]" /* must align with BIG_MEMORY_CHUNK */ 
    #define REDIRECT_SCANF_FORMAT   "%40[^:]: %4096[^\r\n]" /* must align with BIG_MEMORY_CHUNK */
#else
    // default smaller buffers
    #define CHUNK_SIZE              256
    #define SEND_BUF_SIZE           1100
    #define MAX_URL_HOSTNAME_LENGTH 128
    #define MAX_URL_PATH_LENGTH     128
    #define MAX_HEADER_VALUE_LENGTH 41
    #define MAX_HEADER_KEY_LENGTH   40
    #define HEADER_SCANF_FORMAT     "%40[^:]: %40[^\r\n]"   /* must align with MAX_HEADER_VALUE_LENGTH */ 
    #define REDIRECT_SCANF_FORMAT   "%40[^:]: %128[^\r\n]"  /* must align with MAX_URL_PATH_LENGTH */
#endif

// ************ should be a better way to adjust for platform limitations

static char send_buf[SEND_BUF_SIZE] ;
static char *send_buf_p = NULL;
static TCPSocketConnection* m_sock_p = NULL;

static void base64enc(char *out, const char *in) {
    const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ;
    int i = 0, x = 0, l = 0;

    for (; *in; in++) {
        x = x << 8 | *in;
        for (l += 8; l >= 6; l -= 6) {
            out[i++] = code[(x >> (l - 6)) & 0x3f];
        }
    }
    if (l > 0) {
        x <<= 6 - l;
        out[i++] = code[x & 0x3f];
    }
    for (; i % 4;) {
        out[i++] = '=';
    }
    out[i] = '\0' ;
}
int SocketReceive(CYASSL* ssl, char *buf, int sz, void *ctx)
{
    int n ;
    int i ;
#define RECV_RETRY 3

    for(i=0; i<RECV_RETRY; i++) {
        n =  m_sock_p->receive(buf, sz) ;
        if(n >= 0)return n  ;
        Thread::wait(200) ;
    }
    ERR("SocketReceive:%d/%d\n", n, sz)  ;
    return n ;
}
int SocketSend(CYASSL* ssl, char *buf, int sz, void *ctx)
{
    int n ;
    
    Thread::wait(100) ;
    n =  m_sock_p->send(buf, sz);
    if(n > 0) {
        Thread::wait(300) ;
        return n ;
    } else  ERR("SocketSend:%d/%d\n", n, sz);
    return n ;
}

HTTPClient::HTTPClient() :
    m_basicAuthUser(NULL), m_basicAuthPassword(NULL), m_httpResponseCode(0), m_oauthToken(NULL)
{
    // To DEBUG the underlying SSL - uncomment this...
    //CyaSSL_Debugging_ON();
    ctx = 0 ;
    ssl = 0 ;
    SSLver = 3 ; 
    m_basicAuthUser = NULL ;
    m_basicAuthPassword = NULL;
    m_httpResponseCode = 0;
    m_oauthToken = NULL;
    redirect_url = NULL ;
    redirect = 0 ;
    header = NULL ;
    m_sock_p = &m_sock;
}

HTTPClient::~HTTPClient()
{

}


HTTPResult HTTPClient::oauthToken(const char *token) { // OAUTH2 Authentication
    // reset if called
    if (m_oauthToken != NULL) free((void *)m_oauthToken);
    m_oauthToken = NULL;
    
    // fill in if able...
    if (token != NULL && strlen(token) > 0) {    
        m_oauthToken = (char *)malloc(strlen(token)+1);
        memset((void *)m_oauthToken,0,strlen(token)+1);
        strcpy((char *)m_oauthToken,token);
        this->basicAuth(NULL,NULL);
    }
    
    return HTTP_OK ;
}

HTTPResult HTTPClient::basicAuth(const char* user, const char* password) //Basic Authentification
{
    #define AUTHB_SIZE 128
    if((strlen(user) + strlen(password)) >= AUTHB_SIZE)
        return HTTP_ERROR ;
        
    if (m_basicAuthUser) free((void *)m_basicAuthUser);
    if (user != NULL) {
        m_basicAuthUser = (char *)malloc(strlen(user)+1);
        strcpy((char *)m_basicAuthUser, user);
        this->oauthToken(NULL);
    }
    
    if (m_basicAuthPassword) free((void *)m_basicAuthPassword);
    if (password != NULL) {
        m_basicAuthPassword = (char *)malloc(strlen(password)+1);
        strcpy((char *)m_basicAuthPassword, password); 
        this->oauthToken(NULL);
    }   
    
    return HTTP_OK ;
}

HTTPResult HTTPClient::get(const char* url, IHTTPDataIn* pDataIn, int timeout /*= HTTP_CLIENT_DEFAULT_TIMEOUT*/) //Blocking
{
    return connect(url, HTTP_GET, NULL, pDataIn, timeout);
}

HTTPResult HTTPClient::get(const char* url, char* result, size_t maxResultLen, int timeout /*= HTTP_CLIENT_DEFAULT_TIMEOUT*/) //Blocking
{
    HTTPText str(result, maxResultLen);
    return get(url, &str, timeout);
}

HTTPResult HTTPClient::post(const char* url, const IHTTPDataOut& dataOut, IHTTPDataIn* pDataIn, int timeout /*= HTTP_CLIENT_DEFAULT_TIMEOUT*/) //Blocking
{
    return connect(url, HTTP_POST, (IHTTPDataOut*)&dataOut, pDataIn, timeout);
}

HTTPResult HTTPClient::put(const char* url, const IHTTPDataOut& dataOut, IHTTPDataIn* pDataIn, int timeout /*= HTTP_CLIENT_DEFAULT_TIMEOUT*/) //Blocking
{
    return connect(url, HTTP_PUT, (IHTTPDataOut*)&dataOut, pDataIn, timeout);
}

HTTPResult HTTPClient::del(const char* url, IHTTPDataIn* pDataIn, int timeout /*= HTTP_CLIENT_DEFAULT_TIMEOUT*/) //Blocking
{
    return connect(url, HTTP_DELETE, NULL, pDataIn, timeout);
}


int HTTPClient::getHTTPResponseCode()
{
    return m_httpResponseCode;
}

void HTTPClient::setHeader(const char * h)
{
    header = h ;
}

void HTTPClient::setLocationBuf(char * url, int size)
{
    redirect_url = url ;
    redirect_url_size = size ;
}

HTTPResult HTTPClient::setSSLversion(int minorV) 
{
    if((minorV>=0) && (minorV<=3)) 
        SSLver = minorV ;
    else return HTTP_ERROR ;
    return HTTP_OK ;
}


#define CHECK_CONN_ERR(ret) \
  do{ \
    if(ret) { \
      cyassl_free() ;\
      m_sock.close(); \
      ERR("Connection error (%d)", ret); \
      return HTTP_CONN; \
    } \
  } while(0)

#define PRTCL_ERR() \
  do{ \
    cyassl_free() ;\
    m_sock.close(); \
    ERR("Protocol error"); \
    return HTTP_PRTCL; \
  } while(0)

void HTTPClient::cyassl_free(void)
{
    if(ssl) {
        CyaSSL_free(ssl) ;
        ssl = NULL ;
    }
    if(ctx) {
        CyaSSL_CTX_free(ctx) ;
        ctx = NULL ;
    }
    CyaSSL_Cleanup() ;
} 

HTTPResult HTTPClient::connect(const char* url, HTTP_METH method, IHTTPDataOut* pDataOut, IHTTPDataIn* pDataIn, int timeout) //Execute request
{
    CYASSL_METHOD * SSLmethod = NULL;
    m_httpResponseCode = 0; //Invalidate code
    m_timeout = timeout;
    redirect = 0 ;
    
    // reset the send buffer
    memset(send_buf,0,SEND_BUF_SIZE);
        
    pDataIn->writeReset();
    if( pDataOut ) {
        pDataOut->readReset();
    }

    char scheme[8];
    char host[MAX_URL_HOSTNAME_LENGTH];
    char path[MAX_URL_PATH_LENGTH];

    int ret ;

    //First we need to parse the url (http[s]://host[:port][/[path]])
    HTTPResult res = parseURL(url, scheme, sizeof(scheme), host, sizeof(host), &port, path, sizeof(path));
    if(res != HTTP_OK) {
        ERR("parseURL returned %d", res);
        return res;
    }

    if(port == 0) {
        if(strcmp(scheme, "http") == 0)
            port = HTTP_PORT ;
        else if(strcmp(scheme, "https") == 0)
            port = HTTPS_PORT ;
    }

    DBG("Scheme: %s", scheme);
    DBG("Host: %s", host);
    DBG("Port: %d", port);
    DBG("Path: %s", path);

    //Connect
    DBG("Connecting socket to server");

#define MAX_RETRY 5
    int retry ;

    for(retry=0; retry<MAX_RETRY; retry++) {
        int ret = m_sock.connect(host, port);
        if(ret == 0)break ;
    }
    if(retry == MAX_RETRY) {
        m_sock.close();
        ERR("Could not connect");
        return HTTP_CONN;
    }

    if(port == HTTPS_PORT) {

        /* Start SSL connect */
        DBG("SSLver=%d", SSLver) ;
        if(ctx == NULL) {
            switch(SSLver) {
            case 0 : SSLmethod = CyaSSLv3_client_method() ; break ;
            case 1 : SSLmethod = CyaTLSv1_client_method() ; break ;
            case 2 : SSLmethod = CyaTLSv1_1_client_method() ; break ;           
            case 3 : SSLmethod = CyaTLSv1_2_client_method() ; break ;      
            }
            ctx = CyaSSL_CTX_new((CYASSL_METHOD *)SSLmethod);
            if (ctx == NULL) {
                ERR("unable to get ctx");
                return HTTP_CONN;
            }
            CyaSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
            CyaSSL_SetIORecv(ctx, SocketReceive) ;
            CyaSSL_SetIOSend(ctx, SocketSend) ;
        }
        if (ssl == NULL) {
            ssl = CyaSSL_new(ctx);
            if (ssl == NULL) {
                ERR("unable to get SSL object");
                cyassl_free() ;
                return HTTP_CONN;
            }
        }
        
        DBG("ctx=%x, ssl=%x, ssl->ctx->CBIORecv, CBIOSend=%x, %x\n",ctx, ssl, SocketReceive, SocketSend ) ;
        int result = CyaSSL_connect(ssl); 
        if (result != SSL_SUCCESS) {
            ERR("SSL_connect failed");
            int err = CyaSSL_get_error(ssl, result);
            char errorString[80];
            CyaSSL_ERR_error_string(err, errorString);
            ERR("SSL Error: %s (err=%d,status=%d)",errorString,err,result);
            cyassl_free() ;
            return HTTP_CONN;
        }
    } /* SSL connect complete */

    //Send request
    DBG("Sending request");
    char buf[CHUNK_SIZE];
    memset(buf,0,CHUNK_SIZE);
    send_buf_p = send_buf ; // Reset send buffer ;

    const char* meth = (method==HTTP_GET)?"GET":(method==HTTP_POST)?"POST":(method==HTTP_PUT)?"PUT":(method==HTTP_DELETE)?"DELETE":"";
    snprintf(buf, sizeof(buf), "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n", meth, path, host); //Write request
    ret = send(buf);
    if(ret) {
        m_sock.close();
        ERR("Could not write request");
        return HTTP_CONN;
    }

    //Send all headers

    //Send default headers
    DBG("Sending headers");
    if(m_basicAuthUser && m_basicAuthPassword) {
        DBG("Sending basic auth header");
        bAuth() ;    /* send out Basic Auth header */        
    }
    else if (m_oauthToken) {
        DBG("Sending OAUTH header");
        tokenAuth(); /* send out OAUTH header */
    }
    if( pDataOut != NULL ) {
        if( pDataOut->getIsChunked() ) {
            ret = send("Transfer-Encoding: chunked\r\n");
            CHECK_CONN_ERR(ret);
        } else {
            snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", pDataOut->getDataLen());
            DBG("Content buf:%s", buf) ;
            ret = send(buf);
            CHECK_CONN_ERR(ret);
        }
        char type[48];
        if( pDataOut->getDataType(type, 48) == HTTP_OK ) {
            snprintf(buf, sizeof(buf), "Content-Type: %s\r\n", type);
            ret = send(buf);
            CHECK_CONN_ERR(ret);
        }
    }

    //Add user headers
    if(header) {
        ret = send((char *)header);
        CHECK_CONN_ERR(ret);
    }

    //Close headers
    DBG("Headers sent");
    ret = send("\r\n");
    CHECK_CONN_ERR(ret);

    size_t trfLen;

    //Send data (if available)
    if( pDataOut != NULL ) {
        DBG("Sending data");
        while(true) {
            size_t writtenLen = 0;
            pDataOut->read(buf, CHUNK_SIZE, &trfLen);
            buf[trfLen] = 0x0 ;
            DBG("buf:%s", buf) ;
            if( pDataOut->getIsChunked() ) {
                //Write chunk header
                char chunkHeader[64];
                snprintf(chunkHeader, sizeof(chunkHeader), "%X\r\n", trfLen); //In hex encoding
                ret = send(chunkHeader);
                CHECK_CONN_ERR(ret);
            } else if( trfLen == 0 ) {
                DBG("trfLen==0") ;
                break;
            }
            DBG("trfLen 1=%d", trfLen) ;
            if( trfLen != 0 ) {
                DBG("Sending 1") ;
                ret = send(buf, trfLen);
                DBG("Sent 1") ;
                CHECK_CONN_ERR(ret);
            }

            if( pDataOut->getIsChunked()  ) {
                ret = send("\r\n"); //Chunk-terminating CRLF
                CHECK_CONN_ERR(ret);
            } else {
                writtenLen += trfLen;
                if( writtenLen >= pDataOut->getDataLen() ) {
                    DBG("writtenLen=%d", writtenLen) ;
                    break;
                }
                DBG("writtenLen+=trfLen = %d", writtenLen) ;
            }
            DBG("trfLen 2=%d", trfLen) ;
            if( trfLen == 0 ) {
                DBG("trfLen == 0") ;
                break;
            }
        }

    }
    ret = flush() ; // flush the send buffer ;
    CHECK_CONN_ERR(ret);

    //Receive response
    DBG("Receiving response");

    ret = recv(buf, CHUNK_SIZE - 1, CHUNK_SIZE - 1, &trfLen); //Read n bytes
    CHECK_CONN_ERR(ret);

    buf[trfLen] = '\0';

    char* crlfPtr = strstr(buf, "\r\n");
    if(crlfPtr == NULL) {
        PRTCL_ERR();
    }

    int crlfPos = crlfPtr - buf;
    buf[crlfPos] = '\0';

    //Parse HTTP response
    if( sscanf(buf, "HTTP/%*d.%*d %d %*[^\r\n]", &m_httpResponseCode) != 1 ) {
        //Cannot match string, error
        ERR("Not a correct HTTP answer : %s\n", buf);
        PRTCL_ERR();
    }

    if( (m_httpResponseCode < 200) || (m_httpResponseCode >= 400) ) {
        //Did not return a 2xx code; TODO fetch headers/(&data?) anyway and implement a mean of writing/reading headers
        WARN("Response code %d", m_httpResponseCode);
        PRTCL_ERR();
    }

    DBG("Reading headers");

    memmove(buf, &buf[crlfPos+2], trfLen - (crlfPos + 2) + 1); //Be sure to move NULL-terminating char as well
    trfLen -= (crlfPos + 2);

    size_t recvContentLength = 0;
    bool recvChunked = false;
    //Now get headers
    while( true ) {
        crlfPtr = strstr(buf, "\r\n");
        if(crlfPtr == NULL) {
            if( trfLen < CHUNK_SIZE - 1 ) {
                size_t newTrfLen = 0;
                ret = recv(buf + trfLen, 1, CHUNK_SIZE - trfLen - 1, &newTrfLen);
                trfLen += newTrfLen;
                buf[trfLen] = '\0';
                DBG("Read %d chars; In buf: [%s]", newTrfLen, buf);
                CHECK_CONN_ERR(ret);
                continue;
            } else {
                PRTCL_ERR();
            }
        }

        crlfPos = crlfPtr - buf;

        if(crlfPos == 0) { //End of headers
            DBG("Headers read");
            memmove(buf, &buf[2], trfLen - 2 + 1); //Be sure to move NULL-terminating char as well
            trfLen -= 2;
            break;
        }

        buf[crlfPos] = '\0';

        char key[MAX_HEADER_KEY_LENGTH+1];
        memset(key,0,MAX_HEADER_KEY_LENGTH+1);
        
        char value[MAX_HEADER_VALUE_LENGTH+1];
        memset(value,0,MAX_HEADER_VALUE_LENGTH+1);
        
        int n = sscanf(buf, HEADER_SCANF_FORMAT, key, value);
        if ( n == 2 ) {
            DBG("Read header : %s: %s\n", key, value);
            if( !strcmp(key, "Content-Length") ) {
                sscanf(value, "%d", &recvContentLength);
                pDataIn->setDataLen(recvContentLength);
            } else if( !strcmp(key, "Transfer-Encoding") ) {
                if( !strcmp(value, "Chunked") || !strcmp(value, "chunked") ) {
                    recvChunked = true;
                    pDataIn->setIsChunked(true);
                }
            } else if( !strcmp(key, "Content-Type") ) {
                pDataIn->setDataType(value);
            } else if( !strcmp(key, "Location") && redirect_url) {      // Location is capitalized...
                sscanf(buf, REDIRECT_SCANF_FORMAT, key, redirect_url);
                DBG("Redirect %s: %s", key, redirect_url) ;
                redirect = 1 ;
            }
            memmove(buf, &buf[crlfPos+2], trfLen - (crlfPos + 2) + 1); //Be sure to move NULL-terminating char as well
            trfLen -= (crlfPos + 2);

        } else {
            ERR("Could not parse header");
            PRTCL_ERR();
        }

    }

    //Receive data
    DBG("Receiving data");

    while(true) {
        size_t readLen = 0;

        if( recvChunked ) {
            //Read chunk header
            bool foundCrlf;
            do {
                foundCrlf = false;
                crlfPos=0;
                buf[trfLen]=0;
                if(trfLen >= 2) {
                    for(; crlfPos < trfLen - 2; crlfPos++) {
                        if( buf[crlfPos] == '\r' && buf[crlfPos + 1] == '\n' ) {
                            foundCrlf = true;
                            break;
                        }
                    }
                }
                if(!foundCrlf) { //Try to read more
                    if( trfLen < CHUNK_SIZE ) {
                        size_t newTrfLen = 0;
                        ret = recv(buf + trfLen, 0, CHUNK_SIZE - trfLen - 1, &newTrfLen);
                        trfLen += newTrfLen;
                        CHECK_CONN_ERR(ret);
                        continue;
                    } else {
                        PRTCL_ERR();
                    }
                }
            } while(!foundCrlf);
            buf[crlfPos] = '\0';
            int n = sscanf(buf, "%x", &readLen);
            if(n!=1) {
                ERR("Could not read chunk length");
                PRTCL_ERR();
            }

            memmove(buf, &buf[crlfPos+2], trfLen - (crlfPos + 2)); //Not need to move NULL-terminating char any more
            trfLen -= (crlfPos + 2);

            if( readLen == 0 ) {
                //Last chunk
                break;
            }
        } else {
            readLen = recvContentLength;
        }

        DBG("Retrieving %d bytes", readLen);

        do {
            pDataIn->write(buf, MIN(trfLen, readLen));
            if( trfLen > readLen ) {
                memmove(buf, &buf[readLen], trfLen - readLen);
                trfLen -= readLen;
                readLen = 0;
            } else {
                readLen -= trfLen;
            }

            if(readLen) {
                ret = recv(buf, 1, CHUNK_SIZE - trfLen - 1, &trfLen);
                CHECK_CONN_ERR(ret);
            }
        } while(readLen);

        if( recvChunked ) {
            if(trfLen < 2) {
                size_t newTrfLen = 0;
                //Read missing chars to find end of chunk
                ret = recv(buf + trfLen, 2 - trfLen, CHUNK_SIZE - trfLen - 1, &newTrfLen);
                CHECK_CONN_ERR(ret);
                trfLen += newTrfLen;
            }
            if( (buf[0] != '\r') || (buf[1] != '\n') ) {
                ERR("Format error");
                PRTCL_ERR();
            }
            memmove(buf, &buf[2], trfLen - 2);
            trfLen -= 2;
        } else {
            break;
        }

    }
    cyassl_free() ;
    m_sock.close();
    DBG("Completed HTTP transaction");
    if(redirect)return HTTP_REDIRECT ;
    else        return HTTP_OK;
}

HTTPResult HTTPClient::recv(char* buf, size_t minLen, size_t maxLen, size_t* pReadLen)   //0 on success, err code on failure
{
    DBG("Trying to read between %d and %d bytes", minLen, maxLen);
    size_t readLen = 0;

    if(!m_sock.is_connected()) {
        WARN("Connection was closed by server");
        return HTTP_CLOSED; //Connection was closed by server
    }

    int ret;

    if(port == HTTPS_PORT) {
        DBG("Enter CyaSSL_read") ;

        m_sock.set_blocking(false, m_timeout);
        readLen = CyaSSL_read(ssl, buf, maxLen);
        if (readLen > 0) {
            buf[readLen] = 0;
            DBG("CyaSSL_read:%s\n", buf);
        } else {
            ERR("CyaSSL_read, ret = %d", readLen) ;
            return HTTP_ERROR ;
        }
        DBG("Read %d bytes", readLen);
        *pReadLen = readLen;
        return HTTP_OK;
    }

    while(readLen < maxLen) {
        if(readLen < minLen) {
            DBG("Trying to read at most %d bytes [Blocking]", minLen - readLen);
            m_sock.set_blocking(false, m_timeout);
            ret = m_sock.receive_all(buf + readLen, minLen - readLen);
        } else {
            DBG("Trying to read at most %d bytes [Not blocking]", maxLen - readLen);
            m_sock.set_blocking(false, 0);
            ret = m_sock.receive(buf + readLen, maxLen - readLen);
        }

        if( ret > 0) {
            readLen += ret;
        } else if( ret == 0 ) {
            break;
        } else {
            if(!m_sock.is_connected()) {
                ERR("Connection error (recv returned %d)", ret);
                *pReadLen = readLen;
                return HTTP_CONN;
            } else {
                break;
            }
        }

        if(!m_sock.is_connected()) {
            break;
        }
    }
    DBG("Read %d bytes", readLen);
    *pReadLen = readLen;
    return HTTP_OK;
}

HTTPResult HTTPClient::send(char* buf, size_t len)   //0 on success, err code on failure
{
    HTTPResult ret ;
    int cp_len ;

    if(len == 0) {
        len = strlen(buf);
    }

    do {

        if((SEND_BUF_SIZE - (send_buf_p - send_buf)) >= len) {
            cp_len = len ;
        } else {
            cp_len = SEND_BUF_SIZE - (send_buf_p - send_buf) ;
        }
        DBG("send_buf_p:%x. send_buf+SIZE:%x, len=%d, cp_len=%d", send_buf_p, send_buf+SEND_BUF_SIZE, len, cp_len) ;
        memcpy(send_buf_p, buf, cp_len) ;
        send_buf_p += cp_len ;
        len -= cp_len ;

        if(send_buf_p == send_buf + SEND_BUF_SIZE) {
            if(port == HTTPS_PORT){
                ERR("HTTPClient::send buffer overflow");
                return HTTP_ERROR ;
            }
            ret = flush() ;
            if(ret)return(ret) ;
        }
    } while(len) ;
    return HTTP_OK ;
}

HTTPResult HTTPClient::flush()   //0 on success, err code on failure
{
    int len ;
    char * buf ;

    buf = send_buf ;
    len = send_buf_p - send_buf ;
    send_buf_p = send_buf ; // reset send buffer

    DBG("Trying to write %d bytes:%s\n", len, buf);
    size_t writtenLen = 0;

    if(!m_sock.is_connected()) {
        WARN("Connection was closed by server");
        return HTTP_CLOSED; //Connection was closed by server
    }

    if(port == HTTPS_PORT) {
        DBG("Enter CyaSSL_write") ;
        if (CyaSSL_write(ssl, buf, len) != len) {
            ERR("SSL_write failed");
            return HTTP_ERROR ;
        }
        DBG("Written %d bytes", writtenLen);
        return HTTP_OK;
    }
    m_sock.set_blocking(false, m_timeout);
    int ret = m_sock.send_all(buf, len);
    if(ret > 0) {
        writtenLen += ret;
    } else if( ret == 0 ) {
        WARN("Connection was closed by server");
        return HTTP_CLOSED; //Connection was closed by server
    } else {
        ERR("Connection error (send returned %d)", ret);
        return HTTP_CONN;
    }

    DBG("Written %d bytes", writtenLen);
    return HTTP_OK;
}

HTTPResult HTTPClient::parseURL(const char* url, char* scheme, size_t maxSchemeLen, char* host, size_t maxHostLen, uint16_t* port, char* path, size_t maxPathLen)   //Parse URL
{
    DBG("Parsing URL: %s",url);
    char* schemePtr = (char*) url;
    char* hostPtr = (char*) strstr(url, "://");
    if(hostPtr == NULL) {
        WARN("Could not find host");
        return HTTP_PARSE; //URL is invalid
    }

    if( maxSchemeLen < hostPtr - schemePtr + 1 ) { //including NULL-terminating char
        WARN("Scheme str is too small (%d >= %d)", maxSchemeLen, hostPtr - schemePtr + 1);
        return HTTP_PARSE;
    }
    memcpy(scheme, schemePtr, hostPtr - schemePtr);
    scheme[hostPtr - schemePtr] = '\0';

    hostPtr+=3;

    size_t hostLen = 0;

    char* portPtr = strchr(hostPtr, ':');
    if( portPtr != NULL ) {
        hostLen = portPtr - hostPtr;
        portPtr++;
        if( sscanf(portPtr, "%hu", port) != 1) {
            WARN("Could not find port");
            return HTTP_PARSE;
        }
    } else {
        *port=0;
    }
    char* pathPtr = strchr(hostPtr, '/');
    if( hostLen == 0 ) {
        hostLen = pathPtr - hostPtr;
    }

    if( maxHostLen < hostLen + 1 ) { //including NULL-terminating char
        WARN("Host str is too small (%d >= %d)", maxHostLen, hostLen + 1);
        return HTTP_PARSE;
    }
    memcpy(host, hostPtr, hostLen);
    host[hostLen] = '\0';

    size_t pathLen;
    char* fragmentPtr = strchr(hostPtr, '#');
    if(fragmentPtr != NULL) {
        pathLen = fragmentPtr - pathPtr;
    } else {
        pathLen = strlen(pathPtr);
    }

    if( maxPathLen < pathLen + 1 ) { //including NULL-terminating char
        WARN("Path str is too small (%d >= %d)", maxPathLen, pathLen + 1);
        return HTTP_PARSE;
    }
    memcpy(path, pathPtr, pathLen);
    path[pathLen] = '\0';

    return HTTP_OK;
}

HTTPResult HTTPClient::tokenAuth(void)
{
    HTTPResult ret ; 
    ret = send("Authorization: Bearer ") ;
    CHECK_CONN_ERR(ret);
    strcat((char *)m_oauthToken,"\n");
    DBG("oauthToken: %s", m_oauthToken) ;
    ret = send((char *)m_oauthToken);
    CHECK_CONN_ERR(ret); 
    return HTTP_OK ;
}

HTTPResult HTTPClient::bAuth(void)
{
    HTTPResult ret ;
    char b_auth[(int)((AUTHB_SIZE+3)*4/3+1)] ;
    char base64buff[AUTHB_SIZE+3] ;
 
    ret = send("Authorization: Basic ") ;
    CHECK_CONN_ERR(ret);
    sprintf(base64buff, "%s:%s", m_basicAuthUser, m_basicAuthPassword) ;
    DBG("bAuth: %s", base64buff) ;
    base64enc(b_auth, base64buff) ;
    b_auth[strlen(b_auth)+1] = '\0' ;
    b_auth[strlen(b_auth)] = '\n' ;
    DBG("b_auth:%s", b_auth) ;
    ret = send(b_auth) ;
    CHECK_CONN_ERR(ret); 
    return HTTP_OK ;
}
