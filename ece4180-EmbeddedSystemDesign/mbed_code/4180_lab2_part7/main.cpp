// Sweep the motor speed from full-speed reverse (-1.0) to full speed forwards (1.0)
 
#include "mbed.h"
#include "Motor.h"
 
Motor m(p23, p6, p5); // pwm, fwd, rev
 
int main() {
    while(1) {
        for (float s= -1.0; s < 1.0 ; s += 0.01) {
           m.speed(s); 
           wait(0.02);
        }
        for (float s= 1.0; s > -1.0 ; s -= 0.01) {
           m.speed(s); 
           wait(0.02);
        }
    }
}