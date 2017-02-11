#include <iostream>
#include "Point.h"
#include "Line.h"

using namespace std;

int main() {
  cout << "This tests the Line and Point class" << endl;
  
  cout << "Input first point of Line 1: " << endl;
  int x1, y1;
  cin >> x1 >> y1;
 
  cout << "Input second point of Line 1: " << endl;
  int x2, y2;
  cin >> x2 >> y2;

  cout << "Input first point of Line 2: " << endl;
  int x3, y3;
  cin >> x3 >> y3;
 
  cout << "Input second point of Line 2: " << endl;
  int x4, y4;
  cin >> x4 >> y4;
  Line l1 = Line(Point(x1, y1), Point(x2, y2));
  Line l2 = Line(Point(x3, y3), Point(x4, y4));
  l1.Print();
  l2.Print();
  
  cout << "Intersects: " << l1.Intersects(l2) << endl;
  return 0;
}
