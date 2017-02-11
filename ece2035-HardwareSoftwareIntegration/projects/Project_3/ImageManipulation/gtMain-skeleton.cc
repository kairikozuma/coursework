// Main program for gtDisplay graphical interface
// George F. Riley, Georgia Tech, Spring 2015

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "gtDisplay.h"

                   
int main(int argc, char** argv)
{
  int testNum = 1;
  if (argc > 1) testNum = atol(argv[1]);
  std::cout << "Running test number " << testNum << std::endl;
  gtDisplayInit(argc, argv);
  // Your code here  
}

