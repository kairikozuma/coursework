// 9/17
// Subclassing with virtual functions

#include <iostream>

using namespace std;

class TwoInt {
	public:
		TwoInt(int a0, int b0);	
		virtual void Print() const;
		int First() const;
		int Second() const;
		virtual int Sum() const;
		virtual double Average() const;
		void Hello() const;
	private:
		int a;
		int b;
};

// Define class FourInt that subclasses TwoInt
// Inherits member functions and variables
// Does not have access to private variables of TwoInt
class FourInt: public TwoInt {
	public:
		FourInt(int a0, int b0, int c0, int d0);
		virtual void Print() const;
		int Third() const;
		int Fourth() const;
		virtual int Sm() const;
		virtual double Average() const;
		void Hello() const;
	private:
		int c;
		int d;
};



