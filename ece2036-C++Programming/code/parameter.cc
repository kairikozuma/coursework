#include <iostream>
using namespace std;

// Function arguments passed by value by default: values copied to the stack
// Need to use int& to pass by reference: able to change variable within function

int Sub1(int a) {
	a *= 2;
	return a;
}

int Sub2(int& a) {
	a *= 2;
	return a;
}

int main() {
	int alpha = 10;
	cout << "Number is: " << alpha << endl;
	Sub1(alpha);
	cout << "Number after Sub1 is: " << alpha << endl;
	cout << "Number is: " << alpha << endl;
	Sub2(alpha);
	cout << "Number after Sub2 is: " << alpha << endl;
}
