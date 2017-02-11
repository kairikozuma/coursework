#include <iostream>
using namespace std;

class A {
public:
	A();
	A(int);
	A(const A&);
	~A();
	A operator+(A rhs) const;
public:
	int x;
};

class B {
public:
	B();
	B(int);
	B(const B&);
	~B();
	B operator+(const B& rhs) const;
public:
	int x;
};

A::A(int a0) : x(a0) {
	cout << "A int constructor" << endl;
}

A::A(const A& rhs) : x(rhs.x){
	cout << "A copy constructor" << endl;
}

A::~A() {
	cout << "A destructor" << endl;
}

A A::operator+(A rhs) const {
	return A(x + rhs.x);
}

B::B(int b0) : x(b0) {
	cout << "B int constructor" << endl;
}

B::B(const B& rhs) : x(rhs.x){
	cout << "B copy constructor" << endl;
}

B::~B() {
	cout << "B destructor" << endl;
}

B B::operator+(const B& rhs) const {
	B r(x + rhs.x);
	return r;
}

int main() {
	A a(1);
	B b(2);
	
	b = b + b;
	a = a + a;
}