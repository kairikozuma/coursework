#include "mbed.h"

PwmOut led(p21);
DigitalIn sw1(p19);
DigitalIn sw2(p20);

int main() {
    led.period(0.001f);      // 1 second period
    
    while(1) {
             
        if (!sw1 && !sw2) {
            led.write(0.0f);
        } else if (sw1 && !sw2) {
            led.write(.25f);
        } else if (!sw1 && sw2) {
            led.write(.50f);
        } else {
            led.write(1.0f);
        }
        wait(0.2);
    }
}