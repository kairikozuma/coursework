#ifndef ESP8266_H
#define ESP8266_H

#include "mbed.h"

#define SND_BUF_SIZE 1024

class ESP8266 
{
private:
    char _ssid[32];
    char _pwd[32];
    char _snd[SND_BUF_SIZE];
    RawSerial _esp;
    DigitalOut _reset;
    
public:
    ESP8266(PinName tx, PinName rx, PinName reset, char * SSID, char * password); 
    void send();
    void send(char * command);
    void setup();
    void reset();
    void baud(int baudrate);
    void getMAC();
    void getIP();
    
    int putc(int c);
    int getc();
    int readable();
    int writeable();
    void attach(Callback< void()> func, SerialBase::IrqType type);
};

#endif