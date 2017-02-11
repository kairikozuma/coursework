// Point class for ECE2036 Pathloss assignment
// Fall 2015
// Kairi Kozuma
// 10-17-2015

#ifndef __POINT_H__
#define __POINT_H__

class Point
{
// Public methods
public:
  Point();         					// Default constructor
  Point(int, int);  				// Integer constructor with initial values
  Point(const Point&); 				// Copy constructor
  Point& operator=(const Point&);   // Assignment operator
  bool operator==(const Point&);    // Comparison operator
  // Getters
  int getX() const;
  int getY() const;
// Member variables
private:
  int x;
  int y;
};
#endif

