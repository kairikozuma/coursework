//
// ECE2036 Program 2 - Complex Number Class implementation
// Kairi Kozuma
//

#include <iostream>
#include <string>

#include <math.h>

#include "complex.h"

using namespace std;

// Constructors
// Default Constructor with values initialized to zero
Complex::Complex() {
	real = 0;
	imag = 0;
}

// Constructor with initial values as arguments
Complex::Complex(double r, double i) {
	real = r;
	imag = i;
}

// Member functions

// Getters
double Complex::getReal() const {
	return real;
}

double Complex::getImag() const {
	return imag;
}

// Setters
void Complex::setReal(double r) {
	real = r;
}

void Complex::setImag(double i) {
	imag = i;
}

// Other functions
double Complex::magnitude() const {
	return sqrt(real * real + imag * imag);
}

double Complex::angle() const {
	double rad = atan2(imag, real);
	return rad * (180/M_PI);
}

bool Complex::isValid() const {
	return !isnan(real) || !isnan(imag);
}

Complex Complex::conjugate() const {
	return Complex(real, -imag);
}


ostream& operator<<(ostream& os, const Complex& rhs) {
  if (rhs.getImag() == 0) {
  	os << rhs.getReal();
  } else if (rhs.getReal() == 0 && rhs.getImag() != 0) {
    os << rhs.getImag() << "j";
  } else {
    os << rhs.getReal() << " + " << rhs.getImag() << "j";  
  }
  return os;
}

void Complex::print() {
	if(!isValid()) {
		cout << "NaN" << endl;
	} else if(imag != 0) {
		cout << "(" << real << "," << imag << ")" << endl;
	} else {
		cout << real << endl;
	}
}
// Operators
Complex Complex::operator+(const Complex& rhs) const {
	if(!isValid() || !rhs.isValid()) {
		return Complex(NAN, NAN);
	} else {
		return Complex((int)(real + rhs.getReal()),(int)(imag + rhs.getImag()));
	}
}

Complex Complex::operator-(const Complex& rhs) const {
	return Complex(real - rhs.getReal(), imag - rhs.getImag());
}

// Operators (non member)
Complex operator*(const Complex& lhs, const Complex& rhs) {
	return Complex(lhs.getReal()*rhs.getReal() - lhs.getImag()*rhs.getImag(),
				   lhs.getReal()*rhs.getImag() + lhs.getImag()*rhs.getReal());
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
	if (!lhs.isValid() || !rhs.isValid() || rhs.magnitude() == 0) {
		return Complex(NAN, NAN);
	} else {
		double magSq = rhs.magnitude() * rhs.magnitude();
		Complex lrconj = lhs * rhs.conjugate();
		return Complex(lrconj.getReal() / magSq, lrconj.getImag() / magSq);
	}
}
