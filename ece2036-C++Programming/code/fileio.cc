#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream outputFileExample("output2-3.txt", ios::out);

    int number = 10;
    int k = 0;
	
    while (k < number) {
        outputFileExample<<"Hello!"<<endl;
	k++;
    }
    
    outputFileExample.close();
}
