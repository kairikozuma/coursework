#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

bool vl(int num1, int num2) {
  return abs(num1 - num2) < 2; 
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "Need two files" << endl;
    cout << "Found " << argc - 1 << " arguments" << endl;
    return 0;	   
  }

  ifstream infile1(argv[1]);
  ifstream infile2(argv[2]);

  string c;
  int p1, p2; 
  double s1, s2;
  int w1, w2;
  int r1, g1, b1, r2, g2, b2;
  
  int count = 0;
  int numCorrect = 0;  
	
  bool end = false;

  while(!end) {
    if(
      (infile1 >> c >> p1 >> c >> s1 >> c >> w1 >> c >> r1 >> c >> g1 >> c >> b1)
   && (infile2 >> c >> p2 >> c >> s2 >> c >> w2 >> c >> r2 >> c >> g2 >> c >> b2)) {
      if(p1 != p2) {
        cout << "1:" << p1 << " " << s1 << " " << r1 << " " << g1 << " " << b1 << endl;
        cout << "2:" << p2 << " " << s2 << " " << r2 << " " << g2 << " " << b2 << endl;
      } else if(fabs(s1 - s2) > 1e-3) {
        //cout << p1 << " sig ";
      } else if(!(vl(r1,r2) && vl(g1,g2) && vl(b1,b2))) {
        //cout << p1 << " rgb ";
      } else {
        count++;
      }
    } else {
      // cout << p1 << endl;
      end = true;
    }
    count++;
  }
  cout << (double)numCorrect / count << endl;
  return 0;
}
