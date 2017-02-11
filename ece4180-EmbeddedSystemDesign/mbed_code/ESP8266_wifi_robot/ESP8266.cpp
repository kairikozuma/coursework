#include "ESP8266.h"


ESP8266::ESP8266(PinName tx, PinName rx, PinName reset, char * SSID, char * password) :
    _esp(tx, rx), _reset(reset) {
    strcpy(_ssid, SSID);
    strcpy(_pwd, password);        
}
 
//  Setup for ESP8266
void ESP8266::setup() {
    
    /* Set wifi mode */
    send("wifi.setmode(wifi.STATION)\r\n");
    
    /* Set SSID and password */
    strcpy(_snd, "wifi.sta.config(\"");
    strcat(_snd, _ssid);
    strcat(_snd, "\",\"");
    strcat(_snd, _pwd);
    strcat(_snd, "\")\r\n");
    send();
    
    wait(7);

    /* Set up HTTP server */
    send("srv=net.createServer(net.TCP)\r\n");
    send("srv:listen(80,function(conn)\r\n");
    send("conn:on(\"receive\",function(conn,payload)\r\n");
    send("print(payload:match(\"Z%u\"))\r\n");
    send("conn:send(\"<!DOCTYPE html>\")\r\n");
    send("conn:send(\"<html>\")\r\n");
    send("conn:send(\"<h1>Control Robot</h1>\")\r\n");
    send("conn:send('<form action=\"\" method=\"POST\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZF\" value=\"Forward\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZB\" value=\"Back\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZL\" value=\"Left\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZR\" value=\"Right\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZS\" value=\"Servo Left\">\r\n')");
    send("conn:send('<input type=\"submit\" name=\"ZP\" value=\"Servo Right\">\r\n')");
    send("conn:send('</form>\r\n')");
    send("conn:send(\"</html>\")\r\n"); 
    send("end)\r\n");
    send("conn:on(\"sent\",function(conn) conn:close() end)\r\n");
    send("end)\r\n");
    wait(10);
}
 
void ESP8266::send()
{
    _esp.printf("%s", _snd);
    Thread::wait(500);
}

void ESP8266::send(char * command) {
    _esp.printf("%s", command);
    Thread::wait(500);
}

void ESP8266::reset() {
    _reset = 0;
    wait(1); /* Not RTOS friendly */
    _reset = 1;
}

void ESP8266::baud(int baudrate) {
    char buffer[32];
    sprintf(buffer, "AT+CIOBAUD=%d\r\n", baudrate);
    send(buffer);
    
    _esp.baud(baudrate);
}

void ESP8266::getIP() {
    send("\r\nprint(wifi.sta.getip())\r\n");  
}

void ESP8266::getMAC() {
    send("\r\nprint(wifi.sta.getmac())\r\n");  
}

int ESP8266::putc(int c) {
    return _esp.putc(c);
}

int ESP8266::getc() {
    return _esp.getc();
} 
    
int ESP8266::readable() {
    return _esp.readable();
}
int ESP8266::writeable() {
    return _esp.writeable();
}

void ESP8266::attach(Callback< void()> func, SerialBase::IrqType type) {
    _esp.attach(func, type);
}
