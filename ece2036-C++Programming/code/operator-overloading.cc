// 9/3
// Pass by reference (pass address)


class ComplexNumber {
	private:
		double r;
		double i;
	public:
		ComplexNumber();
		ComplexNumber(double r0, double i0);
		
		// Operator overloading
		// &: pass address of b
		// const: prevent variable referenced from being changed by function
		// 			(compile time error when attempting to changed)
		// first const prevents argument values from being changed
		// second const prevents the values of the object that calls method from being changed
		ComplexNumber operator+(const ComplexNumber& rhs) const;
};

Complex Complex:: operator+(const Complex& b) const {
	// implement operator overload of plus
}