#include "mbed_debug.h"
#include "TMP36.h"

// Implementation of ™P36
TMP36::TMP36(PinName pin) : _pin(pin) {} 

float TMP36::read() {
  //convert sensor reading to temperature in degrees C
  return ((_pin.read()*3.3) - 0.500)*100.0;
}
