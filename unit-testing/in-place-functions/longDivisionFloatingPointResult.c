#include <stdlib.h>
#include <stdio.h>
#include "bignums.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file. First the dividend and then the divisor. 

Example:
./longDivisionFloatingPointResult 343456778384378290000000 3443499958888
./longDivisionFloatingPointResult bignumber.txt 100 bignumber2.txt 10


*/

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;
	unsigned long long precision;

	if( argc == 4 )//direct input
	{
		number1 = argv[1];
		number2 = argv[2];
		precision = atoi(argv[3]);
		
		result = longDivisionFloatingPointResult( number1, number2, precision);
	}
	else if(argc == 6)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));
		precision = atoi(argv[3]);

		result = longDivisionFloatingPointResult(number1, number2, precision);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s / %s = %s\n\n", number1, number2, result);

	return EXIT_SUCCESS;
}
