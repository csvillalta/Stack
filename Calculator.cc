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
#include <vector>
#include <iostream>
#include <regex>  // check if something is numeric with a "regular expression" (coming up later)
using namespace std;

map<string, function<Stack (Stack)>> mapOfFunctions;
map<string, std::vector<string>> mapOfUserFunctions;
Stack calc_stack;
bool TAKE_USER_INPUT = false; // flag for taking user input
string user_input_string = ""; // initialize user input string for use in the REPL

// This function was modeled off of: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<string> delimiterParse(string s, string delimiter)
{
	std::vector<string> string_split;
	std::size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    string_split.push_back(token);
	    s.erase(0, pos + delimiter.length());
	}
	return string_split;
}

int findIndex(std::vector<string> input, string value)
{

	std::size_t i = 1;
	while (i < input.size())
	{
		if (input[i] == value)
		{
				return i;
		}
		else if (input[i] == "if")
		{
			std::vector<string> after_if;
			after_if.insert(after_if.begin(), input.begin()+i, input.end());
			i += findIndex(after_if, value);
		}
		i = i + 1;
	}
	// TODO Better error handling
	cout << "BAD" << endl;
	return -1;
}

bool looksLikeInt(string s)
{
	std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
	return std::regex_match(s, intPattern);
}

void addFunctionToDict(string function_name, function<Stack (Stack)> f)
{
	mapOfFunctions[function_name] = f;
}

void addFunctionToUserDict(string function_name, std::vector<string> function_body)
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
	std::vector<string> user_func_copy = mapOfUserFunctions[token]; // TODO user_func_copy is now a vector!
	while (not user_func_copy.empty())
	{
		string curr_token = user_func_copy.front();
		if (looksLikeInt(curr_token))
		{
			calc_stack.push(stoi(curr_token));
			user_func_copy.erase(user_func_copy.begin());
		}
		else if (looksLikeFunc(curr_token))
		{
			calc_stack = mapOfFunctions[curr_token](calc_stack);		// applies the function to calc_stack
			user_func_copy.erase(user_func_copy.begin());
		}
		else if (looksLikeUserFunc(curr_token))
		{
			runUserFunc(curr_token);
			user_func_copy.erase(user_func_copy.begin());
		}
		else if (curr_token == "if")
		{
			if (true) // TODO I need to change 'true' to a function that returns true if the if has a corresponding else and endif
			{
				if (calc_stack.pop() != 0) // i.e. if top of stack is non-zero
				{
					user_func_copy.erase(user_func_copy.begin());
					int else_pos = findIndex(user_func_copy, "else");
					cout << else_pos << endl;
					int endif_pos = findIndex(user_func_copy, "endif");
					cout << endif_pos << endl;
					user_func_copy.erase(user_func_copy.begin()+(else_pos), user_func_copy.begin()+(endif_pos+1));
//					for (unsigned p = 0; p<user_func_copy.size(); p++)
//					{
//						cout << user_func_copy[p] << "a" << endl;
//					}
				}
				else // i.e. if top of stack is zero
				{
					cout << "we in bois" << endl;
					int else_pos = findIndex(user_func_copy, "else");
					user_func_copy.erase(user_func_copy.begin(), user_func_copy.begin()+(else_pos+1));
					int endif_pos = findIndex(user_func_copy, "endif");
					user_func_copy.erase(user_func_copy.begin()+endif_pos);
					for (unsigned p = 0; p<user_func_copy.size(); p++)
					{
						cout << user_func_copy[p] << "a" << endl;
					}
				}

			}
			// TODO Add error handling else statement when if does not have proper format!
		else
		{
			cout << "Not a valid input; please continue." << endl;
		}
	}
}
}

void runCalculator()
{
	string token;
	functionsToAdd();

	cout << "Welcome to Postfix Calculator by Christopher Villalta\nPlease enter some integers!" << endl;

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
				std::vector<string> user_function_def_v = delimiterParse(user_function_def, " ");
				addFunctionToUserDict(user_function_name, user_function_def_v);
				//cout << "No longer taking user input. Here is the string: " << user_input_string << "key: " << user_function_name << "value: " << user_function_def << endl;
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
	if (calculator_stack.size() < 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	calculator_stack.pop();
	return calculator_stack;
}

Stack nip(Stack calculator_stack)
{
	if (calculator_stack.size() < 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	calculator_stack.pop();
	calculator_stack.push(stack_top);
	return calculator_stack;
}

Stack dup(Stack calculator_stack)
{
	if (calculator_stack.size() < 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	calculator_stack.push(calculator_stack.top());
	return calculator_stack;
}

Stack over(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_second);
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	return calculator_stack;
}

Stack tuck(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	calculator_stack.push(stack_top);
	return calculator_stack;
}

Stack swapp(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	calculator_stack.push(stack_top);
	calculator_stack.push(stack_second);
	return calculator_stack;
}

Stack greaterThan(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
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
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
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
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
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




