/*
	Stack.cc
	This file contains all of the function definitions for the Stack class
	This implementation uses a HaverfordCS::list as a stack
	

*/

/*--Includes*/

#include "Stack.h"
using HaverfordCS::list; // allow use of HaverfordCS::list as just 'list'
#include <string>
using namespace std;  // allow all included parts of std, e.g. strings

Stack::Stack() // Basic stack constructor which initializes the stack as an empty HaverfordCS list
{
	repr = list<int>();
}

int Stack::size()
{
	return elementcount;
}

int Stack::top()
{
	if (elementcount > 0) { // Why doesn't (repr != list<int>()) work?s
		return head(repr);
	} else {
		throw "illegal call to Stack::top on empty stack";
	}
}

void Stack::push(int value)
{
	elementcount += 1;
	repr = list<int>(value, repr);
}

int Stack::pop()
{
	if (elementcount > 0) {
		elementcount -= 1;
		// maybe use a map to create a new list without the head
		int topvalue = head(repr);
		repr = rest(repr);
		return topvalue;
	} else {
		throw "illegal call to Stack::pop on empty stack";
	}
}

list<int> Stack::print() // creates a new reversed repr list so that the top of the stack is printed last
{
	list<int> repr_copy = repr;
	list<int> reversed_repr;
	while (not empty(repr_copy))
	{
		reversed_repr = list<int>(head(repr_copy), reversed_repr);
		repr_copy = rest(repr_copy);
	}
	return reversed_repr;
}
