#include <iostream>
using namespace std;
class Base {
	public:
		// Pure virtual function (= 0)
		// Class with pure virtual function cannot be instantiated (abstract)
		// Cannot create Base object

		virtual void Sub1() = 0;
				void Sub2();
		virtual void Sub3();
		virtual void Sub4();
};

// Able to create a pointer Base* x

class A : public Base {
	public:
		// Must implement pure virtual function
		void Sub1();

		// Not virtual, no polymorphism
		// But overriden
		void Sub2();

		// Sub4() is virtual, even if not declared virtual here
		// It is virtual in superclass
		void Sub4();
};

class B : public Base {
	public:
		void Sub1();
		void Sub2();
		void Sub3();
};

// Base Class Methods
// No implementation for Sub1() because it is pure virtual
void Base::Sub2() {
	cout << "Hello from Base::Sub2()" << endl;
}

void Base::Sub3() {
	cout << "Hello from Base::Sub3()" << endl;
	// Calls the polymorphic method of subclasses
	Sub1();
	Sub4();
}

void Base::Sub4() {
	cout << "Hello from Base::Sub4()" << endl;
}

// Class A Methods
// Must implement Sub1() because it is pure virtual
void A::Sub1() {
	cout << "Hello from A::Sub1()" << endl;
}

// Override non virtual method
void A::Sub2() {
	cout << "Hello from A::Sub2()" << endl;
}

// Override virtual method
void A::Sub4() {
	cout << "Hello from A::Sub4()" << endl;
}

// Class B Methods
// Must implement Sub1() because it is pure virtual
void B::Sub1() {
	cout << "Hello from B::Sub1()" << endl;
}

// Override non virtual method
void B::Sub2() {
	cout << "Hello from B::Sub2()" << endl;
}

// Override virtual method
void B::Sub3() {
	cout << "Hello from B::Sub3()" << endl;
}


// Helper subroutine
// Pointers to Base (subclasses)
void SubP(Base* x) {
	x->Sub1();
	// Polymorphism for Sub3(), Sub4() because they are virtual
	// If subclass does not implement a virtual function,
	// function of superclass called
	x->Sub3();
	x->Sub4();
}

// Reference to Base (subclasses)
void SubR(Base& x) {
	// Calls Sub2() of superclass Base, because it is not virtual
	// No polymorphism allowed for Sub2()
	x.Sub2();
	// Polymorphism for Sub3(), Sub4() because they are virtual
	// If subclass does not implement a virtual function,
	// function of superclass called
	x.Sub3();
	x.Sub4();
}

// Copy of B (subclasses)
void SubV(B b) {
	// Calls Sub1() and Sub2() of class B, because it is copied
	b.Sub1();
	b.Sub2();
}

int main() {
	A a;
	B b;

	SubP(&a);
	SubR(b);
	SubV(b);
}
