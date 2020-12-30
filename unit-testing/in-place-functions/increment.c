#include <stdio.h>
#include "bignums.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file.

Examples:

./longAddition 343456778384378290000000 34434999588887878867487736273762731116372

./longAddition bignumber.txt 100 bignumber2.txt 10

*/

int main(int argc, char* argv[])
{	
	char *number1 = NULL;

	if( argc == 2 )//direct input
	{
		number1 =  calloc( strlen(argv[1])+1 , sizeof(char));
		strcpy(number1, argv[1]);
		printf("%s", number1 );
		increment( number1 );
	}
	else if(argc == 4)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));

		increment(number1);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("++ = %s\n\n", number1 );

	return EXIT_SUCCESS;
}

