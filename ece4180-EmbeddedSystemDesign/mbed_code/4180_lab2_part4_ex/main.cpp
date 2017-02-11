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

// Previous degree
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
    IMU.calibrate(1);
    IMU.calibrateMag(0);
    
    uLCD.circle(64, 64, 63, WHITE);
}

// X and Y ranges from +-1
void updateDegree(double mx, double my, double mz) {
    
    uLCD.line(64, 64, prev_x, prev_y, BLACK);
    
//    my = -my;
    float heading;
    if (my == 0.0)
        heading = (mx < 0.0) ? 180.0 : 0.0;
    else
        heading = atan2(my, mx)*360.0/(2.0*PI);
    
    pc.printf("%f degrees %d %d\n", heading, prev_x, prev_y);
    //pc.printf("heading atan=%f \n\r",heading);
    heading -= DECLINATION; //correct for geo location
    if(heading>180.0) heading = heading - 360.0;
    else if(heading<-180.0) heading = 360.0 + heading;
    else if(heading<0.0) heading = 360.0  + heading;

    pc.printf("%f degrees %d %d\n", heading, prev_x, prev_y);
    prev_x = 64 + (int)(62 * cos(heading * PI / 180.0));
    prev_y = 64 + (int)(62 * sin(heading * PI / 180.0));
    // Convert everything from radians to degrees:
    //heading *= 180.0 / PI;
//    double pitch *= 180.0 / PI;
//    double roll  *= 180.0 / PI;
    
    uLCD.line(64, 64, prev_x, prev_y, YELLOW);
}

int main()
{   
    setup();
    while(1) {
      IMU.readMag();
      double mx = IMU.calcMag(IMU.mx);
      double my = IMU.calcMag(IMU.my);
      double mz = IMU.calcMag(IMU.mz);
      updateDegree(mx, my, mz);
      wait(0.1);
    }
}



