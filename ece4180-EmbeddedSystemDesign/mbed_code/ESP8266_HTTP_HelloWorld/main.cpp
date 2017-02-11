#include "mbed.h"
#include "HTTPClient.h"
#include "ESP8266Interface.h"
#include "TCPSocketConnection.h"
#include "Websocket.h"

ESP8266Interface wifi(D1,D0,D2,"GTother","GeorgeP@1927",115200); // TX,RX,Reset,SSID,Password,Baud
RawSerial pc(USBTX, USBRX); // tx, rx
HTTPClient http;
char str[512];
int main()
{
    pc.baud(9600);
    pc.printf("Attempt to connect...");
    wifi.init(); //Reset
    wifi.connect(); //Use DHCP
    pc.printf("IP Address is %s\n", wifi.getIPAddress());
    
    //GET
    pc.printf("\nTrying to fetch page using GET...\n\r");
    int ret = http.get("http://54.175.222.246/get", str, 512);//IP address is httpbin.org/get
    if (!ret) {
        pc.printf("Page fetched successfully - read %d characters\n\r", strlen(str));
        pc.printf("Result: %s\n\r", str);
    } else {
        pc.printf("Error - ret = %d - HTTP return code = %d\n\r", ret, http.getHTTPResponseCode());
    }
    //POST
    HTTPMap map;
    HTTPText inText(str, 512);
    map.put("Hello", "World");
    map.put("test", "1234");
    printf("\nTrying to POST data to httpbin.org/post...\n\r");
    ret = http.post("http://54.175.222.246/post", map, &inText);//IP address is httpbin.org/post
    if (!ret)
    {
      printf("Executed POST successfully - read %d characters\n\r", strlen(str));
      printf("Result: %s\n\r", str);
    }
    else
    {
      printf("Error - ret = %d - HTTP return code = %d\n\r", ret, http.getHTTPResponseCode());
    }

}

