# Basic-arithmetic-with-big-numbers

The four programs that are part of this project support only integer positive numbers and their behaviour with values different than that is not defined.

You can make any of the programs work in two ways:
- Giving as arguments 2 numbers greater or equal zero separated by a spece. (e.g. ./longAddition 3000000000 400000000000000)
- Giving as arguments a pair of these: name of a plain text file, the number of digits to take from this file separated by a space. (e.g ./longDivision M82589933.txt 10 M82589933.txt 2).The names of the files, of course, can be different.

## bignums.c & bignums.h

These files are the header version of the programs described above. With the goal of making the functions resusable. It includes four additional functions for big numbers:

*	char* longSubtraction(char minuend[], char subtrahend[]);
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
*	char* longMultiplication( char* factor1,  char* factor2);
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
*	char *longDivision(char *dividend, char divisor[]);
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division.
*	char* longAddition( char* summand1,  char* summand2);
	*	Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
*	char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);
	*	Receives as arguments, the name of an existing file and an integer with the amount of digits to extract from the file. It returns a string with all the digits that could be read from the file until "SLICELENGTH" or EOF
*	void increment(char* numberPlusPlus);
	*	Recieves an string composed only by digits and increments the number that represents in one. It can handle overflow (e.g. 999+1)
*	void descrement(char* numberPlusPlus);
	*	Recieves an string composed only by digits and decrements the number that represents in one. It won't go lower than zero.
*	char* formatNumber(char *n, int slice, char separator);
	*	It allows you to divide a number (as string) into equally sized groups (e.g. 19,000,000,000,000). It receives the string to be formated, the size of each group and the char that will serve as separator. Note: to use the new line char, you need to pass 10 instead of \n.




