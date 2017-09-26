/*
	Stack header file
*/

#ifndef __STACK_H_INCLUDED__  
#define __STACK_H_INCLUDED__   

#include <hc_list.h>

using namespace HaverfordCS;

class Stack {
public:
	// put method _declarations_ here, please, and code in Stack.cc
	Stack(); // The Stack class constructor
	int size();
	int top();
	void push(int value); // A mutator returns no value; Essentially our main constructor
	int pop(); // Returns the value at the top of the stack and removes it from the stack
	list<int> print();

private:
	// put data field(s) here
	list<int> repr; // This is the representation of our stack: a HaverfordCS list
	int elementcount = 0; // This will hold the size of our list and will be accessible by the size method in constant time!
};

#endif 
