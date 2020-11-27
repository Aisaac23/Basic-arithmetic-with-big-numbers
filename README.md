# Basic arithmetic with big numbers

The set of functions that's part of this project supports only integer and float positive numbers as strings.

You can make any of the programs inside *unit-testing* work in two ways:

1. Giving as arguments: two numbers greater or equal zero separated by a spece. (e.g. ./longAddition 3000000000 400000000000000)
2. Giving as arguments: a pair of these, name of a plain text file, the number of digits to take from this file separated by a space. (e.g ./longDivision M82589933.txt 1000024 file2.txt 200). The names of the files, of course, can be different.

**Additional Considerations**

* This project is bult using pure C standard (C99).

* The file M82589933.zip contains a pain text file with the largest Prime number found (2020). You can use it for testing since it's a **23,249,425** digit long number.
* None of the functions (except for *longDivisionWithReminder*) make any changes to the parameters passed so you are free to pass either constant strings or strings previously allocated in memory.


## bignums.c & bignums.h

You can **#include** these files in your project. Bellow you can find the name of every function and a brief description of their functionality.

*	longAddition();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
*	longSubtraction();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
*	longMultiplication();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
*	longDivision();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division. For this specific function we LOSE the reminder, to keep the reminder on the dividend use longDivisionWithReminder. The dividend should alwyas be >= the divisor otherwise the result will be zero.
*	longDivisionFloatingPointResult();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division with the passed floating-point precision.
*	longDivisionWithReminder();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division. For this specific function we KEEP the reminder on the pointer that originally stores the dividend, so be careful to save your dividend in case you want to use it later. The dividend should alwyas be >= the divisor otherwise the result will be zero.
*	longModule();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the REMINDER of the division.<br><br>
*	longFloatingPointSubtracction();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the subtraction.
*	longFloatingPointMultiplication();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the multiplication.
*	longFloatingPointDivision();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the division with the passed floating-point precision.
*	longFloatingPointAddition();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.<br><br>
*	increment();
	*	Recieves an string composed only by digits and increments the number that represents in one. It can handle overflow (e.g. 999+1)
*	decrement();
	*	Recieves an string composed only by digits and decrements the number that represents in one. It won't go lower than zero<br><br>
*	readBigNumber();
	*	Receives as arguments, the name of an existing file and an integer with the amount of digits to extract from the file. It returns a string with all the digits that could be read from the file until "SLICELENGTH" or EOF


### How to install

1. Add the prepocessor directive: **#include "bignums.h"** to your source code.
2. Compile adding the source code file like: <br>**gcc -Wall -o output yourCode.c bignums.c**
