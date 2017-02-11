#include <iostream>
#include <string>

using namespace std;

int main() {
	// Class strings can be assigned
	string s = "abc123";
	
	// string constructor that achieves same effect
	// Better code because constructor used
	string t("abc123456");
	
	cout << " s is " << s << " length is " << s.length() << endl;
	cout << " t is " << t << " length is " << t.length() << endl;

	// Concatenate strings
	string t2 = t + string("789"); // anonymous temporary object
	cout << " t2 is " << t2 << " length is " << t2.length() << endl;
	
	// Change string
	t2[0] = 'X';
	cout << " t2 is " << t2 << " length is " << t2.length() << endl;
	
	// Make string bigger
	t2 += string("b");
	cout << " t2 is " << t2 << " length is " << t2.length() << endl;

	string t3 = t2.substr(0,3);
	cout << " t3 is " << t3 << " length is " << t3.length() << endl;

}
