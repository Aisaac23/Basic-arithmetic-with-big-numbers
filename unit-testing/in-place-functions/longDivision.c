#include <stdio.h>
#include "bignums.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file. First the dividend and then the divisor. The dividend should alwyas be >= the divisor otherwise the result will be zero.

Example:
./longDivision 343456778384378290000000 3443499958888
./longDivision bignumber.txt 100 bignumber2.txt 10

NOTE: for this specific function we lose the reminder, to keep the reminder on the dividend use longDivisionWithReminder.

*/

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )//direct input
	{
		number1 = argv[1];
		number2 = argv[2];
		result = longDivision( number1, number2 );
	}
	else if(argc == 5)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		result = longDivision(number1, number2);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s / %s = %s\n", number1, number2, result);

	unsigned long long len1, len2;
	len1 = strlen(argv[1]);
	len2 = strlen(argv[2]);

	char number3[len1+1], number4[len2+1];
	strcpy(number3, argv[1]);
	number3[len1] = '\0';
	strcpy(number4, argv[2]);
	number4[len2] = '\0';

	longDivisionInPlace(number3, number4);
	printf("In place: cuotient = %s\n\n", number3);


	return EXIT_SUCCESS;
}

