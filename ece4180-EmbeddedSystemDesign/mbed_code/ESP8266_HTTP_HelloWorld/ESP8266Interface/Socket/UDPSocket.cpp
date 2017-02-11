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

#include "UDPSocket.h"

#include <string>
#include <algorithm>

UDPSocket::UDPSocket()
{
    endpoint_configured = false;
    endpoint_read = false;
    Endpoint currentEndpoint;
}

int UDPSocket::init(void)
{
    return 0;
}

// Server initialization
int UDPSocket::bind(int port)
{
    return 0;
}

// -1 if unsuccessful, else number of bytes written
int UDPSocket::sendTo(Endpoint &remote, char *packet, int length)
{
    Timer tmr;
    int idx = 0;


    confEndpoint(remote);
    
    // initialize transparent mode if not already done
    if(!endpoint_configured) {
        // initialize UDP (default id of -1 means transparent mode)
        //!wifi->start(ESP_UDP_TYPE, remote._ipAddress, remote._port, remote._id
        if(!wifi->startUDP(remote._ipAddress, remote._port, 0,length)) {
            return(-1);
        }
        endpoint_configured = true;
    }
    
    tmr.start();

    while ((tmr.read_ms() < _timeout) || _blocking) {

        idx += wifi->send(packet, length);

        if (idx == length)
            return idx;
    }
    return (idx == 0) ? -1 : idx;
}

// -1 if unsuccessful, else number of bytes received
int UDPSocket::receiveFrom(Endpoint &remote, char *buffer, int length)
{
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
    readEndpoint(remote);
    return (idx == 0) ? -1 : idx;
}

bool UDPSocket::confEndpoint(Endpoint & ep)
{
    currentEndpoint = ep;
    return true;
}

bool UDPSocket::readEndpoint(Endpoint & ep)
{
    ep = currentEndpoint;
    return true;
}
