// Wireless path loss assignment
// ECE3090, Georgia Tech, Fall 2005

#include "math.h"

#include <iostream>
#include <fstream>

#include "qdisplay.h"
#include "Point.h"
#include "Line.h"
#include "PrintPixel.h"

using namespace std;

#define WALL_COUNT 56

Line walls[WALL_COUNT];  // Each wall stored here

int main(int argc, char** argv)
{

  QApplication a(argc, argv);
  ifstream w("walls.txt");
  if (!w)
    {
      cout << "OOps, no walls.txt file" << endl;
      return(1);
    }
  
  // Read the walls file
  while(w)
    {
      int x0 = -1;
      int y0 = -1;
      int x1 = -1;
      int y1 = -1;
      w >> x0 >> y0 >> x1 >> y1;
      if (x0 < 0) break; // done
      // Your code here.  Save wall info in the walls array
    }
  w.close();

  // Create the display object
  QDisplay d(a);
  d.BlankImage(276, 276, 32); // 256x256 plus 10 pixel border (32 bit colors)
  d.Show(); // Make it screen visible
  
  // Your code here.  Create 16 threads and compute the signal streangth
  // at each point; Have the threads update the pixel array with the
  // correct color as it is computed.

  // d.Update(); // Uncomment as needed
  d.Save("pathloss.png");
  a.exec();
  cout << "Done" << endl;
}

      
  
    
