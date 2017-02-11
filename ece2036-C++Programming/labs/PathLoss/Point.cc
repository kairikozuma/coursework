// Point class for ECE2036 Pathloss assignment
// Kairi Kozuma
// 10-17-2015
// Fall 2015

#include "Point.h"

// Default constructor
Point::Point() : x(0), y(0) {}

// Integer constructor with initial x and y values
Point::Point(int xVal, int yVal) : x(xVal), y(yVal) {}

// Copy constructor
Point::Point(const Point& rhs) {
  x = rhs.getX();
  y = rhs.getY();
}

// Assignment operator
Point& Point::operator=(const Point& rhs) {
  x = rhs.getX();
  y = rhs.getY();
  return *this;
}

// Comparison operator
bool Point::operator==(const Point& rhs) {
  return (x == rhs.getX() && y == rhs.getY());
}

// Getters
int Point::getX() const {
  return x;
}

int Point::getY() const {
  return y;
}
