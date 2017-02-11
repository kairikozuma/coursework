// Part 2

#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(p28, p27, p29); // serial tx, serial rx, reset pin;

int main() {
    uLCD.display_control(PORTRAIT);
    uLCD.cls();
    uLCD.printf("Bouncing Ball");
    uLCD.baudrate(BAUD_3000000); //jack up baud rate to max for fast display wait(1.0);
    //Set up initial conditions
    float fx=50.0,fy=21.0,vx=0.4,vy=0.3; 
    int x=50,y=21,radius=4;
    
    uLCD.background_color(BLACK); uLCD.cls();
    
    //draw borders
    uLCD.line(0, 0, 127, 0, BLUE); 
    uLCD.line(127, 0, 127, 127, BLUE); 
    uLCD.line(127, 127, 0, 127, BLUE); 
    uLCD.line(0, 127, 0, 0, BLUE);
    
    // Regular ball bounce speed
    for (int i=0; i<500; i++) {
        //draw ball
        uLCD.filled_circle(x, y, radius, WHITE);
        //bounce off edge walls and slow down a bit
        if ((x<=radius+1) || (x>=126-radius)) vx = -.95*vx;
        if ((y<=radius+1) || (y>=126-radius)) vy = -.95*vy;
        //erase old ball location 
        uLCD.filled_circle(x, y, radius, BLACK);
        //calculate new ball position 
        fx = fx + vx;
        fy = fy + vy;
        x =(int)fx;
        y =(int)fy;
    }
    
    // Pause for 2 seconds
    wait_ms(2000);
    
    // Reset ball and increase speed
    fx=50.0,fy=21.0,vx=4.4,vy=4.4; 
    
    // Increase radius to 15
    x=50,y=21,radius=15;
    
    for (int i=0; i<500; i++) {
        //draw ball
        uLCD.filled_circle(x, y, radius, WHITE);
        //bounce off edge walls and slow down a bit
        if ((x<=radius+4) || (x>=122-radius)) vx = -.95*vx; 
        if ((y<=radius+4) || (y>=122-radius)) vy = -.95*vy;
        //erase old ball location 
        uLCD.filled_circle(x, y, radius, BLACK);
        //calculate new ball position 
        fx = fx + vx;
        fy = fy + vy;
        x =(int)fx;
        y =(int)fy;
    } //end for loop
}//end main