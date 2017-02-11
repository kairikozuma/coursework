#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout<<"Hello"<<endl;
    cout<<setw(15)<<"Hello"<<endl;
    cout<<"Hello"<<endl;
    cout<<setw(15)<<"Hello"<<"Hello"<<endl;
    
    cout<<setfill('*');
    cout<<setw(15)<<"Hello"<<endl;
    cout<<setw(30)<<"Hello"<<endl;
    return 0;
}
