// The Bresenham algorithms for line and circle drawing
// George F. Riley, Georgia Tech, Fall 2009

void SetPixel(int x, int y)
{ // This exists just so this will compile.  For the actual
  // algorithm we would, of course, need to set the specified
  // pixel to a known (black?) value.
}

void MidpointLine(int x0, int y0, int x1, int y1)
{
  int dx = x1 - x0;
  int dy = y1 - y0;
  int d  = 2 * dy - dx;
  int incrE = 2 * dy;          // East increment
  int incrNE = 2 * (dy - dx);  // Northeast increment
  int x = x0;                  // Initial x
  int y = y0;                  // Initial y
  SetPixel(x, y);              // Initial line point
  while(x < x1) 
    {
      if (d <= 0)
        {
           d += incrE;         // Move East
        }
      else
        {
           d += incrNE;        // Move Northeast
           y++;                // and advance y
        }
      x++;
      SetPixel(x, y);          // Next pixel in line
    }
}

void MidpointCircle(int radius)
{
  int x = 0;
  int y = radius;
  double d = 5.0/4.0 - radius;
  SetPixel(x,y);
  while(y > x)
    {
      if (d < 0)
        {
          d += 2.0 * x + 3.0; // Select East
        }
      else
        {
          d += 2.0 * (x-y) + 5.0; // Select SE
          y--;
        }
      x++;
      SetPixel(x, y);
    }
}

int main()
{ // Keep the compiler happy
}
