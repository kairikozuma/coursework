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

#include "mbed.h"
#include "ESP8266.h"
#include "Endpoint.h"
#include <string>
#include <algorithm>

//Debug is disabled by default
#if 0
#define DBG(x, ...)  printf("[ESP8266 : DBG]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define WARN(x, ...) printf("[ESP8266 : WARN]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define ERR(x, ...)  printf("[ESP8266 : ERR]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#else
#define DBG(x, ...) //wait_us(10);
#define WARN(x, ...) //wait_us(10);
#define ERR(x, ...)
#endif

#if 0
#define INFO(x, ...) printf("[ESP8266 : INFO]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#else
#define INFO(x, ...)
#endif

#define ESP_MAX_TRY_JOIN 3
#define ESP_MAXID 4 // the largest possible ID Value (max num of sockets possible)

ESP8266 * ESP8266::inst;
char* ip = NULL;

ESP8266::ESP8266(PinName tx, PinName rx, PinName reset, const char *ssid, const char *phrase, uint32_t baud) :
    wifi(tx, rx), reset_pin(reset), buf_ESP8266(ESP_MBUFFE_MAX)
{
    INFO("Initializing ESP8266 object");
    memset(&state, 0, sizeof(state));

    // change all ' ' in '$' in the ssid and the passphrase
    strcpy(this->ssid, ssid);
    for (int i = 0; i < strlen(ssid); i++) {
        if (this->ssid[i] == ' ')
            this->ssid[i] = '$';
    }
    
    strcpy(this->phrase, phrase);
    for (int i = 0; i < strlen(phrase); i++) {
        if (this->phrase[i] == ' ')
            this->phrase[i] = '$';
    }

    inst = this;
    attach_rx(false);

    wifi.baud(baud); // initial baud rate of the ESP8266

    state.associated = false;
    state.cmdMode = false;
}

bool ESP8266::join()
{
    sendCommand( "AT+CWMODE=1", "change", NULL, 1000);
    string cmd="AT+CWJAP=\""+(string)this->ssid+"\",\""+(string)this->phrase+"\"";
    if( sendCommand( cmd.c_str(), "OK", NULL, 10000) ) {
        // successfully joined the network
        state.associated = true;
        INFO("ssid: %s, phrase: %s", this->ssid, this->phrase);
        return true;
    }
    return false;
}

bool ESP8266::connect()
{
    sendCommand("AT+CWDHCP=1,1","OK",NULL,1000); // DHCP Enabled in Station Mode
    return ESP8266::join();
}

bool ESP8266::is_connected()
{
    return true;
}

bool ESP8266::start(bool type,char* ip, int port, int id)
{
    // Error Check
    if(id > ESP_MAXID) {
        ERR("startUDPMulti: max id is: %d, id given is %d",ESP_MAXID,id);
        return false;
    }
    // Single Connection Mode
    if(id < 0) {
        DBG("Start Single Connection Mode");
        char portstr[5];
        char idstr[2];
        bool check [3] = {0};
        sprintf(idstr,"%d",id);
        sprintf(portstr, "%d", port);
        switch(type) {
            case ESP_UDP_TYPE : //UDP
                check[0] = sendCommand(( "AT+CIPSTART=\"UDP\",\"" + (string) ip + "\"," + (string) portstr ).c_str(), "OK", NULL, 10000);
                break;
            case ESP_TCP_TYPE : //TCP
                check[0] = sendCommand(( "AT+CIPSTART=\"TCP\",\"" + (string) ip + "\"," + (string) portstr ).c_str(), "OK", NULL, 10000);
                break;
            default:
                ERR("Default hit for starting connection, this shouldnt be possible!!");
                break;
        }
        check[1] = sendCommand("AT+CIPMODE=1", "OK", NULL, 1000);// go into transparent mode
        check[2] = sendCommand("AT+CIPSEND", ">", NULL, 1000);// go into transparent mode
        // check that all commands were sucessful
        if(check[0] and check[1] and check[2]) {
            state.cmdMode = false;
            return true;
        } else {
            ERR("startUDPTransparent Failed for ip:%s, port:%d",ip,port);
            return false;
        }
    }
    // Multi Connection Mode
    else {
        //TODO: impliment Multi Connection Mode
        ERR("Not currently Supported!");
        return false;
        
//        DBG("Start Multi Connection Mode");
//        char portstr[5];
//        char idstr[2];
//        bool check [3] = {0};
//        sprintf(idstr,"%d",id);
//        sprintf(portstr, "%d", port);
//        switch(type) {
//            case ESP_UDP_TYPE : //UDP
//                check[0] = sendCommand(( "AT+CIPSTART=" + (string) idstr + ",\"UDP\",\"" + (string) ip + "\"," + (string) portstr ).c_str(), "OK", NULL, 10000);
//                break;
//            case ESP_TCP_TYPE : //TCP
//                check[0] = sendCommand(( "AT+CIPSTART=" + (string) idstr + ",\"TCP\",\"" + (string) ip + "\"," + (string) portstr ).c_str(), "OK", NULL, 10000);
//                break;
//            default:
//                ERR("Default hit for starting connection, this shouldnt be possible!!");
//                break;
//        }
    }
}

bool ESP8266::startUDP(char* ip, int port, int id, int length)
{
    char portstr[5];
    char idstr[1];
    char lenstr[2];
    
    sprintf(portstr, "%d", port);
    sprintf(idstr, "%d", id);
    sprintf(lenstr, "%d", length);
    
    sendCommand("AT+CIPMUX=1", "OK", NULL, 1000);
    sendCommand(( "AT+CIPSTART=" + string(idstr) + ",\"UDP\",\"" + (string) ip + "\"," + (string) portstr + ",1112,0").c_str(), "OK", NULL, 10000);
    sendCommand(("AT+CIPSEND=" + (string)idstr + "," +  (string)lenstr).c_str(), ">", NULL, 1000);// go into transparent mode
    DBG("Data Mode\r\n");
    state.cmdMode = false;

    return true;
}

bool ESP8266::startTCPServer(int port)
{
    bool command_results[3];
    command_results[0]=sendCommand("AT+CWMODE=3", "OK", NULL, 1000);
    command_results[1]=sendCommand("AT+CIPMUX=1", "OK", NULL, 1000);
    if(port == 333){
        command_results[2]=sendCommand("AT+CIPSERVER=1", "OK", NULL, 1000);
    }
    else{
        char portstr[5];
        sprintf(portstr, "%d", port);
        command_results[2]=sendCommand(("AT+CIPSERVER=1," + (string)portstr).c_str(), "OK", NULL, 1000);
    }
    //sendCommand("AT+CIFSR", "OK", NULL, 1000);
    DBG("Data Mode\r\n");
    state.cmdMode = false;
    if (command_results[0] and command_results[1] and command_results[2]){
        return true;
    }
    else{
        return false;
    }
}

bool ESP8266::close()
{
    wait(0.1f);
    send("+++",3);
    wait(1.0f);
    state.cmdMode = true;
    sendCommand("AT+CIPCLOSE","OK", NULL, 10000);
    return true;
}

bool ESP8266::disconnect()
{
    // if already disconnected, return
    if (!state.associated)
        return true;
    // send command to quit AP
    sendCommand("AT+CWQAP", "OK", NULL, 10000);
    state.associated = false;
    return true;
}

/*
    Assuming Returned data looks like this:
    +CIFSR:STAIP,"192.168.11.2"
    +CIFSR:STAMAC,"18:fe:34:9f:3a:f5"
    grabbing IP from first set of quotation marks
*/
char* ESP8266::getIPAddress()
{
    char result[30] = {0};
    int check = 0;
    check = sendCommand("AT+CIFSR", NULL, result, 1000);
    printf("\r\nReceivedInfo for IP Command is: %s\r\n",result);
    ip = ipString;
    if(check) {
        // Success
        string resultString(result);
        uint8_t pos1 = 0, pos2 = 0;
        uint8_t pos3 = 0, pos4 = 0;
        pos1 = resultString.find("+CIFSR:STAIP");
        pos1 = resultString.find('"',pos1);
        pos2 = resultString.find('"',pos1+1);
        pos3 = resultString.find('"',pos2+1); //would find mac address
        pos4 = resultString.find('"',pos3+1);
        strncpy(ipString,resultString.substr(pos1,pos2).c_str(),sizeof(ipString));
        ipString[pos2 - pos1 +1] = 0; // null terminate string correctly.
        INFO("IP: %s",ipString);
        ip = ipString;

    } else {
        // Failure
        ERR("getIPAddress() failed");
        ip = NULL;
    }
    return ip;
}

bool ESP8266::gethostbyname(const char * host, char * ip)
{
    string h = host;
    int nb_digits = 0;

    // no dns needed
    int pos = h.find(".");
    if (pos != string::npos) {
        string sub = h.substr(0, h.find("."));
        nb_digits = atoi(sub.c_str());
    }
    //printf("substrL %s\r\n", sub.c_str());
    if (count(h.begin(), h.end(), '.') == 3 && nb_digits > 0) {
        strcpy(ip, host);
        return true;
    } else {
        // dns needed, not currently available
        ERR("gethostbyname(): DNS Not currently available, only use IP Addresses!");
        return false;
    }
}

void ESP8266::reset()
{
    reset_pin = 0;
    wait_us(20);
    reset_pin = 1;
    //wait(1);
    //reset_pin = !reset_pin
    //send("+++",3);
    wait(1);
    state.cmdMode = true;
    sendCommand("AT", "OK", NULL, 1000);
    sendCommand("AT+RST", "ready", NULL, 10000);
    state.associated = false;

}

bool ESP8266::reboot()
{
    reset();
    return true;
}

void ESP8266::handler_rx(void)
{
    //read characters
    char c;
    while (wifi.readable()) {
        c=wifi.getc();
        buf_ESP8266.queue(c);
        //if (state.cmdMode) pc.printf("%c",c); //debug echo, needs fast serial console to prevent UART overruns
    }
}

void ESP8266::attach_rx(bool callback)
{
    if (!callback) {
        wifi.attach(NULL);
    }
    else {
        wifi.attach(this, &ESP8266::handler_rx);
    }
}

int ESP8266::readable()
{
    return buf_ESP8266.available();
}

int ESP8266::writeable()
{
    return wifi.writeable();
}

char ESP8266::getc()
{
    char c=0;
    while (!buf_ESP8266.available());
    buf_ESP8266.dequeue(&c);
    return c;
}

int ESP8266::putc(char c)
{
    while (!wifi.writeable() || wifi.readable()); //wait for echoed command characters to be read first
    return wifi.putc(c);
}

void ESP8266::flush()
{
    buf_ESP8266.flush();
}

int ESP8266::send(const char * buf, int len)
{
    //TODO: need to add handler for data > 2048B, this is the max packet size of the ESP8266.
    if(len >= 2048){
        WARN("send buffer >= 2048B, need to chunk this up to be less.");    
    }
    const char* bufptr=buf;
    for(int i=0; i<len; i++) {
        putc((int)*bufptr++);
    }
    return len;
}

bool ESP8266::sendCommand(const char * cmd, const char * ACK, char * res, int timeout)
{
    char read;
    size_t found = string::npos;
    string checking = "";
    Timer tmr;
    int result = 0;

    DBG("sendCmd:\t %s",cmd);

    attach_rx(true);

    //We flush the buffer
    while (readable())
        getc();

    if (!ACK || !strcmp(ACK, "NO")) {
        for (int i = 0; i < strlen(cmd); i++) {
            result = (putc(cmd[i]) == cmd[i]) ? result + 1 : result;
            wait(0.005f); // prevents stuck recieve ready (?) need to let echoed character get read first
        }
        putc(13); //CR
        wait(0.005f); // wait for echo
        putc(10); //LF

    } else {
        //We flush the buffer
        while (readable())
            getc();

        tmr.start();
        for (int i = 0; i < strlen(cmd); i++) {
            result = (putc(cmd[i]) == cmd[i]) ? result + 1 : result;
            wait(.005); // wait for echo
        }
        putc(13); //CR
        wait(0.005f); // wait for echo
        putc(10); //LF

        while (1) {
            if (tmr.read_ms() > timeout) {
                //We flush the buffer
                while (readable())
                    getc();

                DBG("check:\t %s", checking.c_str());

                attach_rx(true);
                return -1;
            } else if (readable()) {
                read = getc();
                //printf("%c",read); //debug echo
                if ( read != '\r' && read != '\n') {
                    checking += read;
                    found = checking.find(ACK);
                    if (found != string::npos) {
                        wait(0.01f);

                        //We flush the buffer
                        while (readable())
                            read = getc();
                        //printf("%c",read); //debug echo
                        break;
                    }
                }
            }
        }
        DBG("check: %s", checking.c_str());

        attach_rx(true);
        return result;
    }

    //the user wants the result from the command (ACK == NULL, res != NULL)
    if (res != NULL) {
        int i = 0;
        Timer timeout;
        timeout.start();
        tmr.reset();
        while (1) {
            if (timeout.read() > 2) {
                if (i == 0) {
                    res = NULL;
                    break;
                }
                res[i] = '\0';
                DBG("user str 1: %s", res);

                break;
            } else {
                if (tmr.read_ms() > 300) {
                    res[i] = '\0';
                    DBG("user str: %s", res);

                    break;
                }
                if (readable()) {
                    tmr.start();
                    read = getc();

                    // we drop \r and \n
                    if ( read != '\r' && read != '\n') {
                        res[i++] = read;
                    }
                }
            }
        }
        DBG("user str: %s", res);
    }

    //We flush the buffer
    while (readable())
        getc();

    attach_rx(true);
    DBG("result: %d", result)
    return result;
}
