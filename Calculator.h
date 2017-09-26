/*
	StackInterface header file
*/

#ifndef __CALCULATOR_H_INCLUDED__   // if x.h hasn't been included yet...
#define __CALCULATOR_H_INCLUDED__   //   #define this so the compiler knows it has been included

Stack stack_add(Stack calculator_stack);
Stack stack_subtract(Stack calculator_stack);
Stack stack_multiply(Stack calculator_stack);
Stack popAndPrint(Stack calculator_stack);
Stack printStack(Stack calculator_stack);
Stack printStackSize(Stack calculator_stack);
Stack printCurrentResult(Stack calculator_stack);
void runCalculator();

#endif
