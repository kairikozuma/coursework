#include <iostream>
#include <math.h> // C library (need h, which stands for header)
using namespace std;

class ComplexNumber {
	private:
		double r;
		double i;
	public:
		// Constructor: Member function name has the same name as the class
		ComplexNumber(); // Constructor for initialization and other startup things
		ComplexNumber(double r0, double i0); // Method overloading
		// Copy constructor
		// provided automatically by compiler
		// ComplexNumber(mComplexNumber) would compile even if not declared
		ComplexNumber(const ComplexNumber& rhs); // Copy constructor
		double Mag();
		void Print();
		void SetR(double r0);
		void SetI(double i0);
		double GetR();
		double GetI();
		ComplexNumber Add(const ComplexNumber& rhs);
		ComplexNumber operator+(const ComplexNumber& rhs);
}; // Need a semicolon after curly brace for class declaration ******

// Global function that adds two ComplexNumber variables
ComplexNumber Add(ComplexNumber& lhs, ComplexNumber& rhs) {
    cout << "Hello from global function Add" << endl;
	return ComplexNumber(lhs.GetR() + rhs.GetR(), lhs.GetI() + rhs.GetI());
}

// Create global array
// When created, default constructor called for every element
ComplexNumber mComplexArray [100];

// Constructor called automatically when declared
ComplexNumber::ComplexNumber() {
	r = 0;
	i = 0;
	cout << "Inside ComplexNumber constructor" << endl;
}

ComplexNumber::ComplexNumber(double r0, double i0) {
	r = r0;
	i = i0;
	cout << "Inside ComplexNumber constructor overloaded" << endl;
}

ComplexNumber::ComplexNumber(const ComplexNumber& rhs) {
	cout << "Hello from Copy Constructor" << endl;
	r = rhs.r;
	i = rhs.i;
}

// Arithmetic operations
ComplexNumber ComplexNumber::Add(const ComplexNumber& rhs) {
    cout << "Hello from member function Add" << endl;
	ComplexNumber result(r + rhs.r, i + rhs.i);
	return result;
	// return ComplexNumber(r + rhs.r, i + rhs.i);
	// anonymous temporary object
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& rhs) {
    cout << "Hello from member function +" << endl;
	ComplexNumber result(r + rhs.r, i + rhs.i);
	return result;
}

double ComplexNumber::Mag() {
	return sqrt(r * r + i * i);
}

void ComplexNumber::SetR(double r0) {
	r = r0;
}

void ComplexNumber::SetI(double i0) {
	i = i0;
}

double ComplexNumber::GetR() {
	return r;
}

double ComplexNumber::GetI() {
	return i;
}

void ComplexNumber::Print() {
	cout << "R " << r << " I " << i << endl;
}

int main() {
	cout << "Hello from main" << endl;
	ComplexNumber N1;
	ComplexNumber N2(20, 30);
	ComplexNumber N3(40, 50);
    cout << "N1 = N2" << endl;
	N1 = N2; // Operator assignment
    cout << "ComplexNumber N4 = N3" << endl;
	ComplexNumber N4 = N3; // Copy constructor called
    cout << "ComplexNumber N5 = ComplexNumber(N2)" << endl;
	ComplexNumber N5 = ComplexNumber(N2);
	ComplexNumber N6 = N1.Add(N3);
	ComplexNumber N7 = N1 + N3; // Operator overloading
    ComplexNumber N8 = Add(N1, N3);

	// Separate objects in memory
	N1.SetR(10);
	N1.SetI(20);

	cout << "N1: ";
	N1.Print();
	cout << "N2: ";
	N2.Print();
	cout << "N3: ";
	N3.Print();
	cout << "N4 = N3" << endl;
	cout << "N4: ";
	N4.Print();
	cout << "N5 = ComplexNumber(N2)" << endl;
	cout << "N5: ";
	N5.Print();
	cout << "N6 = N1.Add(N3)" << endl;
	cout << "N6: ";
	N6.Print();
	cout << "N7 = N1 + N3 (operator overloading)" << endl;
	cout << "N7: ";
	N7.Print();
	cout << "N8 = Add(N1, N3) global function" << endl;
	cout << "N8: ";
	N8.Print();
}
