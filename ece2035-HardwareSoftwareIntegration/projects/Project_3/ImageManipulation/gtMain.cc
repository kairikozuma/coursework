// Main program for gtDisplay graphical interface
// George F. Riley, Georgia Tech, Spring 2015

// Kairi Kozuma 2035
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "gtDisplay.h"
#define pi 3.14159265358979323846

int Pixel(int y, int x, int twidth);
void MidpointCircle(uint32_t * pic, int w, int r);
void MidpointLine(uint32_t * pic, uint32_t col, double theta, int w, int r);
void DrawSpoke(uint32_t * pic, uint32_t col1, double theta, int w, int r);
void EraseSpoke(uint32_t * pic, double theta, int w, int r);
uint8_t ON = 255;
uint8_t OFF = 0;
uint32_t RED = gtMakeRGB(ON, OFF, OFF);
uint32_t GREEN = gtMakeRGB(OFF, ON, OFF);
uint32_t BLUE = gtMakeRGB(OFF, OFF, ON);
uint32_t WHITE = gtMakeRGB(ON, ON, ON);
uint32_t BLACK = gtMakeRGB(OFF, OFF, OFF);


int main(int argc, char** argv)
{  
  int testNum = 1;
  if (argc > 1) testNum = atol(argv[1]);
  std::cout << "Running test number " << testNum << std::endl;
  gtDisplayInit(argc, argv);
  
  // *** Problem 1 *** //                                                                                                                                    
  if (testNum == 1) {
  // Load Tech Tower Image                                                                                                                                 
  gtWindow ttwindow = gtLoadImage("TechTowerSign.jpg");
  uint32_t * ttmatrix = gtGet32BitPixels(ttwindow);

  // Store Dimensions of Loaded Image                                                                                                                      
  int i, j;
  int theight = gtGetHeight(ttwindow);
  int twidth = gtGetWidth(ttwindow);
  int tdepth = gtGetWidth(ttwindow);
  
  // Make Empty Image Matrix         
  gtWindow ttwindowinv = gtCreateBlankWindow(twidth, theight, tdepth);
  uint32_t * ttmatrixinv = gtGet32BitPixels(ttwindowinv);
  // Iterate Through Array to Invert It                                                                                                                    
  for (i = 0; i < theight; i++) {
    for(j = 0; j < twidth; j++) {
      ttmatrixinv[Pixel(theight - i - 1,j,twidth)] = ttmatrix[Pixel(i,j,twidth)];
    }
  }
  gtUpdate(ttwindowinv);
  gtRun();
  }
  

  // *** Problem 2 *** //                                                                                                                                    
  else if (testNum == 2) {	
  // Load Tech Tower Image                                                                                                                                 
  gtWindow ttwindow = gtLoadImage("TechTowerSign.jpg");
  uint32_t * ttmatrix = gtGet32BitPixels(ttwindow);

  // Store Dimensions of Loaded Image                                                                                                                      
  int i, j;
  int theight = gtGetHeight(ttwindow);
  int twidth = gtGetWidth(ttwindow);
  // Make Empty Image Matrix
  gtWindow ttwindowgray = gtCreateBlankWindow(twidth, theight, 8);
  uint8_t * ttmatrixgray = gtGet8BitPixels(ttwindowgray);
  uint32_t pxcol;
  uint8_t pxgray;
  for (i = 0; i < theight; i++) {
    for(j = 0; j < twidth; j++) {
      pxcol = ttmatrix[Pixel(i,j,twidth)];
      pxgray = (uint8_t)(gtGetRed(pxcol)/3 + gtGetGreen(pxcol)/3 + gtGetBlue(pxcol)/3);
      ttmatrixgray[Pixel(i,j,twidth)] = pxgray;
    }
  }      
  gtUpdate(ttwindowgray);
  gtRun();
  }

  // *** Problem 3 *** //
  else if (testNum == 3) {
  int size = 512;
  int rate = 24;
  int r = 200;
  int c,d;
  int times = 5;
  double shift = pi/15;
  gtWindow mywheel = gtCreateBlankWindow(size, size, 3);
  gtUpdateRate(mywheel,rate);
  uint32_t * wheelmat = gtGet32BitPixels(mywheel);
  MidpointCircle(wheelmat, size,  r);
  gtUpdate(mywheel);
  for (c = 0; c <= times; c++) {
    for (d = 0; d < 15; d++) {
      DrawSpoke(wheelmat, RED, d*shift, size, r);
      gtUpdate(mywheel);
      EraseSpoke(wheelmat, d*shift, size, r);
    }
  }
  gtUpdate(mywheel);
  gtRun();
  }
}

void DrawSpoke(uint32_t * pic, uint32_t col1, double theta, int w, int r) {
  uint32_t col2;
  uint32_t col3;
  if(col1 == RED) {
    col2 = GREEN;
    col3 = BLUE;
  }
  else if(col1 == GREEN) {
  col2 = BLUE;
  col3 = RED;
  }
  else if(col1 == BLUE){
  col2 = RED;
  col3 = GREEN;
  }
  MidpointLine(pic, col1, theta, w, r);
  MidpointLine(pic, col2, theta + pi/6, w, r);
  MidpointLine(pic, col3, theta + pi/6 + pi/6, w, r);
}

void EraseSpoke(uint32_t * pic, double theta, int w, int r) {
  uint32_t col = 0xFFFFFF;
  MidpointLine(pic, col, theta, w, r);
  MidpointLine(pic, col, theta + pi/6, w, r);
  MidpointLine(pic, col, theta + pi/6 + pi/6, w, r);
}
void MidpointLine(uint32_t * pic, uint32_t col, double theta, int w, int r) {
  int x0 = 0;
  int y0 = 0;
  int x1,  y1;
  int cent = (w*w)/2 - w/2;
  while (theta > pi/2) {
    theta -= pi/2;
  }
  if (theta <= pi/4) {
    x1 = round(r*cos(theta));
    y1 = round(r*sin(theta));
  }
  else if (theta > pi/4 && theta <= pi/2) {
    x1 = round(r*cos(pi/2 - theta));
    y1 = round(r*sin(pi/2 - theta));
  }
  int dx = x1;
  int dy = y1;
  int d = 2 * dy - dx;
  int incrE = 2 * dy;
  int incrNE = 2 * (dy - dx);
  int x = x0;
  int y = y0;
  if(theta <= pi/4) {
    pic[cent + y * w + x] = col;
    pic[cent + x * w - y] = col;
    pic[cent - y * w - x] = col;
    pic[cent - x * w + y] = col;
    while(x < x1) {
      if(d <= 0) {d += incrE;}
      else {d += incrNE; y++;}
      x++;
      pic[cent + y * w + x] = col;
      pic[cent + x * w - y] = col; 
      pic[cent - y * w - x] = col;
      pic[cent - x * w + y] = col;
    }
  }
  if(theta > pi/4) {
    pic[cent + x * w + y] = col;
    pic[cent + y * w - x] = col;
    pic[cent - x * w - y] = col;
    pic[cent - y * w + x] = col;
    while(x < x1) {
      if(d <= 0) {d += incrE;}
      else {d += incrNE; y++;}
      x++;
      pic[cent + x * w + y] = col;
      pic[cent + y * w - x] = col;
      pic[cent - x * w - y] = col;
      pic[cent - y * w + x] = col;
    }
  }

}


void MidpointCircle(uint32_t * pic, int w, int r) {
  int x = 0;
  int y = r;
  int cent = (w*w)/2 - w/2;
  double d = 5.0/4.0 - r;
  pic[y * w + x] = 0;
  while(y > x)
    {
      if (d < 0) {d += 2.0 * x + 3.0;} // Select East
      else {d += 2.0 * (x-y) + 5.0; // Select SE 
      y--;}
      x++;
      pic[cent + ((y-1) * w) + x - 1] = 0;
      pic[cent - (y * w) - x] = 0;
      pic[cent + ((x-1) * w) + y - 1] = 0;
      pic[cent - (x * w) - y] = 0;
      pic[cent + ((y-1) * w) - x] = 0;
      pic[cent - (y * w) + x - 1] = 0;
      pic[cent + ((x-1) * w) - y] = 0;
      pic[cent - (x * w) + y - 1] = 0;
    }
}

int Pixel(int y, int x, int twidth) {
  return (y)*(twidth) +(x);
}
