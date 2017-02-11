// 9/3
#include <iostream>
#include <iomanip>

using namespace std;

// Invalid declaration, cannot compile
// int a[];

// Valid declaration
// Global variables initalized to 0
int a[5];
int b[] = {0, 1, 2, 3};

int main() {
	// sizeof(): number of bytes used by array
	// need length: sizeof(array)/sizeof(array[0]) or sizeof(array)/sizeof(int)
	for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++) {
		cout << "a[" << i << "] is " << a[i] << endl;
	}
	
	for (int i = 0; i < sizeof(b)/sizeof(int); i++) {
		cout << "b[" << i << "] is " << b[i] << endl;
	}
}