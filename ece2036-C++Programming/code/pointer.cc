// 9/15
// Pointer Code

#include <iostream>
using namespace std;

// Preprocessor defines
// Whenever ASIZE is used, susbtitute with 8
#define ASIZE 8

// Initialize array a
int a[ASIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

// Initialize array b
int b[ASIZE] = {10, 20, 30, 40, 50, 60, 70, 80};

int c = 100;
int d = 200;

int main() {
    int* pA = a; // pA is a pointer, pointing to array "a", element 0
    cout << "pA is " << pA << " *pA is " << *pA
        << " pA[0] is " << pA[0] << endl;

    // Pointer increment, increments by size element
    // // ++ has precedence over *
    // Postfix, j0 is A[0]
    // pA is incremented by 1 (4 bytes)
    int j0 = *pA++;
    // Postfix, j1 is A[1]
    // pA is incremented by 1 (4 bytes)
    int j1 = *pA++;
    cout << "j0 is " << j0 << endl <<  "j1 is " << j1 << endl;

    // Prefix, j2 = A[3], because it is incremented before
    int j2 = *++pA;
    cout << "j2 is " << j2 << endl;

    // Increment the value of A[3] from 3 to 4
    // j3 becomes 3;
    // pA remains unchanged, still points to A[3]
    int j3 = (*pA)++;

    // pA remains unchanged, still points to A[3]
    // j4 is 4
    int j4 = (*pA)++;
    cout << "j3 is " << j3 << endl << "j4 is " << j4 << endl;

    // pA remains unchaged, still points to A[3]
    // j5 is 6, which is the value of A[3]
    int j5 = ++(*pA);
    cout << "j5 is " << j5 << endl;

    // Reset pA to beginning of array A
    pA = a; // or pA = &a[0]
    int* pB = b; // pB points to array b
    cout << "&c " << &c << " &d " << &d <<
     " pB " << pB << " pA " << pA << endl;

    for (int i = 0; i < ASIZE; ++i) {
        // copy a to b
        // dereference has less overhead than array access
        *pB++ = *pA++;
        // Print out b
        for (int i = 0; i < ASIZE; ++i) {
            cout << b[i] << endl;
        }
		cout << "Iteration " << i << endl;
    }
    cout << endl;
    // Print out one after array pB
	  // Most likely C, but do not know
    cout << "*pB is " << *pB << endl;

    pA = a; // reset pA
    cout << " first " << *pA++ << " second " << *pA++
         << " third " << *pA++ << " fourth " << *pA++ << endl;
	// *pA++ evaluated right to left
	// Incremented first at last *pA++ command, then at third, and so one
	// prints A[3] after first, A[2] after second, A[1] after third, A[0] after fourth
  // Behavior observed on linux machine
  // WARNING: May print A[0], A[1], A[2], A[3] depending on system!!! (Mac OSX)
}
