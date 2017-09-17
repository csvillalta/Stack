/*
	main.cc
	This file contains the main function for the Stack class and text interface.
*/

/*-----------Includes---------*/
#include "listDemos.h"
#include "listPractice.h"
#include "Stack.h"
#include "testStack.h"
#include "Calculator.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main ()
{
	cout << "***** First, some list demos:" << endl;
	hcListDemos();
	stdListDemo();
	cout << endl << endl;  // space between tests

	cout << "***** Now, the list practice, as described in the pre-lab" << endl;
	if (!testListPractice()) cout << "oops, it returns false" << endl;
	cout << endl << endl;  // space between tests
	

	cout << "***** Finally, the stack program (you'll need to write this, too):" << endl;
	testStack();
	runCalculator();
	return 0;
}
