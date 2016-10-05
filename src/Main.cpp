#include "LineEditor.h"
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	string fileName;
	switch (argc) {
	case 1:		// no file name
		break;
	case 2:		// read from argument string
		fileName = argv[1];
		break;
	default:
		cout << ("too many arguments - all discarded") << endl;
		fileName.clear();
		break;
	}		
	LineEditor ed(fileName);	// create an editor object	
	ed.run();	// run our editor
	return 0;
}