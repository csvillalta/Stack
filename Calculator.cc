/*
	StackInterface.cc
	This file contains all of the function definitions for the StackInterface.
	These will be used to generate the text interface for the stack

	Functions:
	Title			  		Description
	------------------------------------------
*/

/*----------Includes---------*/
#include <stdlib.h> // for exit()
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

// findIndex takes a string vector input and two strings
// and returns the index of the value that has not been
// matched to a preceeding new_match_indicator.
int findIndex(std::vector<string> input, string value, string new_match_indicator)
{

	std::size_t i = 1;
	while (i < input.size())
	{
		if (input[i] == value)
		{
				return i;
		}
		else if (input[i] == new_match_indicator)
		{
			std::vector<string> after_if;
			after_if.insert(after_if.begin(), input.begin()+i, input.end());
			i += findIndex(after_if, value, new_match_indicator);
		}
		i = i + 1;
	}
	cout << "Missing an \"" << value << "\"" << endl;
	exit(1);
}

// This function checks if there are equal occurences of strings a, b and c
// in the vector.
bool checkFormat(std::vector<string> input_vector, string a, string b, string c)
{
	int a_count=0, b_count=0, c_count=0;
	for (unsigned p = 0; p<input_vector.size(); p++)
	{
		if (input_vector[p] == a)
		{
			a_count++;
		}
		if (input_vector[p] == b)
		{
			b_count++;
		}
		if (input_vector[p] == c)
		{
			c_count++;
		}
	}
	return (a_count == b_count && b_count == c_count);
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
	addFunctionToDict(">=", lessThanOrEqual);
	addFunctionToDict("<=", greaterThanOrEqual);
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


void runUserFunc(std::vector<string> function_body) // This function is what parses our user defined function string
{
	std::vector<string> maintained_user_func_copy = function_body; // a function_body copy that is not mutated while the loop runs
																	// this will allow a while loop to return to the beginning of the begin
																	// block even after the begin block's code has been run and mutated!
	std::vector<string> user_func_copy = function_body;
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
			runUserFunc(mapOfUserFunctions[curr_token]);
			user_func_copy.erase(user_func_copy.begin());
		}
		else if (curr_token == "if")
		{
			if (checkFormat(user_func_copy, "if", "else", "endif"))
			{
				if (calc_stack.pop() != 0) // i.e. if top of stack is non-zero
				{
					user_func_copy.erase(user_func_copy.begin());
					int else_pos = findIndex(user_func_copy, "else", "if");
//					cout << else_pos << endl;
					int endif_pos = findIndex(user_func_copy, "endif", "if");
//					cout << endif_pos << endl;
					user_func_copy.erase(user_func_copy.begin()+(else_pos), user_func_copy.begin()+(endif_pos+1));
//					for (unsigned p = 0; p<user_func_copy.size(); p++)
//					{
//						cout << user_func_copy[p] << "a" << endl;
//					}
				}
				else // i.e. if top of stack is zero
				{
					int else_pos = findIndex(user_func_copy, "else", "if");
					user_func_copy.erase(user_func_copy.begin(), user_func_copy.begin()+(else_pos+1));
					int endif_pos = findIndex(user_func_copy, "endif", "if");
					user_func_copy.erase(user_func_copy.begin()+endif_pos);
					for (unsigned p = 0; p<user_func_copy.size(); p++)
					{
						cout << user_func_copy[p] << "a" << endl;
					}
				}

			}
			else
			{
				cout << "If/Else/Endif mismatch" << endl;
			}
		}
		else if (curr_token == "begin")
		{
			if (checkFormat(user_func_copy, "begin", "while", "repeat"))
			{
	//			cout << "entered begin" << endl;
				std::vector<string> user_func_loop_copy = user_func_copy; // making a copy we can mutate freely without mutating the original
	//			for (unsigned p = 0; p<user_func_loop_copy.size(); p++)
	//			{
	//				cout << user_func_copy[p] << "|1|" << endl;
	//			}
				user_func_loop_copy.erase(user_func_loop_copy.begin());	// erase the "begin" keyword
	//			for (unsigned p = 0; p<user_func_loop_copy.size(); p++)
	//			{
	//				cout << user_func_copy[p] << "|2|" << endl;
	//			}
				runUserFunc(user_func_loop_copy); // i.e. run the code after the while loop...
				user_func_copy.erase(user_func_copy.begin(), user_func_copy.begin()+findIndex(user_func_copy, "repeat", "begin")+1);
			}
			else
			{
				cout << "Begin/While/Repeat mismatch" << endl;
			}
		}
		else if (curr_token == "while") // when the token reaches a while, the while shall check a condition and that will determine to
		{								// run it's code and hit the return statement or to skip to after it's corresponding return.
										// Invariant: assuming the function_body parameter is the code right after the begin encountered
			if (calc_stack.pop() != 0) // i.e. the flag/condition is true, then continue running the while code block
			{
//				cout << "entered while true case" << endl;
				std::vector<string> while_code_block (user_func_copy.begin()+1, user_func_copy.begin()+findIndex(user_func_copy, "repeat", "begin"));
//				for (unsigned p = 0; p<while_code_block.size(); p++)
//				{
//					cout << user_func_copy[p] << "|2|" << endl;
//				}
				// std::vector<string> begin_code_block (user_func_copy.begin(), user_func_copy.begin()+findIndex(user_func_copy, "while", "begin")-1);
				runUserFunc(while_code_block); // run the while_code_block
//				cout << "ran while code block" << endl;
				user_func_copy = maintained_user_func_copy; // run the code again from the start (this will work since the stack is being mutated in the background)
			}
			else	// i.e. the flag/condition is false, then skip to after this while's corresponding return
			{
//				cout << "entered while false case" << endl;
				user_func_copy.clear();
//				int return_pos = findIndex(user_func_copy, "repeat", "begin");
//				user_func_copy.erase(user_func_copy.begin(), user_func_copy.begin()+return_pos);
			}
		}
		else if (curr_token == "repeat")
		{
			cout << "we read repeat..." << endl;
			exit(1);
		}
		else
		{
			cout << "Not a valid input; please continue." << endl;
			user_func_copy.erase(user_func_copy.begin());
		}
}
}

void runCalculator()
{
	string token;
	functionsToAdd();

	cout << "Welcome to Postfix Calculator by Christopher Villalta" << endl;

	while (cin >> token and token != "bye") {
		if (not TAKE_USER_INPUT)
		{
			if (token == ":")
			{
				TAKE_USER_INPUT = true;
				// cout << "Now taking user input." << endl;
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
					std::vector<string> token_corresponding_function = mapOfUserFunctions[token];
					runUserFunc(token_corresponding_function);
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
				// cout << "No longer taking user input. Here is the string: " << user_input_string << "key: " << user_function_name << "value: " << user_function_def << endl;
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

Stack greaterThanOrEqual(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	if (stack_top >= stack_second)
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

Stack lessThanOrEqual(Stack calculator_stack)
{
	if (calculator_stack.size() <= 1) {
		cout << "Not enough inputs in stack to perform this operation." << endl;
		return calculator_stack;
	}
	int stack_top = calculator_stack.pop();
	int stack_second = calculator_stack.pop();
	if (stack_top <= stack_second)
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




