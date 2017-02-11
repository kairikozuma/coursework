#include <iostream>
using namespace std;
class Pascaline
{
public:
 Pascaline();
 //member functions prototypes
 void setRegister1(int);
 void setRegister2(int);
 void setRegisterResult(int);
 int getRegister1();
 int getRegister2();
 int getRegisterResult();
 void clearRegisters();
 void addRegisters();
 void getInputValues();
 void displayOutputValue();
private:
 int register1;
 int register2;
 int registerResult;
//Could be syntax error around here!
}; //end class Pascaline
//-----------------------------------------
//---Implementation of the member functions
//-----------------------------------------
//Insert Constructor implementation here
Pascaline::Pascaline() {
	register1 = 0;
	register2 = 0;
	registerResult = 0;
}
//member function to set register1
void Pascaline::setRegister1( int reg1)
{
	register1 = reg1;
}
//member function to set register2
void Pascaline::setRegister2( int reg2)
{
    register2 = reg2;
}
//member function to set resultRegister
void Pascaline::setRegisterResult( int regResult)
{
 registerResult = regResult;
}
//member function to get the register1 value
int Pascaline::getRegister1()
{
	return register1;
}
//member function to get the register2 value
int Pascaline::getRegister2()
{
 return(register2);
}
//member function to get the resultRegister value
int Pascaline::getRegisterResult()
{
	return registerResult;
}
//Implement clearRegisters() here
//It sets the value of all the registers equal to zero
void Pascaline::clearRegisters() {
	register1 = 0;
	register2 = 0;
	registerResult = 0;
}
//Implement addRegisters() here
//It will add register1 and register2 and put result in registerResult
void Pascaline::addRegisters() {
	registerResult = register1 + register2;
}
//Implement getInputValues() here
//It will get the input data from the user
void Pascaline::getInputValues() {
	int reg1, reg2;
	cout << "Please input the value of register1: ";
	cin >> reg1;
	// cout << endl;
	setRegister1(reg1);
	cout << "Please input the value of register2: ";
	cin >> reg2;
	// cout << endl;
	setRegister2(reg2);
}
//Implement displayOutputValue() here
// It will display the output register and computation to the user
void Pascaline::displayOutputValue() {
	cout << "***Pascaline Result: " << register1 << " + " << register2 << " = " << registerResult << "\n";
}

int main()
{
 //Instantiate your main object here
 Pascaline myPascaline;
 char input;
 //Now call the public member functions to use the object
 do {
 myPascaline.getInputValues();
 myPascaline.addRegisters();
 myPascaline.displayOutputValue();
 // Trying using an escape sequence in the string
 cout << "\nWould you like to calculate again (Y or N)?";
 cin >> input;
 cout << endl;
 } while (input == 'Y');
 return 0;
}
