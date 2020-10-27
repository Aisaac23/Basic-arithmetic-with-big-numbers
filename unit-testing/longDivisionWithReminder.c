#include <stdio.h>
#include "bignums.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file. First the dividend and then the divisor. The dividend should alwyas be >= the divisor otherwise the result will be zero.

Example:
./longDivisionWithReminder 343456778384378290000000 3443499958888
./longDivisionWithReminder bignumber.txt 100 bignumber2.txt 10

NOTE: for this specific function we keep the reminder, but we have to make sure to pass as arguments two modifiable arrays, in other words: DO NOT pass constant expressions as arguments.

*/

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )//direct input
	{
		number1 = argv[1];
		number2 = argv[2];
		printf("%s", number1);
		result = longDivisionWithReminder( number1, number2 );
	}
	else if(argc == 5)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));
		printf("%s", number1);
		result = longDivisionWithReminder(number1, number2);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf(" / %s = %s, reminder: %s\n\n", number2, result, number1);

	return EXIT_SUCCESS;
}
