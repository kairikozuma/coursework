// Parallel Numerical Integration
// ECE2036 Assignment 7, Fall 2015
// Kairi Kozuma

#include <stdio.h>
#include <math.h>
#include "gthread.h"

using namespace std;

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
double correctTwo = 1.8850380210460e+11;

// The below global variable is used by the threads to update the
// total area of the integral.  You will need mutex protection before
// updating this variable.
MyFloat_t totalArea;

// Evaluate f(x) = 47.125x^4 - 23.864x^3 + 5.7x^2 - 173.1x + 10.523
MyFloat_t evaluate(MyFloat_t x) {
  const MyFloat_t coeffFour = 47.125;
  const MyFloat_t coeffThree = -23.864;
  const MyFloat_t coeffTwo = 5.7;
  const MyFloat_t coeffOne = -173.1;
  const MyFloat_t coeffZero = 10.523;
  return coeffFour  * x * x * x * x
       + coeffThree * x * x * x
       + coeffTwo   * x * x
       + coeffOne   * x
       + coeffZero;
}

// Calculate midpoint riemann sum from start to end, with desginated step size
void RiemannSum(MyFloat_t start, MyFloat_t end, MyFloat_t step) {
  int iterationLimit = (int)((end - start) / step);
  MyFloat_t sum = 0.0;

  for (int i = 0; i < iterationLimit; i++) {
    sum += evaluate(start + i * step) + evaluate(start + (i + 1) * step);
  }

  sum = sum * (step / 2);

  gthread_mutex_t mutex = NULL;

  LockMutex(mutex);
  totalArea += sum;
  UnlockMutex(mutex);

  EndThread();
}

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

  // Define parameters for RiemannSum
  MyFloat_t stepSize = 1.0;
  MyFloat_t begin = -100.0;
  MyFloat_t end = 100.0;

  int numThreads = 10;
  int numIteration = 7;

  // Calculate the size of each thread interval
  MyFloat_t intervalSize = (end - begin) / numThreads;

  for (int i = 0; i < numIteration; i++) {

  	// Make 10 threads
  	for (int i = 0; i < numThreads; i++) {
  	  CreateThread(RiemannSum, begin + i * intervalSize, begin + (i + 1) * intervalSize, stepSize);
  	}

  	// Wait for all threads to complete
    WaitAllThreads();

    // Print out the result
    double error = fabs(correctTwo - (double)totalArea);
    printf("DeltaX %3.6f, Area %15.13e, error %15.13e\n", stepSize, totalArea, error);

    // Reset the totalArea
    totalArea = 0.0;

  	// Make step size smaller by magnitude 1
    stepSize /= 10;
  }

}
