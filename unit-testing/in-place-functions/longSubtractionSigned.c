#include <stdio.h>
#include "bignums_ip.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file.

Examples:

./longSubtraction 343456778384378290000000 34434999588887878867487736273762731116372

./longSubtraction bignumber.txt 100 bignumber2.txt 10

*/

int main(int argc, char* argv[])
{
	unsigned long long len1, len2;

	if( argc == 3 )//direct input
	{
		len1 = strlen(argv[1]);
		len2 = strlen(argv[2]);

		char number3[len1+1], number4[len2+1];
		strcpy(number3, argv[1]);
		strcpy(number4, argv[2]);
		number3[len1] = '\0';
		number4[len2] = '\0';

		printf("%s - %s ", number3, number4 );
		longSubtractionInPlaceSigned(number3, number4);
		if(len1 > len2)
			printf("= %s\n\n", number3);
		else
			printf("= %s\n\n", number4);
	}
	else
	{
		printf("There could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	return EXIT_SUCCESS;
}

