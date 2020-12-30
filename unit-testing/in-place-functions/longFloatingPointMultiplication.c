#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "bignums.h"

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )//direct input
	{
		number1 = argv[1];
		number2 = argv[2];
		result = longFloatingPointMultiplication( number1, number2 );
	}
	else if(argc == 5)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		result = longFloatingPointMultiplication(number1, number2);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s * %s = %s\n\n", number1, number2, result);

	return EXIT_SUCCESS;
}

