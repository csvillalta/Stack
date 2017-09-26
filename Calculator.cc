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
	addFunctionToDict(".s", printStack);
	addFunctionToDict(".", printCurrentResult);
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
				cout << "The function " << token << " has been applied." << endl;
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
	int b = calculator_stack.pop();
	int a = calculator_stack.pop();
	calculator_stack.push(a+b);
	cout << "Result of operation: "<< calculator_stack.top() << endl;
	return calculator_stack;
}

Stack stack_subtract(Stack calculator_stack)
{
	int a = calculator_stack.pop();
	int b = calculator_stack.pop();
	calculator_stack.push(b-a);
	cout << "Result of operation: "<< calculator_stack.top() << endl;
	return calculator_stack;
}

Stack stack_multiply(Stack calculator_stack)
{
	int a = calculator_stack.pop();
	int b = calculator_stack.pop();
	calculator_stack.push(b*a);
	cout << "Result of operation: "<< calculator_stack.top() << endl;
	return calculator_stack;
}

Stack popAndPrint(Stack calculator_stack)
{
	calculator_stack.pop();
	cout << calculator_stack.print() << endl;
	return calculator_stack;
}

Stack printStack(Stack calculator_stack)
{
	cout << calculator_stack.print() << endl;
	return calculator_stack;
}

Stack printStackSize(Stack calculator_stack)
{
	cout << "<" << calculator_stack.size() << ">" << endl;
	return calculator_stack;
}

Stack printCurrentResult(Stack calculator_stack)
{
	cout << calculator_stack.top() << endl;
	return calculator_stack;
}







