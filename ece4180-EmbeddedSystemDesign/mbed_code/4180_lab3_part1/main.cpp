#include "mbed.h"

/* Declare tickr */
Ticker ticker;

/* Declare leds */
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

/* Global count variable to keep track of how many seconds passed */
int count = 0;

void led_blink() {
    /* Cycle through 0 through 11 */
    if (count == 11) {
        count = 0;
    } else {
        count++;
    }
    /* Modulus to turn on leds at different counts */
    led1 = 1;
    led2 = !(count % 2);
    led3 = !(count % 3);
    led4 = !(count % 4);
    
    /* Wait some time */
    wait(0.2);
    
    /* Turn of all leds */
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}
 
int main() {
    ticker.attach(&led_blink, 1.0); /* Interrupt every 1 second */
 
    // Forever loop
    while(1) {
    }
}