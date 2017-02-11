#include "mbed.h"

DigitalOut motor(p20);

int main() {
    while(1) {
        motor = 1;
        wait(1);
        motor = 0;
        wait(1);
    }
}
