//
// ECE2036 Program 2 - Complex Number Class decaration
// Kairi Kozuma
//

// Class declaration for Complex number class

#include <iostream>

using namespace std;

class Complex {
public:
  // Constructors
  	Complex();
  	Complex(double r, double i);
  	Complex(const std::string& s);
  // Member Functions
  	// Getters
 	double getReal() const;
  	double getImag() const;
  	// Setters
  	void setReal(double r);
  	void setImag(double i);
  	// Other methods
  	double magnitude() const;
  	double angle() const;
  	bool isValid() const;
  	Complex conjugate() const;
  	friend ostream& operator<<(ostream&, const Complex&);
  	void print();
  	// Overloaded Methods
  	Complex operator+(const Complex& rhs) const;
  	Complex operator-(const Complex& rhs) const;
private:
  // Data Members
  	double real;
  	double imag;
};

// Non-member function operator overloading
Complex operator*(const Complex& lhs, const Complex& rhs);
Complex operator/(const Complex& lhs, const Complex& rhs);
