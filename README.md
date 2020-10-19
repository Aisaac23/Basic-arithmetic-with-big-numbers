# Basic-arithmetic-with-big-numbers

The programs that are part of this project support only integer and float positive numbers as strings.

You can make any of the programs work in two ways:
- Giving as arguments: 2 numbers greater or equal zero separated by a spece. (e.g. ./longAddition 3000000000 400000000000000)
- Giving as arguments: a pair of these, name of a plain text file, the number of digits to take from this file separated by a space. (e.g ./longDivision M82589933.txt 1000024 file2.txt 200). The names of the files, of course, can be different.

## bignums.c & bignums.h

These files are the header version of the programs described above. With the goal of making the functions resusable. It includes four additional functions for big numbers:

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
*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division. For this specific function we KEEP the reminder on the pointer that originally stores the dividend, so becareful to save your dividend in case you want to use it later. The dividend should alwyas be >= the divisor otherwise the result will be zero.
*	longModule();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the REMINDER of the division.
*	longFloatingPointSubtracction();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the subtraction.
*	longFloatingPointMultiplication();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the multiplication.
*	longFloatingPointDivision();
	*	Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the division with the passed floating-point precision.
*	longFloatingPointAddition();
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
*	readBigNumber();
	*	Receives as arguments, the name of an existing file and an integer with the amount of digits to extract from the file. It returns a string with all the digits that could be read from the file until "SLICELENGTH" or EOF
*	increment();
	*	Recieves an string composed only by digits and increments the number that represents in one. It can handle overflow (e.g. 999+1)
*	descrement();
	*	Recieves an string composed only by digits and decrements the number that represents in one. It won't go lower than zero.
*	formatNumber();
	*	It allows you to divide a number (as string) into equally sized groups (e.g. 19,000,000,000,000). It receives the string to be formated, the size of each group and the char that will serve as separator. Note: to use the new line char, you need to pass 10 instead of \n.
