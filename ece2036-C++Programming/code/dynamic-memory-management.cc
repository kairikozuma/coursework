// Dynamic memory management

#include <iostream>

using namespace std;

class Vector {
  public:
	// No default constructor
	Vector(int nElements);
	Vector(const Vector&);

	// Assignment operator
	Vector& operator=(const Vector&);

	// Destructor
	~Vector();

	// Return the size of the array
	int getLength() const;

	// Getters
	int getElement(int whichElement) const;

	// Setters
	void setElement(int whichElement, int newValue);

	// Indexing operator
	// returns reference to int, allow V1[2] = 5;
	int& operator[](int whichElement);

	// Print for debugging
	void print() const;

  // private: not accessible outside class
  private:
	int length;
	int* pArray;	// dynamic memory pointer to actual array
};

Vector::Vector(int nElements) {
	cout << "Hello from Vector::Vector(int nElements)" << endl;
	length = nElements;
	pArray = new int[length];
}

// Copy constructor
Vector::Vector(const Vector& v) {
	cout << "Hello from Vector::Vector(const Vector& v)" << endl;
	length = v.getLength();

	// Allocate new memory
	pArray = new int[length];

	// Copy contents
	for (int i = 0; i < length; i++) {
		pArray[i] = v.getElement(i);
	}
}

// Destructor
Vector::~Vector() {
	cout << "Hello from Vector:~Vector() destructor" << endl;
	delete [] pArray; // Free memory
}

// Assignment operator
Vector& Vector::operator=(const Vector& rhs) {
	cout << "Hello from Vector::operator=(const Vector& rhs) assignment" << endl;

	// Protect against self-assignment
	if (&rhs == this) {
		return *this;
	}

	// Give memory back
	delete [] pArray;

	// Allocate memory and copy
	length = rhs.getLength();
	pArray = new int[length];

	for (int i = 0; i < length; i++) {
		pArray[i] = rhs.getElement(i);
	}

    return *this;
}

int Vector::getLength() const {
	return length;
}

int Vector::getElement(int whichElement) const {
	// Can check here if whichElement is valid
	return pArray[whichElement];
}

void Vector::setElement(int whichElement, int newValue) {
	pArray[whichElement] = newValue;
}

// Indexing operator
int& Vector::operator[] (int whichElement) {
	// return reference to specified element
	// allows indexing operator to be used on left and right side
	return pArray[whichElement];
}

void Vector::print() const {
	for (int i = 0; i < getLength(); i++) {
		cout << "Element " << i << " = " << getElement(i) << endl;
	}
	cout << endl; // Space out prints
}

int main() {
	// Initialize v1 with 5 elements
	Vector v1(5);
	for (int i = 0; i < v1.getLength(); i++) {
		v1.setElement(i, i);
	}

	Vector v2(v1);

	cout << "Printing v1" << endl;
	v1.print();

	cout << "Printing v2" << endl;
	v2.print();

	// Vector with 10 elements
	Vector v3(10);
	for (int i = 0; i < v3.getLength(); i++) {
		v3.setElement(i, i * 10);
	}

	// Assignment operator
	v2 = v3;

	cout << "Printing v3" << endl;
	v3.print();

	cout << "Printing v2" << endl;
	v2.print();

	// Illustrating indexing operator
	int val1 = v1[4];
	cout << "v1[4] is " << val1 << endl;

	// Set new value with the indexint operator
	v1[4] = 50;
	cout << "v1[4] is " << v1[4] << endl;

	// Illustrate self-assignment
	v1 = v1;

	// Destructor called
}
