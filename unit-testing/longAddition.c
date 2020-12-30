#include <stdio.h>
#include "bignums.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file.

Examples:

./longAddition 343456778384378290000000 34434999588887878867487736273762731116372

./longAddition bignumber.txt 100 bignumber2.txt 10

*/

int main(int argc, char* argv[])
{	
	char *result = NULL, *number1 = NULL, *number2 = NULL;
	

	if( argc == 3 )//direct input
	{
		number1 = argv[1];
		number2 = argv[2];
		result = longAddition( number1, number2 );
	}
	else if(argc == 5)//input from file
	{

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		result = longAddition(number1, number2);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s + %s = %s\n", number1, number2, result);
	
	return EXIT_SUCCESS;
}

