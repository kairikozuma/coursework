// Virtual methods
// 9/16

#include <iostream>

using namespace std;

// Virtual Functions
// - If base class declares a function is virtual,
//	all subclasses have the same function as virtual
//  without explicit declaration
//  (but good practice to declare as virtual)
class TwoInt {
	public:
		TwoInt(int a0, int b0);
		// Virtual functions
		virtual void Print() const;
		int First() const;
		int Second() const;
		virtual int Sum() const;
		virtual double Average() const;
		// Non virtual functions
		void Hello() const;
	// Private keyword, cannot access outside class
	private:
		int a;
		int b;
};

// Class FourInt that inherits from TwoInt
class FourInt : public TwoInt {
	public:
		FourInt(int a0, int b0, int c0, int d0);
		virtual void Print() const;
		int Third() const;
		int Fourth() const;
		virtual int Sum() const;
		virtual double Average() const;
		void Hello() const;
	private:
		int c;
		int d;
};

// Methods for TwoInt
TwoInt::TwoInt(int a0, int b0)
	: a(a0), b(b0)	// Constructor for integers
{
	// Nothing else needed
}

// Print the variable
void TwoInt::Print() const {
	cout << "a " << a << ", b " << b << endl;
}

// Getters
int TwoInt::First() const {
	return a;
}

int TwoInt::Second() const {
	return b;
}

int TwoInt::Sum() const {
	cout << "Hello from TwoInt::Sum()" << endl;
	return a + b;
}

double TwoInt::Average() const {
	cout << "Hello from TwoInt::Average()" << endl;
	return Sum() / 2.0;
}

void TwoInt::Hello() const {
	cout << "Hello from TwoInt::Hello" << endl;
}

// Methods for FourInt
FourInt::FourInt(int a0, int b0, int c0, int d0)
	: TwoInt(a0, b0), c(c0), d(d0)
{
	// Nothing else needed
}

void FourInt::Print() const {
	TwoInt::Print();
	cout << "c " << c << ", d " << d << endl;
}

int FourInt::Third() const {
	return c;
}

int FourInt::Fourth() const {
	return d;
}

int FourInt::Sum() const {
	cout << "Hello from FourInt::Sum()" << endl;
	return TwoInt::Sum() + c + d;
}

double FourInt::Average() const {
	return Sum() / 4.0;
}

void FourInt::Hello() const {
	cout << "Hello from FourInt::Hello" << endl;
}

// If virtual function called, polymorphism allowed (functions of subclass)
// If non virtual function caled, only TwoInt functions allowed (type of argument)
// Can specify TwoInt function to be called with ti.TwoInt::someFunction();
// Virtual functions - extra overhead because evaluated at runtime

void Sub1Ref(const TwoInt& ti) {
	// TwoInt object as parameter
	// Any subclass of TwoInt can be passed
	cout << "Hello from Sub1Ref()" << endl;
	ti.Print();
	cout << "Sub1Ref() calling sum" << endl;
	int s = ti.Sum();
	cout << "Sub1Ref() calling average" << endl;
	double avg = ti.Average();
	cout << "Sum is " << s << " average " << avg << endl;
	ti.Hello();
}

void Sub1Ptr(TwoInt* ti) {
	// TwoInt objet pointer as paramter
	// Any pointer to subclass of TwoInt can be classed
	cout << "Hello from Sub1Ptr()" << endl;
	ti->Print(); // Pointer must be dereferenced
	cout << "Sub1Ptr() calling sum" << endl;
	int s = ti->Sum();
	cout << "Sub1Ptr() calling average" << endl;
	double avg = ti->Average();
	cout << "Sum is " << s << " average " << avg << endl;
	ti->Hello();
}

// If FourInt passed, only the TwoInt part passed (a and b)
// Values of a and b part of FourInt put on the stack
void Sub1Value(TwoInt ti) {
	// Sub1Value expects TwoInt BY VALUE
	cout << "Hello from Sub1Value()" << endl;
	ti.Print();
	cout << "Sub1Value() calling sum" << endl;
	int s = ti.Sum();
	cout << "Sub1Value() calling average" << endl;
	double avg = ti.Average();
	cout << "Sum is " << s << " average " << avg << endl;
	ti.Hello();
}

int main() {
	TwoInt ti1(1,2);
	TwoInt ti2(2,4);
	FourInt fi1(10, 11, 12, 13);
	FourInt fi2(fi1);
	// Call Sub1 variants by passing TwoInt objects
	// by reference, pointer, value
	cout << "Calling Sub1Ref() passing TwoInt" << endl;
	Sub1Ref(ti1);
	cout << "Calling Sub1Ptr() passing TwoInt" << endl;
	Sub1Ptr(&ti2);
	cout << "Calling Sub1Value() passing TwoInt" << endl;
	Sub1Value(ti1);

	// Pass subclass of TwoInt
	cout << "Calling Sub1Ref() passing FourInt" << endl;
	Sub1Ref(fi1);
	cout << "calling Sub1Ptr() passing FourInt" << endl;
	Sub1Ptr(&fi2);
	cout << "calling Sub1Value() passing FourInt" << endl;
	Sub1Value(fi1);

	cout << "Calling Hello on TwoInt and FourInt" << endl;
	ti1.Hello();
	fi1.Hello();
}
