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
#include "TCPSocketConnection.h"
#include <cstring>
#include <algorithm>

using std::memset;
using std::memcpy;

//Debug is disabled by default
#if 1
#define DBG(x, ...)  printf("[TCPConnection : DBG]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define WARN(x, ...) printf("[TCPConnection: WARN]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define ERR(x, ...)  printf("[TCPConnection : ERR]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#else
#define DBG(x, ...)
#define WARN(x, ...)
#define ERR(x, ...)
#endif

TCPSocketConnection::TCPSocketConnection() :
    _is_connected(false)
{
}

int TCPSocketConnection::connect(const char* host, const int port)
{
//    if (init_socket(SOCK_STREAM) < 0)
//        return -1;
//
    if (set_address(host, port) != 0)
        return -1;
//
//    if (lwip_connect(_sock_fd, (const struct sockaddr *) &_remoteHost, sizeof(_remoteHost)) < 0) {
//        close();
//        return -1;
//    }
//    _is_connected = true;
    _is_connected = ESP8266->start(ESP_TCP_TYPE,_ipAddress,_port);
    if(_is_connected) { //success
        return 0;
    } else { // fail
        return -1;
    }
}

bool TCPSocketConnection::is_connected(void)
{
    return _is_connected;
}

int TCPSocketConnection::send(char* data, int length)
{
    if (!_is_connected) {
        ERR("TCPSocketConnection::receive() - _is_connected is false : you cant receive data until you connect to a socket!");
        return -1;
    }
    Timer tmr;
    int idx = 0;
    tmr.start();
    while ((tmr.read_ms() < _timeout) || _blocking) {

        idx += wifi->send(data, length);

        if (idx == length)
            return idx;
    }
    return (idx == 0) ? -1 : idx;

    //return wifi->send(data,length);
//
//    if (!_blocking) {
//        TimeInterval timeout(_timeout);
//        if (wait_writable(timeout) != 0)
//            return -1;
//    }
//
//    int n = lwip_send(_sock_fd, data, length, 0);
//    _is_connected = (n != 0);
//
//    return n;

}

// -1 if unsuccessful, else number of bytes written
int TCPSocketConnection::send_all(char* data, int length)
{
//   if ((_sock_fd < 0) || !_is_connected)
//        return -1;
//
//    int writtenLen = 0;
//    TimeInterval timeout(_timeout);
//    while (writtenLen < length) {
//        if (!_blocking) {
//            // Wait for socket to be writeable
//            if (wait_writable(timeout) != 0)
//                return writtenLen;
//        }
//
//        int ret = lwip_send(_sock_fd, data + writtenLen, length - writtenLen, 0);
//        if (ret > 0) {
//            writtenLen += ret;
//            continue;
//        } else if (ret == 0) {
//            _is_connected = false;
//            return writtenLen;
//        } else {
//            return -1; //Connnection error
//        }
//    }
//    return writtenLen;
    return send(data,length); // just remap to send
}

int TCPSocketConnection::receive(char* buffer, int length)
{
    if (!_is_connected) {
        ERR("TCPSocketConnection::receive() - _is_connected is false : you cant receive data until you connect to a socket!");
        return -1;
    }
    Timer tmr;
    int idx = 0;
    int nb_available = 0;
    int time = -1;

    //make this the non-blocking case and return if <= 0
    // remember to change the config to blocking
    // if ( ! _blocking) {
    // if ( wifi.readable <= 0 ) {
    // return (wifi.readable);
    // }
    // }
    //---
    tmr.start();
    if (_blocking) {
        while (1) {
            nb_available = wifi->readable();
            if (nb_available != 0) {
                break;
            }
        }
    }
    //---
    // blocking case
    else {
        tmr.reset();

        while (time < _timeout) {
            nb_available = wifi->readable();
            if (nb_available < 0) return nb_available;
            if (nb_available > 0) break ;
            time = tmr.read_ms();
        }

        if (nb_available == 0) return nb_available;
    }

    // change this to < 20 mS timeout per byte to detect end of packet gap
    // this may not work due to buffering at the UART interface
    tmr.reset();
    // while ( tmr.read_ms() < 20 ) {
    // if ( wifi.readable() && (idx < length) ) {
    // buffer[idx++] = wifi->getc();
    // tmr.reset();
    // }
    // if ( idx == length ) {
    // break;
    // }
    // }
    //---
    while (time < _timeout) {

        nb_available = wifi->readable();
        //for (int i = 0; i < min(nb_available, length); i++) {
        for (int i = 0; i < min(nb_available, (length-idx)); i++) {
            buffer[idx] = wifi->getc();
            idx++;
        }
        if (idx == length) {
            break;
        }
        time = tmr.read_ms();
    }
    //---
    return (idx == 0) ? -1 : idx;

//************************ original code below
//
//    if (!_blocking) {
//        TimeInterval timeout(_timeout);
//        if (wait_readable(timeout) != 0)
//            return -1;
//    }
//
//    int n = lwip_recv(_sock_fd, data, length, 0);
//    _is_connected = (n != 0);
//
//    return n;

}

// -1 if unsuccessful, else number of bytes received
int TCPSocketConnection::receive_all(char* data, int length)
{
    //ERR("receive_all() not yet implimented");
    //  if ((_sock_fd < 0) || !_is_connected)
//        return -1;
//
//    int readLen = 0;
//    TimeInterval timeout(_timeout);
//    while (readLen < length) {
//        if (!_blocking) {
//            //Wait for socket to be readable
//            if (wait_readable(timeout) != 0)
//                return readLen;
//        }
//
//        int ret = lwip_recv(_sock_fd, data + readLen, length - readLen, 0);
//        if (ret > 0) {
//            readLen += ret;
//        } else if (ret == 0) {
//            _is_connected = false;
//            return readLen;
//        } else {
//            return -1; //Connnection error
//        }
//    }
//    return readLen;
    receive(data,length);
}
