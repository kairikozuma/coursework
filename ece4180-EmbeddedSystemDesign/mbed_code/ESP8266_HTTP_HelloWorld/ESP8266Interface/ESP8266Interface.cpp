#include "ESP8266Interface.h"

ESP8266Interface::ESP8266Interface( PinName tx, PinName rx, PinName reset,
                                const char * ssid, const char * phrase, uint32_t baud ) :
    ESP8266(tx, rx, reset, ssid, phrase, baud )
{
}

int ESP8266Interface::init()
{
    ESP8266::reset();
    return 0;
}

bool ESP8266Interface::connect()
{
    return ESP8266::connect();
}

int ESP8266Interface::disconnect()
{
    return ESP8266::disconnect();
}

char * ESP8266Interface::getIPAddress()
{
    return ESP8266::getIPAddress();
}