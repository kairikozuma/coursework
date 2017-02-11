// Implement the PrintPixel() function

#include <iostream>
#include <iomanip>

#include "PrintPixel.h"

using namespace std;

void PrintPixel(int pixel, // Pixel index
                double dB, // Computed signal strength
                int walls, // Number of walls intersected with this pixel line
                int red,   // Computed red component of image color
                int green, // Computed green component of image color
                int blue)  // Computed blue component if image color
{
  cout << fixed << setprecision(3)
       << " p " << pixel
       << " dB " << dB
       << " walls " << walls
       << " red " << red
       << " green " << green
       << " blue " << blue << endl;
}


