// uLCD-144-G2 demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"

#include "LSM9DS1.h"
#define PI 3.14159
// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -4.94 // Declination (degrees) in Atlanta,GA.


Serial pc(USBTX, USBRX);
uLCD_4DGL uLCD(p28,p27,p30); // LCD serial tx, serial rx, reset pin;
LSM9DS1 IMU(p9, p10, 0xD6, 0x3C); // IMU

// Previous location
int prev_x = 0;
int prev_y = 0;
    
void setup() {
     
//    pc.printf("setup");
    // Setup LCD
    uLCD.cls();
    uLCD.printf("Change baudrate......");
    uLCD.baudrate(3000000); //jack up baud rate to max for fast display
        
    uLCD.background_color(BLACK);
    uLCD.cls();
    
    // Calibrate IMU
    IMU.begin();
//    IMU.calibrate(1);
//    IMU.calibrateMag(0);
    
//      pc.printf("setup ends");
}

// Scale +-1 to 0 to 127
int scale(double a) {
    return (int) ((64.0) + a * (-64.0));
}

// X and Y ranges from +-1
void updateBubble(double x, double y) {
    uLCD.filled_circle(prev_x, prev_y, 10, BLACK);
    prev_x = scale(x);
    prev_y = scale(y);
    uLCD.filled_circle(prev_x, prev_y, 10, YELLOW);
    uLCD.circle(64, 64, 12, WHITE);
}

int main()
{   
    setup();
    while(1) {
      // Read in accelerometer if valid
      while(!IMU.accelAvailable());
      IMU.readAccel();
      double x = IMU.calcAccel(IMU.ax);
      double y = IMU.calcAccel(IMU.ay);   
      updateBubble(x, y);
//      pc.printf("%f %f", x, y);
      wait(0.05);
    }
}



