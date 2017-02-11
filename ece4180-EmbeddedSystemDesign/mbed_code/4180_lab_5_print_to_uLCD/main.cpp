// uLCD-144-G2 demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;
Serial pc(USBTX,USBRX);

int main()
{
    while(1) {
        char c = pc.getc();
        pc.putc(c);
        uLCD.putc(c);    
//        uLCD.cls();
    }
}



