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
	cout << testStack.size() << endl;
	cout << "testStack.push(1)" << endl;
	testStack.push(1);
	cout << testStack.size() << endl;
	cout << testStack.pop() << endl;
	cout << testStack.size() << endl;
	testStack.push(1);
	cout << "testStack.push(1)" << endl;
	testStack.push(2);
	cout << "testStack.push(2)" << endl;
	cout << testStack.size() << endl;
	cout << testStack.top() << endl;
	cout << testStack.pop() << endl;
	cout << testStack.size() << endl;
	Stack secondStack;
	cout << secondStack.size() << endl;
	return return_val;
}
