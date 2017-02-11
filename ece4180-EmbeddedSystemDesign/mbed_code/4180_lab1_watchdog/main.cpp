#include "mbed.h"

DigitalOut myled(p20);
DigitalIn dipsw(p19);

// Simon's Watchdog code from
// http://mbed.org/forum/mbed/topic/508/
class Watchdog {
public:
// Load timeout value in watchdog timer and enable
    void kick(float s) {
        LPC_WDT->WDCLKSEL = 0x1;                // Set CLK src to PCLK
        uint32_t clk = SystemCoreClock / 16;    // WD has a fixed /4 prescaler, PCLK default is /4
        LPC_WDT->WDTC = s * (float)clk;
        LPC_WDT->WDMOD = 0x3;                   // Enabled and Reset
        kick();
    }
// "kick" or "feed" the dog - reset the watchdog timer
// by writing this required bit pattern
    void kick() {
        LPC_WDT->WDFEED = 0xAA;
        LPC_WDT->WDFEED = 0x55;
    }
};
 
// Setup the watchdog timer
Watchdog wdt;
 
int main() {
    int count = 0;

// setup a 5 second timeout on watchdog timer hardware
// needs to be longer than worst case main loop exection time
    wdt.kick(5.0);  
    
    while(1) {
        if (count == 5) {
            while(1) {};
        }
        if (!dipsw) {
            myled = 1;
            count ++;
            while (!dipsw) {
            }
        } else {
            myled = 0;
        }
        wait(.1);
        
        // End of main loop so "kick" to reset watchdog timer and avoid a reset
        wdt.kick();
    }
}