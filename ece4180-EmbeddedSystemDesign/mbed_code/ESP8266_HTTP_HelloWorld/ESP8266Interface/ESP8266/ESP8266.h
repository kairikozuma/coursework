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
 *
 * @section DESCRIPTION
 *
 * ESP8266 serial wifi module
 *
 * Datasheet:
 *
 * http://www.electrodragon.com/w/Wi07c
 */

#ifndef ESP8266_H
#define ESP8266_H

#include "mbed.h"
#include "CBuffer.h"

#define DEFAULT_WAIT_RESP_TIMEOUT 500
#define ESP_TCP_TYPE 1
#define ESP_UDP_TYPE 0 
#define ESP_MBUFFE_MAX 256

/**
 * The ESP8266 class
 */
class ESP8266
{

public:
    /**
    * Constructor
    *
    * @param tx mbed pin to use for tx line of Serial interface
    * @param rx mbed pin to use for rx line of Serial interface
    * @param reset reset pin of the wifi module ()
    * @param ssid ssid of the network
    * @param phrase WEP, WPA or WPA2 key
    * @param baud the baudrate of the serial connection
    */
    ESP8266( PinName tx, PinName rx, PinName reset, const char * ssid, const char * phrase, uint32_t baud );

    /**
    * Connect the wifi module to the ssid contained in the constructor.
    *
    * @return true if connected, false otherwise
    */
    bool join();

    /**
    * Same as Join: connect to the ssid and get DHCP settings
    * @return true if successful
    */
    bool connect();
    
    /**
    * Check connection to the access point
    * @return true if successful
    */
    bool is_connected();

    /**
    * Disconnect the ESP8266 module from the access point
    *
    * @return true if successful
    */
    bool disconnect();
    
    /*
    * Start up a UDP or TCP Connection
    * @param type 0 for UDP, 1 for TCP
    * @param ip A string that contains the IP, no quotes
    * @param port Numerical port number to connect to
    * @param id number between 0-4, if defined it denotes ID to use in multimode (Default to Single connection mode with -1)
    * @return true if sucessful, 0 if fail
    */
    bool start(bool type, char* ip, int port, int id = -1);

    /*
    * Legacy Start for UDP only connection in transparent mode
    * @param ip A string that contains the IP, no quotes
    * @param id number between 0-4
    * @param port Numerical port number to connect to
    * @param length number of characters in the message being sent
    */
    bool startUDP(char* ip, int port, int id, int length);

    /*
    *Starts the ESP chip as a TCP Server
    *@param port Numerical port of the server, default is 333
    */
    bool startTCPServer(int port = 333);

    /**
    * Close a connection
    *
    * @return true if successful
    */
    bool close();
    
    /**
    * Return the IP address 
    * @return IP address as a string
    */
    char* getIPAddress();

    /**
    * Return the IP address from host name
    * @return true on success, false on failure
    */    
    bool gethostbyname(const char * host, char * ip);

    /**
    * Reset the wifi module
    */
    void reset();
    
    /**
    * Reboot the wifi module
    */
    bool reboot();

    /**
    * Check if characters are available
    *
    * @return number of available characters
    */
    int readable();

    /**
    * Check if characters are available
    *
    * @return number of available characters
    */
    int writeable();

    /**
    * Read a character
    *
    * @return the character read
    */
    char getc();

    /**
    * Write a character
    *
    * @param the character which will be written
    */
    int putc(char c);

    /**
    * Flush the buffer
    */
    void flush();

    /**
    * Send a command to the wifi module. Check if the module is in command mode. If not enter in command mode
    *
    * @param str string to be sent
    * @param ACK string which must be acknowledge by the wifi module. If ACK == NULL, no string has to be acknowledged. (default: "NO")
    * @param res this field will contain the response from the wifi module, result of a command sent. This field is available only if ACK = "NO" AND res != NULL (default: NULL)
    *
    * @return true if successful
    */
    bool sendCommand(const char * cmd, const char * ack = NULL, char * res = NULL, int timeout = DEFAULT_WAIT_RESP_TIMEOUT);

    /**
    * Send a string to the wifi module by serial port. This function desactivates the user interrupt handler when a character is received to analyze the response from the wifi module.
    * Useful to send a command to the module and wait a response.
    *
    *
    * @param str string to be sent
    * @param len string length
    * @param ACK string which must be acknowledge by the wifi module. If ACK == NULL, no string has to be acknoledged. (default: "NO")
    * @param res this field will contain the response from the wifi module, result of a command sent. This field is available only if ACK = "NO" AND res != NULL (default: NULL)
    *
    * @return true if ACK has been found in the response from the wifi module. False otherwise or if there is no response in 5s.
    */
    int send(const char * buf, int len);

    static ESP8266 * getInstance() {
        return inst;
    };

protected:
    RawSerial wifi;
    DigitalOut reset_pin;
    char phrase[30];
    char ssid[30];
    char ipString[20];
    CircBuffer<char> buf_ESP8266;

    static ESP8266 * inst;

    void attach_rx(bool null);
    void handler_rx(void);


    typedef struct STATE {
        bool associated;
        bool cmdMode;
    } State;

    State state;
};

#endif