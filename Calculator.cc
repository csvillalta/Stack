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


bool looksLikeInt(string s)
{
	std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
	return std::regex_match(s, intPattern);
}

void runCalculator()
{
	string token;

	while (cin >> token and token != "bye") {
		if (looksLikeInt(token)) {
			cout << "thanks for entering the integer " << stoi(token) << endl;
		} else {
			cout << "thanks for entering the non-integer token " << token << endl;
		}
	}
}
