#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream inFileExample("fileio2.txt", ios::in);
    int x;
    int y;
    bool goodInput;

    if(inFileExample == false) { // bad file
	cout<<"Error in opening file"<<endl;
    }
    else {
        inFileExample>>x>>y;
    }

    for(int j = y; j > 0; j--) {
        for(int i = x; i > 0; i--) {
            cout<<"*";
	}
        cout<<endl;
    }

    cout<<y<<" by "<<x<<endl;
}
