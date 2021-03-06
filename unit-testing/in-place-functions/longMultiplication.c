#include <stdio.h>
#include "bignums_ip.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file.

Example:

./longMultiplication 343456778384378290000000 34434999588887878867487736273762731116372
./longMultiplication bignumber.txt 100 bignumber2.txt 10

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

		printf("%s * %s ", number3, argv[2] );
		longMultiplicationInPlace(number3, argv[2]);
		printf("= %s\n\n", number3);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	return EXIT_SUCCESS;
}

