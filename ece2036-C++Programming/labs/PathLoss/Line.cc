// Line class for ECE2036 Pathloss assignment
// Fall 2015
// Kairi Kozuma
// 10-17-2015

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Line.h"
#include "Point.h"

using namespace std;

// Default constructor
Line::Line() : a(0,0), b(0,0) {}

// Line constructor from two points
Line::Line(const Point& a0, const Point& b0) : a(a0), b(b0) {

  // Calculate change in x and y of line
  int deltaX = b.getX() - a.getX();
  int deltaY = b.getY() - a.getY();

  int longerSide = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

  // Check whether line is horizontal or vertical
  isHoriz = (deltaY == 0);
  isVerti = (deltaX == 0);

  // Initialize number of points in a line
  numPoints = longerSide + 1;
}

// Copy constructor
Line::Line(const Line& rhs) {
  // Copy the end points
  a = rhs.getEndpointA();
  b = rhs.getEndpointB();

  // Copy the points of the line
  numPoints = rhs.getNumPoints();

  // Copy whether line is vertical or horizontal
  isHoriz = rhs.isHorizontal();
  isVerti = rhs.isVertical();
}

// Assignment operator
Line& Line::operator=(const Line& rhs) {

  // Copy the end points
  a = rhs.getEndpointA();
  b = rhs.getEndpointB();

  // Copy the number of points in line
  numPoints = rhs.getNumPoints();

  // Copy whether line is vertical or horizontal
  isHoriz = rhs.isHorizontal();
  isVerti = rhs.isVertical();

  return *this;
}


// Comparison operator
bool Line::operator==(const Line& rhs) {
  return (a == rhs.getEndpointA() && b == rhs.getEndpointB())
      || (a == rhs.getEndpointB() && b == rhs.getEndpointA());
}

// Get the length of the line using Pythagorean Theorem
double Line::Length() const {
  return sqrt(pow(a.getX() - b.getX(), 2)  + pow(a.getY() - b.getY(), 2));
}

// Determine if a line intersects this line
// Assumes that the rhs is a horizontal or vertical line
bool Line::Intersects(const Line& rhs) const {

  if (rhs.isHorizontal()) {
    if (isHorizontal()) return false;

    int y_min = a.getY();
    int y_max = b.getY();
    int y_val = rhs.getEndpointA().getY();
    if (y_min > y_max) {
      int temp = y_min;
      y_min = y_max;
      y_max = temp;
    }

    int x = a.getX();
    int y = a.getY();

    int x_min = rhs.getEndpointA().getX();
    int x_max = rhs.getEndpointB().getX();
    if (x_min > x_max) {
      int temp = x_min;
      x_min = x_max;
      x_max = temp;
    }

    if (isVertical()) {
      return (y_val <= y_max && y_val >= y_min)
          && (x <= x_max && x >= x_min);
    }

    if (y_val > y_max || y_val < y_min) return false;

    double slope = (double)(b.getY() - a.getY()) / (b.getX() - a.getX());
    double x_val = (y_val - y) / slope + x;

    return x_val >= x_min && x_val <= x_max;

  } else if (rhs.isVertical()) {
    if (isVertical()) return false;

    int x_min = a.getX();
    int x_max = b.getX();
    int x_val = rhs.getEndpointA().getX();
    if (x_min > x_max) {
      int temp = x_min;
      x_min = x_max;
      x_max = temp;
    }

    int y_min = rhs.getEndpointA().getY();
    int y_max = rhs.getEndpointB().getY();
    if (y_min > y_max) {
      int temp = y_min;
      y_min = y_max;
      y_max = temp;
    }

    int x = a.getX();
    int y = a.getY();

    if (isHorizontal()) {
        return (x_val <= x_max && x_val >= x_min)
            && (y <= y_max && y >= y_min);
    }

    if (x_val > x_max || x_val < x_min) return false;

    double slope = (double)(b.getY() - a.getY()) / (b.getX() - a.getX());
    double y_val = slope * (rhs.getEndpointA().getX() - x) + y;

    return y_val >= y_min && y_val <= y_max;

  } else {
    cout << "RHS is not a horizontal or vertical line" << endl;
    return false;
  }

}

// Get endpoints
Point Line::getEndpointA() const {
  return a;
}

Point Line::getEndpointB() const {
  return b;
}

// Get the points in the line
Point* Line::getPoints() const {
  // Calculate change in x and y of line
  int deltaX = b.getX() - a.getX();
  int deltaY = b.getY() - a.getY();

  int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
  if (deltaX < 0) dx1 = -1; else if (deltaX > 0) dx1 = 1;
  if (deltaY < 0) dy1 = -1; else if (deltaY > 0) dy1 = 1;

  // Determine the greater change between deltaX and deltaY
  int longerSide = abs(deltaX);
  int shorterSide = abs(deltaY);

  if (longerSide > shorterSide) {
  	dx2 = dx1;
  } else {
    int temp = shorterSide;
    shorterSide = longerSide;
    longerSide = temp;
    dy2 = dy1;
  }

  // Initialize array of points
  Point* points = new Point[numPoints];

  // Bresenham algorithm to find all points in a line
  int x = a.getX();
  int y = a.getY();
  int numerator = longerSide >> 1;
  for (int i = 0; i <= longerSide; i++) {
    points[i] = Point(x,y);
    numerator += shorterSide;
    if (numerator >= longerSide) {
      numerator -= longerSide;
      x += dx1;
      y += dy1;
    } else {
      x += dx2;
      y += dy2;
    }
  }
  return points;
}

// Get the number of points in the line
int Line::getNumPoints() const {
  return numPoints;
}

// Get whether line is horizontal or vertical
bool Line::isHorizontal() const {
  return isHoriz;
}

bool Line::isVertical() const {
  return isVerti;
}

// Print the line
void Line::Print() const {
  cout << "(" << a.getX() << "," << a.getY() << ")" << "-";
  cout << "(" << b.getX() << "," << b.getY() << ")" << endl;
}
