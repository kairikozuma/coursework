/* ESP8266Interface.h */
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
 
#ifndef ESP8266INTERFACE_H_
#define ESP8266INTERFACE_H_

#include "ESP8266.h"
#include "Endpoint.h"

 /**
 * Interface using ESP8266 to connect to an IP-based network
 */
class ESP8266Interface: public ESP8266 {
public:

    /**
    * Constructor
    *
    * \param tx mbed pin to use for tx line of Serial interface
    * \param rx mbed pin to use for rx line of Serial interface
    * \param reset reset pin of the wifi module ()
    * \param ssid ssid of the network
    * \param phrase WEP or WPA key
    * \param baud the baudrate of the serial connection (defaults to 115200, diff revs of the firmware use diff baud rates
    */
  ESP8266Interface(PinName tx, PinName rx, PinName reset, const char * ssid, const char * phrase, uint32_t baud = 115200 );

  /** Initialize the interface with DHCP.
  * Initialize the interface and configure it to use DHCP (no connection at this point).
  * \return 0 on success, a negative number on failure
  */
  int init(); //With DHCP

  /** Connect
  * Bring the interface up, start DHCP if needed.
  * \return 0 on success, a negative number on failure
  */
  bool connect();
  
  /** Disconnect
  * Bring the interface down
  * \return 0 on success, a negative number on failure
  */
  int disconnect();
  
  /** Get IP address
  *
  * \return ip address
  */
  char* getIPAddress();
  
private:
};

#include "UDPSocket.h"

#endif /* ESP8266INTERFACE_H_ */
