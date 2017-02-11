// Hello World to sweep a servo through its full range

#include "mbed.h"
#include "Servo.h"

Servo myservo(p21);
AnalogIn pot(p19);

int main() {    
    while(1) {
        myservo = pot;
        wait(0.1);
    }
}
