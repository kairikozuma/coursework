// 9/8

#include <iostream>

using namespace std;

class B {
	public:
	// Constructors
		B(); 
		B(int b0);
		B(const B& b0);
		
	// Destructor
		~B();
	
	// Methods
		void print(); // Cannot be called from non const object
		void printC() const; // Can be called from const object
		// const is promise that it will not alter the object
	
	public:
	// Member variable
		int x;
};

// Constructors
B::B()
	: x(0) 
{}

B::B(int x0) 
	: x(x0) 
{}
	
B::B(const B& b) 
	: x(b.x) 
{}
	
// Destructor
B::~B()
{
	cout << "Destructor for B, x = " << x << endl;
}

// Methods
void B::print() {
	cout << "Hello from print()" << endl;
}

void B::printC() const {
	cout << "Hello from printC()" << endl;
}

// Copies by value
void sub1(B b) {
	b.print();
	b.printC();
}

// All others copy by reference, but difference in const
void sub2(B& b) {
	b.print();
	b.printC();
}

void sub3(const B& b) {
	// Cannot call non const methods from const object (Compile error)
	// b.print();
	b.printC();
}

void sub4(B* b) {
	// Dereference to call methods and get variables
	b -> print();
	b -> printC();
}

void sub5(const B* b) {
	// Cannot call non const methods from const object (Compile error)
	// b -> print();
	b -> printC();
}

int main() {
	B b0;
	B b1(2);
	B b3(b1);
	b0.print();
	sub1(b1);
	sub2(b1);
	sub3(b1);
	sub4(&b1);
	sub5(&b1);
}