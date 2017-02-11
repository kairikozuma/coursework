//
// ECE2036 Program 2 - Matrix Class decaration
// Kairi Kozuma
//

// Class declaration for Matrix class
// ECE3036 Lab 6

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <iostream>
#include "complex.h"

using namespace std;

typedef Complex      Element_t; // Type for each matrix element
typedef unsigned int Index_t;   // Type for row/col sizes

class Matrix {
public:
  //Constructors and destructor
  Matrix();
  Matrix(const Matrix& rhs);
  Matrix(Index_t nr, Index_t nc);
  ~Matrix();

  // Define overloaded operators here, including assignment operator
  Matrix operator=(const Matrix&);
  Matrix operator+(const Matrix&);
  Matrix operator-(const Matrix&);
  Matrix operator*(const Matrix&);
  Matrix operator*(const Complex&);
  friend ostream& operator<<(ostream&, const Matrix&);
  Matrix operator~() const;

  // Requpired member functions here
  Complex& operator()(int, int) const;
  void transpose();
  void printMatrix() const;
  Index_t getRows() const;
  Index_t getCols() const;
  Complex* getElements() const;
  bool isValid() const;

  // Data Members
private:
  Index_t    nRows;
  Index_t    nCols;
  Complex* elements;
  bool       nam;
};

// Non member function
Matrix operator*(const Complex&, Matrix&);

#endif
