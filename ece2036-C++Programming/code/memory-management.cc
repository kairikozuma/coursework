#include <stdlib.h>
#include <iostream>

using namespace std;

// Rule of 3
// Need all if need one
// 1. Copy constructor
// 2. Assignment operator
// 3. Destructor

// Run valgrind
// valgrind --tool=mmecheck ./program

class Vector {
	public:
		Vector();
		Vector(size_t);
		// size_t: size of unsigned integer (32 bit or 64 bit)
		//         depends on 32 bit or 64 bit machine
		Vector(const Vector& rhs); // Copy constructor
		~Vector(); // Destructor
		Vector& operator=(const Vector& rhs); // Assignment operator
	public:
		int* p;
		size_t size;
};

// Constructor
Vector::Vector(): p(0), size(0) {
}

Vector::Vector(size_t sz) {
    cout << "Hello from size constructor" << endl;
	p = new int[sz];
	size = sz;
}

Vector::Vector(const Vector& rhs) {
    cout << "Hello from copy constructor" << endl;
	p = new int[rhs.size];
	size = rhs.size;

	for (size_t i = 0; i < size; i++) {
		p[i] = rhs.p[i];
	}

}

// Destructor
Vector::~Vector() {
	cout << "Hello from destructor" << endl;
	delete[] p;
}

// Assignment Operator
// Free memory from the heap if pointers exist that points to it
Vector& Vector::operator=(const Vector& rhs) {
    cout << "Hello from assignment operator" << endl;

    // Check for self assignment
    if (this == &rhs) {
        cout << "Self assignment!" << endl;
        return *this;
    }

	// Free memory of lhs before assigning new one
	delete[] p;
	// Safe to delete a null pointer (p can be 0)

	// Create new pointer
	p = new int[rhs.size];
	size = rhs.size;

	for (size_t i = 0; i < size; i++) {
		p[i] = rhs.p[i];
	}

	// Return Vector
	return *this;
}

int Sub() {
	Vector v1(100);
	// Copy constructor
	Vector v2(200);
	// v2.p points to same array as v1.p
	v1 = v2;
    Vector v3 = v2;
    v1 = v1;
	return 0;
}

// Shallow copy
// - copies pointer, not what it points to

// Deep copy
// - copies the data that the pointer points to

int main() {
	Sub();
}
