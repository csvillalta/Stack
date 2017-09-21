#include <hc_list.h>
#include <hc_list_helpers.h>
using namespace HaverfordCS;
#include "listPractice.h"

#include <functional>	// for std::function

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

bool testListPractice()
{
	// Building the list using only the fundamental constructor
	list<int> list1 = list<int>();
	list1 = list<int>(-2, list1);
	list1 = list<int>(17, list1);
	list1 = list<int>(1, list1);
	list1 = list<int>(0, list1);
	list1 = list<int>(42, list1);

	// Building the list using ez_list secondary constructor
	list<int> list2 = ez_list(42, 0 , 1, 17, -2);
	cout << "list1 = " << list1 << "\n" << endl;
	cout << "list2 = " << list2 << "\n" << endl;
	cout << "sumPlusLength1(list2) = " << sumPlusLength1(list2) << "\n" << endl;
	cout << "sumPlusLength2(list2) = " << sumPlusLength2(list2) << endl;

	return true;  // should only return true if your tests of your pre-lab functions work
}

int sumPlusLength1(list<int> l)
{
	int total = 0; // Used to sum up the elements of the list
	int count = 0; // Keeps a count of the elements in the list
	while(!empty(l))
	{
		total += head(l);
		count++;
		l = rest(l);
	}

	return total + count;
}



int sumPlusLength2(list<int> l)
{
	list<int> listWithCountAdded = map<int, int>([](int x) { return x = x+1; }, l);
	int total = reduce<int, int>([](int i, int sumSoFar) { return i+sumSoFar; }, 0, listWithCountAdded);
	return total;
}
