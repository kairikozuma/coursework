#include <iostream>
using namespace std;

class Pascaline {
	public:
		Pascaline();
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
};
// Pascaline Constructor
Pascaline::Pascaline() {
	register1 = 0;
	register2 = 0;
	registerResult = 0;
}

// setters for Pascaline
void Pascaline::setRegister1(int reg1) {
	register1 = reg1;
}
void Pascaline::setRegister2(int reg2) {
    register2 = reg2;
}
void Pascaline::setRegisterResult(int regResult) {
	registerResult = regResult;
}
// getters for Pascaline
int Pascaline::getRegister1() {
	return register1;
}
int Pascaline::getRegister2() {
	return register2;
}
int Pascaline::getRegisterResult() {
	return registerResult;
}
// other member methods
void Pascaline::clearRegisters() {
	register1 = 0;
	register2 = 0;
	registerResult = 0;
}
// Implement addRegisters() here
// It will add register1 and register2 and put result in registerResult
void Pascaline::addRegisters() {
	registerResult = register1 + register2;
}
// Implement getInputValues() here
// It will get the input data from the user
void Pascaline::getInputValues() {
	int reg1, reg2;
	cout << "Please input the value of register1: ";
	cin >> reg1;
	setRegister1(reg1);
	cout << "Please input the value of register2: ";
	cin >> reg2;
	setRegister2(reg2);
}
// Implement displayOutputValue() here
// It will display the output register and computation to the user
void Pascaline::displayOutputValue() {
	cout << "***Pascaline Result: " << register1 << " + " << register2 << " = " << registerResult << "\n";
}

class LeibnizWheel {
	public:
		LeibnizWheel();
		void setRegister1(int);
		void setRegister2(int);
		void setRegisterResult(int);
		int getRegister1();
		int getRegister2();
		int getRegisterResult();
		void clearRegisters();
		void multiplyRegisters();
		void getInputValues();
		void displayOutputValue();
	private:
		int register1;
		int register2;
		int registerResult;
};
// LeibnizWheel Constructor
LeibnizWheel::LeibnizWheel() {
	register1 = 0;
	register2 = 0;
	registerResult = 0;
}

// setters for LeibnizWheel
void LeibnizWheel::setRegister1(int reg1) {
	register1 = reg1;
}
void LeibnizWheel::setRegister2(int reg2) {
    register2 = reg2;
}
void LeibnizWheel::setRegisterResult(int regResult) {
	registerResult = regResult;
}
// getters for LeibnizWheel
int LeibnizWheel::getRegister1() {
	return register1;
}
int LeibnizWheel::getRegister2() {
	return register2;
}
int LeibnizWheel::getRegisterResult() {
	return registerResult;
}
// other member methods
void LeibnizWheel::clearRegisters() {
	register1 = 0;
	register2 = 0;
}
// Implement addRegisters() here
// It will add register1 and register2 and put result in registerResult
void LeibnizWheel::multiplyRegisters() {
	registerResult = register1 * register2;
}
// Implement getInputValues() here
// It will get the input data from the user
void LeibnizWheel::getInputValues() {
	int reg1, reg2;
	cout << "Please input the value of register1: ";
	cin >> reg1;
	setRegister1(reg1);
	cout << "Please input the value of register2: ";
	cin >> reg2;
	setRegister2(reg2);
}
// Implement displayOutputValue() here
// It will display the output register and computation to the user
void LeibnizWheel::displayOutputValue() {
	cout << "***Leibniz Wheel Result: " << register1 << " * " << register2 << " = " << registerResult << "\n";
}

int main() {
	//Instantiate objects
	Pascaline mPascaline;
	LeibnizWheel mLeibnizWheel;
 	char choice;
 	char machine;
 	do {
 		cout << "Would you like to use the Leibniz Wheel(enter 'L') or the Pascaline(enter 'P')?";
 		cin >> machine;
 		if (machine == 'L') {
 			mLeibnizWheel.getInputValues();
 			mLeibnizWheel.multiplyRegisters();
 			mLeibnizWheel.displayOutputValue();
 		} else if (machine == 'P') {
			mPascaline.getInputValues();
			mPascaline.addRegisters();
			mPascaline.displayOutputValue();
 		} else {
 			cout << "I am not sure what you want!" << endl;
 		}
 		cout << "\nWould you like to calculate again (Y or N)?";
 		cin >> choice;
 		cout << endl;
 	} while (choice == 'Y');
 	return 0;
}
