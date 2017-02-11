// 9/1
#include <iostream>
#include <iomanip>

using namespace std;
int main() {
	// While loop
	// Curly braces necessary if more than one line in the body
	
	int i = 10;
	while(i > 0) {
		cout << "i is " << i << endl;
		i = i - 1;
	}
	
	// While loop that does not execute once
	while(i > 0) {
		cout << "This will not print out" << endl;
	}
	
	// For loop
	for (int k = 0; k < 10; k++) {
		cout << "k is " << k << endl;
	}
		
	// Next line cannot compile because k is limited to for loop
	// cout << "k is " << k << endl;
	
	int j = 0;
	for (; j < 10; j++) {
		cout << "j is " << j << endl;
		int l = j;
	}
	// j declared outside loop, can be accessed
	cout << "j is " << j << endl;
	
	// Next line cannot compile because l is limited to for loop 
	// cout << "l is " << l << endl;
	
	// Nested for loops
	for(int m = 0; m < 10; m++) {
		for(int n = 0; n < 10; n++) {
			if (n > 8) {
				// Breaks out of the inner most loop
				// Outer most loop continues
				break;
			}
			
			if (n == 3) {
				// Skips to end of inner most loop, but continue iteration
				continue;
			}
			cout << "m: " << m << " n: " << n << endl;
		}
	}
	
	// Do while loop
	// Executes at least once
	int a = 4;
	do {
		a = a++;
		cout << "a: " << a << endl;
	} while (a > 10);
	// Even though condition is false, one iteration occurs
	int j2 = 0;
	for (int j1 = 0; j1 < 10; j1++) {
		// Shorthand operator
		j2 += j1;
		cout << "j2: " << j2 << endl;
	}
	
	// Prefix and postfix operator
	// Prefix: changes value before assignment
	// Postfix: changes value after assignment
	int x = 0;
	int y = x++;
	x = 0;
	int z = ++x;
	cout << "x: " << x << " y: " << y << " z: " << z << endl;
	
	// Logical operators
	int c = 4;
	int d = 6;
	if (c > 0 && c < 10) {
		cout << "c is greater than 0 and less than 10" << endl;
	}
	
	if (d > 1000 || d < 10) {
		cout << "d is greater than 1000 or less than 10" << endl;
	}
	
	// Operator precedence
	// () before && before ||
	if (d > 1000 && d < 10 || d == 6) {
		cout << "d meets the conditions" << endl;
		// Checks the left hand of the || first
	}
	
	int r = 0x1234; // Hexadecimal
	cout << hex << r << endl;
	// >> shift operator
	// & bitwise logical and
	// | bitwise logical or
	cout << ((r & 0xf000) >> 12) << " ";
	cout << ((r & 0x0f00) >> 8) << " ";
	cout << ((r & 0x00f0) >> 4) << " ";
	cout << ((r & 0x000f)) << endl;
	
	// if statement is true if non zero
	// zero is false
	int w = 0;
	if(!w) {
		cout << "This will print because !w (w is 0)" << endl;
	}
	
	if(1) {
		cout << "This will print" << endl;
	}
	
	if(0) {
		cout << "This will not print" << endl;
	}
	
	// else if
	if(w == 1) {
		cout << "w is 1" << endl;
	} else if (w == 2) {
		cout << "w is 2" << endl;
	} else {
		cout << "w is not 1 or 2" << endl;
	}
}
