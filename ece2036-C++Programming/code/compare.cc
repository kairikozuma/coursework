#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char * * argv) {
	if (argc > 2) {
		ifstream key(argv[1], ios::in);
		ifstream outputAnswer(argv[2],ios::in);
		char input_key;
		char input_answer;
		bool GETOUT = false, KEY_GETOUT =false, ANSWER_GETOUT= false, ERROR = false;
		int characterPosition = 0;
		while (!GETOUT) {
			if (key) //is valid then read in value
			{
				key >> input_key;
				characterPosition++;
			}
			else //file is not there or file is ended
			{
				KEY_GETOUT = true;
				GETOUT = true;
			}
			if (outputAnswer) //is valid then read in value
			{
				outputAnswer >> input_answer;
			}
				else //file is not there or file is ended
			{
				ANSWER_GETOUT = true;
				GETOUT = true;
			}
			if ((input_key != input_answer)||(KEY_GETOUT != ANSWER_GETOUT)) {
			//this also produces an error if files are not of the same length
				cout << "You have an error at character position " << characterPosition;
				cout << ":" << input_answer << " - please try again" << endl;
				ERROR = true; GETOUT = true;
			}
		}
	if (!ERROR) { cout << "Good job! You have perfect output!" << endl; }
	key.close();
	outputAnswer.close();
	}
	else {
		cout << "You must input two filenames to compare!" << endl;
	}
}//end main