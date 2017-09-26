/*
	testStack.cc
	This file contains all of the function definitions for the Stack_test.
	These will be used to test the functionality of the Stack class methods.
	
*/

#include "testStack.h"

using namespace std;
// If we use an imperative 'push' method, as well as 'add' to add top 2 elements and peek to return top element,
//  we might test it like this:
/*
bool test_add()
{
	Stack tester = Stack();
	tester.push(5);
	tester.push(6);
	tester.add();
	return tester.top() == 11;
}
*/


bool testStack() // probably this should be made more difficult...
{
	bool return_val = true; // or bool return_val = test_add() && ...
	Stack testStack;
	cout << "Stack testStack;" << endl;
	cout << "testStack.size()\n" << testStack.size() << endl;
	testStack.push(1);
	cout << "testStack.push(1)\n" << endl;
	cout << "testStack.size()\n" << testStack.size() << endl;
	cout << "testStack.pop()\n" << testStack.pop() << endl;
	cout << "testStack.size()\n" << testStack.size() << endl;
	testStack.push(10);
	cout << "testStack.push(10)\n" << endl;
	testStack.push(20);
	cout << "testStack.push(20)\n" << endl;
	cout << "testStack.size()\n" << testStack.size() << endl;
	cout << "testStack.top()\n" << testStack.top() << endl;
	cout << "testStack.pop()\n" << testStack.pop() << endl;
	cout << "testStack.size()\n" << testStack.size() << endl;
	cout << "testStack.top()\n" << testStack.top() << endl;
	Stack secondStack;
	cout << "Stack secondStack; " << endl;
	cout << "secondStack.size()\n" << secondStack.size() << endl;
	return return_val;
}
