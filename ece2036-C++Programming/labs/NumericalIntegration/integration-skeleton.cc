// Parallel Numerical Integration
// ECE2893 Assignment 7, Spring 2011
// YOUR NAME HERE

#include <stdio.h>
#include <math.h>
#include "gthread.h"

// The typedef below defines whether we are using 32-bit float
// or 64-bit double for all of the floating point values.  Your program
// should use this type (MyFloat_t) for all variables that use floating
// point.
typedef MYFLOAT  MyFloat_t;

// The below is the "correct" value, determined by evaluating the
// integral and computing the area.  This is used when computing
// the "error" for the numerical integration calculations.
// Also, this is the only place in the program where the type "double"
// is to be used.  Everywhere else, you should use type MyFloat_t.
double correct = 1.8850380185600e+11;

// The below global variable is used by the threads to update the
// total area of the integral.  You will need mutex protection before
// updating this variable.
MyFloat_t totalArea;

int main()
{
  // Print out whether using float or double
  if (sizeof(MyFloat_t) == 4)
    {
      printf("Using float\n");
    }
  else
    {
      printf("Using double\n");
    }
  // Your code here.  You need to loop 7 times with 7 different
  // deltaX values, as described in the assignment.  For each of
  // the 7 iterations, create 10 threads and compute the integral
  // numerically using the Riemann sum metthod.  After all 10 threads
  // have completed, print out the deltaX value, the computed integral
  // and the error.
}



  
