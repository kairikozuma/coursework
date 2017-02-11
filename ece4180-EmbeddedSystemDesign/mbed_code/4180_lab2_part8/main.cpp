#include "mbed.h"
//Adafruit MEMs SPW2430 microphone demo
BusOut myleds(LED1,LED2,LED3,LED4);
 
class microphone
{
public :
    microphone(PinName pin);
    float read();
    operator float ();
private :
    AnalogIn _pin;
};
microphone::microphone (PinName pin):
    _pin(pin)
{
}
float microphone::read()
{
    return _pin.read();
}
inline microphone::operator float ()
{
    return _pin.read();
}
 
microphone mymicrophone(p16);
 
int main()
{
    while(1) {
//read in, subtract 0.67 DC bias, take absolute value, and scale up .1Vpp to 15 for builtin LED display
        myleds = int(abs((mymicrophone - (0.67/3.3)))*500.0);
//Use an 8kHz audio sample rate (phone quality audio);
        wait(1.0/8000.0);
    }
}
 