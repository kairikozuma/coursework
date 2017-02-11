// Testomg the qdisplay API
// George F. Riley, Georgia Tech, Spring 2004
// ECE3090, Spring 2004

#include <iostream>
#include <fstream>
#include "qdisplay.h"
#include <vector>
#include <deque>

using namespace std;

#define INDEX(x,y,w) ((y)*(w)+x)
#define SIZE(w,h) ((w)*(h))
#define WHITE 0xff
#define BLACK 0x0
#define GRAY  0x80
#define ROW(p,w) ((p) / (w))
#define COL(p,w) ((p) % (w))
#define INRANGE(r, c, w, h) ((r) >= 0 && (r) < (h) && (c) >= 0 && (c) < (w))
typedef deque<int> IntDeq_t;
typedef vector<bool> VVec_t;

int main( int argc, char **argv )
{
  QApplication a(argc, argv);
  QDisplay     d(a);
  d.BlankImage(256, 256, 8);
  int w = d.Width();
  int h = d.Height();
  int depth = d.Depth();
  
  d.Show();
  
  //QRgb* pixels = (QRgb*)d.ImageData();
  unsigned char* pixels  = d.ImageData();
  
  for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
        {
          int index = y * w + x;
          if (x < w/2) pixels[index] = 0x0;
          else         pixels[index] = 0xff/2;
          //d.Update();
        }
      //pixels[index] = QColor(255, 0, 0).rgb();
      //d.Update(x, x, 1, 1);
      //d.Update();
    }
  a.exec();
}
