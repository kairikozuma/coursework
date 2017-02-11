#include "mbed.h"

PwmOut pwr_trans(p21);

int main() {
   pwr_trans.period(1e-1); 
    while(1) {
        for(double i = 0.0; i < 1.0; i += 0.1) {
            pwr_trans.write(i);
            wait(1);
        }
    }
}
