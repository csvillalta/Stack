My calculator implementation consists of several parts:

	1) a stack
	2) pre-defined functions that can manipulate that stack like a calculator
	3) a pre-defined dictionary that maps certain strings to the pre-defined functions
	4) a dictionary that will hold user defined functions
	5) a REPL loop that interprets user inputs as either: integers, pre-defined functions, user-defined functions

The REPL loop is a combination of if, else if and else clauses that classify
a user input token and decide what to do with it based on the classification.

User defined functions begin with ":" which triggers a flag in the REPL that
makes it reroute tokens and append them to a token string.

User defined functions end with ";" which resets the flag and splits the
token string into two pieces which will define the key and value to enter
into the user defined function dictionary. The key is a string and the value
is a vector that holds the individual function name strings.

Now the REPL is set up to first recognize integers, then pre-defined functions
(by searching the pre-defined function dictionary) then finally user-defined functions (again, by
searching the user function dictionary). If the token does not match any of those, then
the REPL states that it did not recognize the token as a valid input and does
nothing to the stack and continues running.

The benefits of this implementation allows for easy addition of pre-defined functions.
User-defined functions are also handled fairly neatly by storing them in a dictionary
as a key-value pair where the key is the name of the function (i.e. the first token
in the user input string) and the value is the rest of that string (i.e the function
body). Thus, when the user defines a function and calls it, there is a function that
parses the value string and applies the individual tokens embedded in it.

The tradeoffs in space are having to maintain two dictionaries and having to maintain
a user defined string. This system does not check the validity of a function until runtime
where the REPL simply ignores any nonsense inputs. Recursion in user defined functions is
currently limited to infinite recursion since no if statement has been implemented.

The tradeoffs in time come from having to search a dictionary to run a function every time
we call that function.


If Else Endif implementation: The calculator now accepts if else and endif clauses, but only
in user function definitions (as is done in FORTH). The function that runs user functions
simply checks for an "if" in the vector. As soon as this "if" is encountered, there is a call
to another function which checks which else and endif correspond to this if (this is important
for nested if clauses). The if simply pops the stack and checks if the item is non-zero which
will then determine whether to edit the vector to exclude the if command block or the else
command block.

Begin While Repeat implementation: The calculator now accepts FORTH style loops, but only in
user function definitions. These loops are implemented very similarly to how the if clauses are;
the function that runs user defined functions checks for a "begin" which then calls a function to
check where this begin's corresponding while and repeat are (important for tracking nested loops).
