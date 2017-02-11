#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int intValue;
    float floatValue;
       
    cout<<"Intput an integer:";
    cin>>intValue;
    
    cout<<"The value that you entered in hex is:"<<hex<<intValue<<endl;
    cout<<dec; // Revert back to decimal format
    
    cout<<"The value in decimal is:"<<intValue<<endl;
    
    cout<<"Input a float number:";
    cin>>floatValue;
    cout<<setprecision(2);
    cout<<scientific<<"The float value with scientific:"<<floatValue<<endl;
    cout<<fixed<<"The float value with fixed is:"<<floatValue<<endl;
    
    return 0;
}
