/*
	StackInterface.cc
	This file contains all of the function definitions for the StackInterface.
	These will be used to generate the text interface for the stack

	Functions:
	Title			  		Description
	------------------------------------------
*/

/*----------Includes---------*/
#include <hc_list.h>
using HaverfordCS::list;	// like using namespace HaverfordCS, but just for one name
#include "Stack.h"
#include "Calculator.h"
#include <string>
#include <iostream>
#include <regex>  // check if something is numeric with a "regular expression" (coming up later)
using namespace std;

map<string, function<Stack (Stack)>> mapOfFunctions;
Stack calc_stack;

bool looksLikeInt(string s)
{
	std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
	return std::regex_match(s, intPattern);
}

void addFunctionToDict(string function_name, function<Stack (Stack)> f)
{
	mapOfFunctions[function_name] = f;
}

void functionsToAdd()
{
	addFunctionToDict("+", stack_add);
	addFunctionToDict("-", stack_subtract);
	addFunctionToDict("*", stack_multiply);
	addFunctionToDict(".s", printStackSize);
	addFunctionToDict(".", popAndPrint);
	addFunctionToDict("drop", drop);
	addFunctionToDict("nip", nip);
	addFunctionToDict("dup", dup);
	addFunctionToDict("over", over);
	addFunctionToDict("tuck", tuck);
	addFunctionToDict("swap", swapp);
	addFunctionToDict(">", lessThan);
	addFunctionToDict("<", greaterThan);
	addFunctionToDict("=", equalTo);
}

bool looksLikeFunc(string s) // Referenced https://www.cprogramming.com/tutorial/stl/stlmap.html
{
	if(mapOfFunctions.find(s) == mapOfFunctions.end())
		return false;
	return true;

}

void runCalculator()
{
	string token;
	functionsToAdd();

	cout << "Welcome to Postfix Calculator by Christopher Villalta\nPlease enter some integers." << endl;

	while (cin >> token and token != "bye") {
		if (looksLikeInt(token)) {
			cout << "Thanks for entering the integer " << stoi(token) << endl;
			calc_stack.push(stoi(token));
		} else {
			if (looksLikeFunc(token)) {
				cout << "Applying " << token << " ..." << endl;
				calc_stack = mapOfFunctions[token](calc_stack);
			} else {
				cout << "Not a valid input; please continue." << endl;
			}
		}
	}
}

// Actual stack calculator functions

Stack stack_add(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}

	int b = calculator_stack.pop();
	int a = calculator_stack.pop();
	calculator_stack.push(a+b);
	return calculator_stack;
}

Stack stack_subtract(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int a = calculator_stack.pop();
	int b = calculator_stack.pop();
	calculator_stack.push(b-a);
	return calculator_stack;
}

Stack stack_multiply(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int a = calculator_stack.pop();
	int b = calculator_stack.pop();
	calculator_stack.push(b*a);
	return calculator_stack;
}

Stack popAndPrint(Stack calculator_stack)
{
	if (calculator_stack.size() <= 0) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int topvalue = calculator_stack.pop();
	cout << topvalue << endl;
	return calculator_stack;
}

Stack printStack(Stack calculator_stack)
{
	cout << calculator_stack.print() << endl;
	return calculator_stack;
}

Stack printStackSize(Stack calculator_stack)
{
	cout << "<" << calculator_stack.size() << "> " << calculator_stack.print() << endl;
	return calculator_stack;
}

Stack printCurrentResult(Stack calculator_stack)
{
	cout << calculator_stack.top() << endl;
	return calculator_stack;
}

Stack drop(Stack calculator_stack)
{
	calculator_stack.pop();
	return calculator_stack;
}

Stack nip(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	calculator_stack.pop();
	calculator_stack.push(stack_top);
	return calculator_stack;
}

Stack dup(Stack calculator_stack)
{
	calculator_stack.push(calculator_stack.top());
	return calculator_stack;
}

Stack over(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_second);
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	return calculator_stack;
}

Stack tuck(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	calculator_stack.push(stack_top);
	return calculator_stack;
}

Stack swapp(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	return calculator_stack;
}

Stack greaterThan(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	if (stack_top > stack_second)
	{
		calculator_stack.push(-1);
		return calculator_stack;
	}
	else
	{
		calculator_stack.push(0);
		return calculator_stack;
	}
}

Stack lessThan(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	if (stack_top < stack_second)
	{
		calculator_stack.push(-1);
		return calculator_stack;
	}
	else
	{
		calculator_stack.push(0);
		return calculator_stack;
	}
}

Stack equalTo(Stack calculator_stack)
{
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	if (stack_top == stack_second)
	{
		calculator_stack.push(-1);
		return calculator_stack;
	}
	else
	{
		calculator_stack.push(0);
		return calculator_stack;
	}
}
