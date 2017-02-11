// Part 3
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(p28, p27, p29); // serial tx, serial rx, reset pin;

//Setup a new class for TMP36 sensor
class TMP36 {
public:
  TMP36(PinName pin);
  TMP36();
  float read();
private:
  //class sets up the AnalogIn pin
  AnalogIn _pin;
};

TMP36::TMP36(PinName pin) : _pin(pin) {} 
//This is an initializer list ... more to come in class 
// _pin(pin) means pass pin to the AnalogIn constructor

float TMP36::read() {
  //convert sensor reading to temperature in degrees C
  return ((_pin.read()*3.3) - 0.500)*100.0;
}

// instantiate new class to set p20 to analog input 
// to read and convert TMP36 sensor's voltage output
TMP36 myTMP36(p20);

int main() {
  float tempC, tempF;  
    
  // Make text white
  uLCD.color(WHITE);
  
  while(1) {
    tempC = myTMP36.read();
    
    //convert to degrees F
    tempF = (9.0*tempC)/5.0 + 32.0;
    
    //print current temp
    uLCD.cls();
    uLCD.circle(65, 65, 60, WHITE);
    uLCD.circle(65, 65, 59, WHITE);
    uLCD.circle(65, 65, 58, WHITE);
    uLCD.circle(65, 65, 57, WHITE);
  
    // Make text large
    uLCD.text_width(2);
    uLCD.text_height(2);  
    uLCD.locate(2,3);
    uLCD.printf("%5.2fC", tempC);
    uLCD.locate(2,5);
    uLCD.printf("%5.2fF", tempF);
    wait(1);
  }
}