#include "mbed.h"

BusOut LEDs(LED1, LED2, LED3, LED4);

const double MAX = 1.0;

/* Distance measuring unit analog input */
AnalogIn dmu(p20);
int main() {
    while(1) {
        double value = dmu;
        if (value < MAX / (5.0)) {
            LEDs = 0b0;
        } else if (value < MAX / (5.0/2.0)) {
            LEDs = 0b1;
        } else if (value < MAX / (5.0/3.0)) {
            LEDs = 0b11;
        } else if (value < MAX / (5.0/4.0)) {
            LEDs = 0b111;
        } else  {
            LEDs = 0b1111;
        }   
        wait (0.1);
    }
}
