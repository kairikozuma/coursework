#include "mbed.h"
 
DigitalOut myled(p20);
DigitalIn dipsw(p19);

// Use dip switch to turn on/off LED
int main()
{ 
    while(1) {
        if (!dipsw) {
            myled = 1;    
        } else {
            myled = 0;
        }
        wait(.1);
    }
}