//
// ECE2036 Program 2 - Matrix Class implementation
// Kairi Kozuma
//

#include <iostream>
#include <iomanip>
#include <vector>

#include "matrix.h"
#include "string-parse.h"

using namespace std;

// Constructors here
Matrix::Matrix()
  : nRows(0), nCols(0), elements(NULL), nam(true)
{
}

Matrix::Matrix(const Matrix& rhs)
  : nRows(rhs.getRows()), nCols(rhs.getCols()), nam(false)
 {
  int size = nRows * nCols;
  elements = new Complex[size];
  for (int i = 0; i < size; i++) {
    elements[i] = Complex(rhs.getElements()[i]);
  }
}

Matrix::Matrix(Index_t nr, Index_t nc)
  : nRows(nr), nCols(nc), nam(false)
{
  elements = new Complex[nr * nc];
}

// Destructors here
Matrix::~Matrix() {
  delete [] elements;
}

// Operators here (Matrix addition, subtraction, multiplication)
// Define overloaded operators here, including assignment operator
Matrix Matrix::operator=(const Matrix& rhs) {
  nRows = rhs.getRows();
  nCols = rhs.getCols();
  int size = nRows * nCols;
  delete [] elements;
  elements = new Complex[size];
  for (int i = 0; i < size; i++) {
    elements[i] = rhs.getElements()[i];
  }
  nam = false;
  return *this;
}

Matrix Matrix::operator+(const Matrix& rhs) {
  // Check if valid operation
  if (nRows == rhs.getRows() && nCols == rhs.getCols()) {
    Matrix newMatrix(getRows(), getCols());
    int size = nRows * nCols;
    for (int i = 0; i < size; i++) {
      newMatrix.getElements()[i] = elements[i] + rhs.getElements()[i];
    }
    return newMatrix;
  } else {
    return Matrix();
  }
}
Matrix Matrix::operator-(const Matrix& rhs) {
  // Check if valid operation
  if (nRows == rhs.getRows() && nCols == rhs.getCols()) {
    Matrix newMatrix(getRows(), getCols());
    int size = nRows * nCols;
    for (int i = 0; i < size; i++) {
      newMatrix.getElements()[i] = elements[i] - rhs.getElements()[i];
    }
    return newMatrix;
  } else {
    return Matrix();
  }
}

Matrix Matrix::operator*(const Matrix& rhs) {
  if (nCols == rhs.getRows()) { // Check if inner dimensions are equals
    // Construct new matrix with outer dimensions
    Matrix newMatrix(nRows, rhs.getCols());

    // Do matrix multiplication
    int rc = rhs.getCols();
    int rr = rhs.getRows();
    int lr = nRows;
    int lc = nCols;
    for (int i = 0; i < rc; i++) {
      for (int j = 0; j < lr; j++) {
        Complex sum(0,0);
        for (int k = 0; k < lc; k++) {
          sum = sum + elements[j * lc +  k] * rhs.getElements()[k * rr + i];
        }
        newMatrix.getElements()[i * lr + j] = sum;
      }
  }
    return newMatrix;
  } else { // Return not-a-matrix
    return Matrix();
  }
}

Matrix Matrix::operator*(const Complex& rhs) {
  // Construct new matrix of same size
  Matrix newMatrix(nRows, nCols);

  // Multiply each element by complex number
  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols; j++) {
      newMatrix.getElements()[i * nCols + j] = elements[i * nCols + j] * rhs;
    }
  }
  return newMatrix;
}

// Non member function
Matrix operator*(const Complex& lhs, Matrix& rhs) {
  return rhs * lhs;
}

ostream& operator<<(ostream& os, const Matrix& rhs) {
  cout << rhs.isValid() << endl;
  cout << !rhs.isValid() << endl;
  if (!rhs.isValid()) {
  	os << "NaM" << endl;
  	return os;
  }

  int mRows = rhs.getRows();
  int mCols = rhs.getCols();
  Element_t* elements = rhs.getElements();
  for (int i = 0; i < mRows; i++) {
    for (int j = 0; j < mCols; j++) {
      os << setw(10) << elements[i * mCols + j];
    }
    os << endl;
  }
  return os;
}

Matrix Matrix::operator~() const {
  Matrix newMatrix(*this);
  newMatrix.transpose();
  return newMatrix;
}

// Other member functions here
Complex& Matrix::operator()(int row, int col) const {
  return elements[nCols * (row - 1) + (col - 1)];
}

void Matrix::transpose() {
  // Switch the dimensions
  Index_t temp = nRows;
  nRows = nCols;
  nCols = temp;

  // Tranpose
  // TODO: implement
}

void Matrix::printMatrix() const {
  cout << *this;
}

// Getters
Index_t Matrix::getRows() const {
  return nRows;
}

Index_t Matrix::getCols() const {
  return nCols;
}

Element_t* Matrix::getElements() const {
  return elements;
}

bool Matrix::isValid() const {
  return !nam;
}
