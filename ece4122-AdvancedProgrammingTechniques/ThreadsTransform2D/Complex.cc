//
// ECE3090 Program 3 - Complex Number Class implementation
// YOUR NAME HERE
// PLATFORM (Windows or Linux)
//

#include <iostream>
#include <string>

#include <math.h>

#include "Complex.h"

using namespace std;

// Constructors
Complex::Complex()
    : real(0), imag(0)
{
}

Complex::Complex(double r)
    : real(r), imag(0)
{
}

Complex::Complex(double r, double i)
    : real(r), imag(i)
{
}

// Operators
Complex Complex::operator+(const Complex& b) const
{
  return Complex(real + b.real, imag + b.imag);
}

Complex Complex::operator-(const Complex& b) const
{
  return Complex(real - b.real, imag - b.imag);
}

Complex Complex::operator*(const Complex& b) const
{
  return Complex(real*b.real - imag*b.imag,
                 real*b.imag + imag*b.real);
}

Complex Complex::operator/(const Complex& b) const
{
  Complex tmp = (*this) * b.Conj();
  Complex magSquared = b.Mag() * b.Mag();
  if (magSquared.Mag().real == 0.0)
    {
      return Complex(); // jus zero in this cae
    }
  return Complex(tmp.real/magSquared.real, tmp.imag/magSquared.real);
}


// Complex Complex::operator/(const Complex& b) const
// {
//   return Complex(real/b.real, imag/ b.real);
//   // THis needs some work
// }

// Member functions
Complex Complex::Mag() const
{
  return Complex(sqrt(real*real + imag*imag));
}

Complex Complex::Angle() const
{
  return Complex(atan2(imag, real) * 360 / (2 * M_PI));
}

Complex Complex::Conj() const
{ // Return to complex conjugate
  return Complex(real, -imag);
}

void Complex::Print() const
{
  double r = real;
  double i = imag;
if (i == 0)
  cout << r;
else
  cout << '(' << r << "," << i << ')';
}

// Global function to output a Complex value
std::ostream& operator << (std::ostream &os, const Complex& c)
{
  if (c.imag == 0) os << c.real ;
  else  
  os << '(' << c.real << "," << c.imag << ')';
  return os;
}
