#include "mbed.h"

// The sinewave is created on this pin
AnalogOut sine_pin(p18);
const double pi = 3.141592653589793238462;
const double amplitude = 1.0f;
const double offset = 65535/2;

int main()
{
    double rads = 0.0;
    uint16_t sample = 0;
    
    while(1) {
        // sinewave output
        for (int i = 0; i < 360; i++) {
            rads = (pi * i) / 180.0f;
            sample = (uint16_t)(amplitude * (offset * (cos(rads + pi/2))) + offset);
            sine_pin.write_u16(sample);
        }
    }
}