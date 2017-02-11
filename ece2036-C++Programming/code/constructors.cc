// 9/8

#include <iostream>

using namespace std;

// Destructor: called when variable goes out of scope

class A {
	public:
	// 3 Constructors
		// Default constructor
		A();
		A(int a0);
		// Copy constructor
		// Generated automatically by compiler
		A(const A& a0);

	// Destructor: uses tilda
	// Called automatically when out of scope
		~A();

	public:
	// Member variable
		int a;
};

// Default onstructor
A::A()
	: a(0) // Constructor for a to 0
{
	cout << "Default Constructor, a = " << a << endl;
}

// Int constructor
A::A(int a0)
	: a(a0)
{
	cout << "Int constructor, a = " << a << endl;
}

// Copy constructor
// Int constructor
A::A(const A& a0)
	: a(a0.a)
{
	cout << "Copy constructor, a = " << a << endl;
}

// Default destructor calls destructor for all member variables
// Does some operation if memory was allocated to the heap

// Destructor
A::~A()
{
	cout << "Destructor, a = " << a << endl;
}

// Global variables
A ga1;
A ga2(2);
A ga3(ga2);
// Global variables constructed before main

void sub1() {
	cout << "Entering sub1" << endl;
	A suba1;
	A suba2(2);
	A suba3(suba2);
	cout << "Exiting sub1" << endl;
}
// Destructors called in reverse order of constructed objects

// Subroutine with object A passed by value
// Copy constructor called for parameter if passed by value
void sub2(A a)
{
	cout << "Entering sub2" << endl;
	A sub2a(a);
	cout << "Exiting sub2" << endl;
}
// Destuctor called for a and sub2a (2 calls)

// Subroutine with object A passed by pointer
void sub3(A* a) // a is a pointer to object of class A
{
	cout << "Entering sub3" << endl;
	a -> a = 10; // Dereference and set original object's
				// member variable a to 10
	A sub3a(3);
	cout << "Exiting sub3" << endl;
}
// Destructor called for only sub3a (because a passed by reference)

// Subroutine with object A passed by reference
void sub4(A& a) // a is address
{
	cout << "Entering sub4" << endl;
	a.a = 20; // Alters original object
	A sub4a(a);
	cout << "Exiting sub4" << endl;
}

int main() {
	cout << "Entering main" << endl;
	A a0;
	A a1(a0);
	A a2(2);
	A a3(3);
	cout << "a3.a is " << a3.a << endl;
	A a4 = a2; // Copy constructor called
	cout << "Calling sub1" << endl;
	sub1();
	cout << "Back from sub1" << endl;
	cout << "Calling sub2" << endl;
	sub2(a3);
	cout << "Back from sub2" << endl;
	cout << "Calling sub3" << endl;
	sub3(&a3); // Argument is address (&)
	cout << "Back from sub3" << endl;
	cout << "a3.a is " << a3.a << endl;
	cout << "Calling sub4" << endl;
	sub4(a3); // Argument is address (&)
	cout << "Back from sub4" << endl;
	cout << "a3.a is " << a3.a << endl;
	cout << "Exiting main" << endl;

	// Compiler automatically tries to match constructor
	//	with correct type passed as argument
}
