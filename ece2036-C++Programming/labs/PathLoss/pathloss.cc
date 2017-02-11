// Wireless path loss assignment
// Pathloss class for ECE2036 Pathloss assignment
// Kairi Kozuma
// 10-17-2015

#include "math.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "qdisplay.h"
#include "Point.h"
#include "Line.h"
#include "PrintPixel.h"

using namespace std;

#define WALL_COUNT 56

// 256x256 plus 10 pixel border (32 bit colors)
#define HEIGHT 276
#define WIDTH 276
#define HEIGHT_IMAGE 256
#define WIDTH_IMAGE 256
#define BORDER 10
#define DEPTH 32

// Wireless path loss formula
#define PT 20
#define GT 0
#define GR 0
#define FREQUENCY 2.4e9
#define CVAL 3.0e8
#define XN 5
#define LOGF 10 * (log((4 * M_PI * FREQUENCY) / CVAL) / log(10))

// maxmimum and minimum signal
#define SIGNAL_MAX -20
#define SIGNAL_MID -50
#define SIGNAL_MIN -80

Line walls[WALL_COUNT];  // Each wall stored here

// Global values to store the values passed to the PrintPixel function
int nWalls = 0;
double sigVal = 0;
int rgb[3] = {0,0,0};

// Get the index from the x and y values
int getIndex(int x, int y) {
  return y * WIDTH + x;
}

// Get the x value from the index
int getXVal(int i) {
  return i % WIDTH;
}

// Get the y value from the index
int getYVal(int i) {
  return i / WIDTH;
}

// Get the normalized index value
int getNormalIndex(int i) {
  return (getXVal(i) - BORDER) + (getYVal(i) - BORDER) * WIDTH_IMAGE;
}

// Determine if the pixel is within border
bool isWithinBorder(int i) {
  return (getXVal(i) >= BORDER && getXVal(i) < WIDTH - BORDER)
      && (getYVal(i) >= BORDER && getYVal(i) < HEIGHT - BORDER);
}

// Calculate the signal strength
double calcSignalStrength(const Line& mLine) {
  int numWalls = 0;
  for (size_t j = 0; j < sizeof(walls) / sizeof(walls[0]); j++) {
    if (mLine.Intersects(walls[j])) {
	     numWalls++;
	  }
  }
  if (mLine.Length() == 0) {
    return 0;
  }
  nWalls = numWalls;
  double signalStrength = PT + GT + GR
    - LOGF
    - 20 * (log(mLine.Length()) / log(10))
    - numWalls * XN;

  return signalStrength;
}

// Get the color from the signal strength
QRgb getColor(double signalStrength) {
  if (signalStrength >= SIGNAL_MAX) {          // Return green if signal is max
    sigVal = SIGNAL_MAX;
    rgb[0] = 0;
    rgb[1] = 255;
    rgb[2] = 0;
    return QColor(0, 255, 0).rgb();
  } else if (signalStrength <= SIGNAL_MIN) {   // Return red if signal is min
    sigVal = SIGNAL_MIN;
    rgb[0] = 255;
    rgb[1] = 0;
    rgb[2] = 0;
    return QColor(255, 0, 0).rgb();
  } else {
    sigVal = signalStrength;
    double colorAdjust = fabs(50.0 + signalStrength) * (255/30.0);
  	if (signalStrength > SIGNAL_MID) {
  		rgb[0] = 255 - round(colorAdjust);
  		rgb[1] = 255;
  		rgb[2] = 0;
  	} else {
  		rgb[0] = 255;
  		rgb[1] = 255 - round(colorAdjust);
  		rgb[2] = 0;
    }
    return QColor(rgb[0], rgb[1], rgb[2]).rgb();
  }
}

int main(int argc, char** argv) {
  QApplication a(argc, argv);
  ifstream w("walls.txt");
  if (!w)
    {
      cout << "Oops, no walls.txt file" << endl;
      return(1);
    }

  // Read the walls file
  int i = 0;
  while(w)
    {
      int x0 = -1;
      int y0 = -1;
      int x1 = -1;
      int y1 = -1;
      w >> x0 >> y0 >> x1 >> y1;
      if (x0 < 0) break; // done

	  // Save new Line in walls array
	  walls[i] = Line(Point(x0 + BORDER, y0 + BORDER), Point(x1 + BORDER, y1 + BORDER));
	  i++;
    }
  w.close();

  // Create the display object
  QDisplay d(a);
  d.BlankImage(HEIGHT, WIDTH, DEPTH);
  d.Show(); // Make it screen visible

  // Get reference to display array
  QRgb* pixels = (QRgb*)d.ImageData();

  // Draw the walls
  for (size_t j = 0; j < sizeof(walls) / sizeof(walls[0]); j++) {
    Point* points = walls[j].getPoints();
    int numPoints = walls[j].getNumPoints();
    for (int i = 0; i < numPoints; i++) {
      pixels[getIndex(points[i].getX(), points[i].getY())] = QColor(0,0,0).rgb(); // Black
    }
    delete [] points;
  }

  d.Update();
  // Iterate through each pixel
  for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
	 // If the pixel is not black and within the box, calculate the signal
	 double signalStrength = 0.0;

	// Line connecting two points
    if (isWithinBorder(i) && pixels[i] != QColor(0,0,0).rgb()) {
      Line mLine = Line(Point(BORDER + 1, BORDER + 1), Point(getXVal(i), getYVal(i)));
      signalStrength = calcSignalStrength(mLine);
	  pixels[i] = getColor(signalStrength);
	  PrintPixel(getNormalIndex(i), sigVal, nWalls, rgb[0], rgb[1], rgb[2]);
	}
  }

  d.Update(); // Uncomment as needed
  d.Save("pathloss.png");
  a.exec();
  cout << "Done" << endl;
}
