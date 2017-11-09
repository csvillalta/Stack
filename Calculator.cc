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
map<string, string> mapOfUserFunctions;
Stack calc_stack;
bool TAKE_USER_INPUT = false; // flag for taking user input
string user_input_string = ""; // initialize user input string for use in the REPL

bool looksLikeInt(string s)
{
	std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
	return std::regex_match(s, intPattern);
}

void addFunctionToDict(string function_name, function<Stack (Stack)> f)
{
	mapOfFunctions[function_name] = f;
}
void addFunctionToUserDict(string function_name, string function_body)
{
	mapOfUserFunctions[function_name] = function_body;
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

bool looksLikeUserFunc(string s) // Referenced https://www.cprogramming.com/tutorial/stl/stlmap.html
{
	if(mapOfUserFunctions.find(s) == mapOfUserFunctions.end())
		return false;
	return true;
}

void runUserFunc(string token) // This function is what parses our user defined function string
{
	string user_func_copy = mapOfUserFunctions[token];
	string curr_token;
	int original_length = user_func_copy.length();
	int end;
	while (user_func_copy.length() != 0)
	{
		end = user_func_copy.find(" ")+1;
		curr_token = user_func_copy.substr(0, end-1);	// extracts a function from our value string
		cout << curr_token << endl;
		if (looksLikeInt(curr_token))
		{
			calc_stack.push(stoi(curr_token));
		}
		else if (looksLikeFunc(curr_token))
		{
			calc_stack = mapOfFunctions[curr_token](calc_stack);		// applies the function to calc_stack
		}
		else if (looksLikeUserFunc(curr_token))
		{
			runUserFunc(curr_token);
		}
		else
		{
			cout << "Not a valid input; please continue." << endl;
		}
		user_func_copy = user_func_copy.substr(end, original_length);
	}
}

void runCalculator()
{
	string token;
	functionsToAdd();

	cout << "Welcome to Postfix Calculator by Christopher Villalta\nPlease enter some integers." << endl;

	while (cin >> token and token != "bye") {
		if (not TAKE_USER_INPUT)
		{
			if (token == ":")
			{
				TAKE_USER_INPUT = true;
				cout << "Now taking user input." << endl;
			}
			else if (looksLikeInt(token))
			{
				cout << "Thanks for entering the integer " << stoi(token) << endl;
				calc_stack.push(stoi(token));
			}
			else
			{
				if (looksLikeFunc(token))
				{
					cout << "Applying " << token << " ..." << endl;
					calc_stack = mapOfFunctions[token](calc_stack);
				}
				else if (looksLikeUserFunc(token))
				{
					runUserFunc(token);
				}
				else
				{
					cout << "Not a valid input; please continue." << endl;
				}
			}
		}
		else
		{
			if (token != ";")
			{
				user_input_string += token + " ";
			}
			else
			{
				TAKE_USER_INPUT = false;
				string user_function_name = user_input_string.substr(0, user_input_string.find(" "));
				string user_function_def = user_input_string.substr(user_input_string.find(" ")+1, user_input_string.length());
				cout << user_function_def << endl;
				addFunctionToUserDict(user_function_name, user_function_def);
				cout << "No longer taking user input. Here is the string: " << user_input_string << "key: " << user_function_name << "value: " << user_function_def << endl;
				user_input_string = ""; // resets our user string to define new definitions
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


