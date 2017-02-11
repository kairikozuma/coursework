// Line class for ECE2036 Pathloss assignment
// Fall 2015
// Kairi Kozuma
// 10-17-2015

#ifndef __LINE_H__
#define __LINE_H__

#include "Point.h"

class Line
{
public:
  Line();                            // Default constructor
  Line(const Point&, const Point&);  // Construct with two endpoints
  Line(const Line&);                 // Copy constructor
  Line& operator=(const Line&);      // Assignment operator
  bool operator==(const Line&);      // Comparison operator
// Member variables
private:
  Point a;
  Point b;
  int numPoints;   // Number of points in a line
  bool isHoriz;    // Whether line is horizontal
  bool isVerti;    // Whether line is vertical
public:
  double Length() const; 				// Returns length of line segment
  bool Intersects(const Line&) const; 	// Returns true if line intersects
  Point getEndpointA() const; 			// Return first endpoint
  Point getEndpointB() const;		    // Return second endpoint
  Point* getPoints() const; 			// Returns array of points of line
  int getNumPoints() const; 			// Returns number of points in the array
  bool isHorizontal() const;            // Return whether line is horizontal
  bool isVertical() const;				// Return whether line is vertical
  void Print() const; // Prints the details of the line for debugging
};
#endif

