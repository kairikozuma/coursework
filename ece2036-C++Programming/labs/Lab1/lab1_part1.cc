
#include <iostream>
#include <iomanip>
using namespace std;

/*
int factorial(int x) {
    long y = 1;
    for(x; x > 0; x--) {
        y *= x;
    }
    return y;
}

int combination(int n, int r) {
    return factorial(n) / (factorial(r) * factorial(n - r));
}
*/

int combination(int n, int r) {
	long a = 1;
	long b = 1;
	if(r > n/2) {
		r = n - r;
	}
	for(int i = 0; i < r; i++) {
		a *= (n - i);
	}
	for(int i = 0; i < r; i++) {
		b *= (i + 1);
	}
	return (long) (a/b);
}

int main() {
    int size;
    cout << "Please input the size of the triangle: ";
    cin >> size;
    int width = 7;
    
    for (int i = 0; i <= size; i++) {
        cout << "n = " << i;
        if (i >= 10) {
            cout << ":";
        }
        else {
            cout <<  ": ";
        }
        for (int j = 0; j <= i; j++) {
            cout << setw(width) << combination(i, j);
        }
        cout << endl;
    }
    
}

