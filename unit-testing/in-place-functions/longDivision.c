#include <stdio.h>
#include "bignums_ip.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file. First the dividend and then the divisor. The dividend should alwyas be >= the divisor otherwise the result will be zero.

Example:
./longDivision 343456778384378290000000 3443499958888
./longDivision bignumber.txt 100 bignumber2.txt 10

NOTE: for this specific function we lose the reminder, to keep the reminder on the dividend use longDivisionWithReminder.

*/

int main(int argc, char* argv[])
{
	unsigned long long len1;

	if( argc == 3 )//direct input
	{
		len1 = strlen(argv[1]);

		char number3[len1+1];
		strcpy(number3, argv[1]);
		number3[len1] = '\0';

		printf("%s / %s ", number3, argv[2] );
		longDivisionInPlace(number3, argv[2]);
		printf("= %s\n\n", number3);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	return EXIT_SUCCESS;
}

