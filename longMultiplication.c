#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*The program receives as arguments, two unsigned integers

Example:

./longMultiplication 343456778384378290000000 34434999588887878867487736273762731116372
*/

char* longMultiplication( char *factor1,  char *factor2);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

int main(int argc, char* argv[])
{	
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if(argc == 3)
		result = longMultiplication(argv[1], argv[2]);	
	else if(argc == 5)
	{
		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));
		printf("%s * %s =\n\n", number1, number2);
		result = longMultiplication(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s\n", result);
	free(result);
	return EXIT_SUCCESS;
	
}

char* longMultiplication( char* factor1,  char* factor2)
{	
	unsigned long long f1Size = strlen(factor1), f2Size = strlen(factor2);
	unsigned long long resultSize = f1Size + f2Size;
	
	char *result; 
	unsigned int product = 0, prevCarry = 0, sumCarry = 0, carry = 0;
	
	// We need to prepare the space for normalized summands and the result... 
	result = calloc( resultSize+1, sizeof(char) );

	for(unsigned long long i = 0; i<resultSize; i++)
			result[i] = '0';
	result[resultSize] = '\0';
	
	unsigned long long longest, shortest, resultIndex, units;
	longest = f1Size >= f2Size ? f1Size : f2Size ;
	shortest = f2Size <= f1Size ? f2Size : f1Size ;
	units = 0;

	do
	{
		shortest--;
		resultIndex = (resultSize-1) - units;		
		do
		{
			// we get the product of the multiplication of two factors and its carry
			longest--;
			if( f1Size >= f2Size )
				product = (factor1[longest]-'0') * (factor2[shortest]-'0');
			else
				product = (factor1[shortest]-'0') * (factor2[longest]-'0');

			carry = (product > 9) ? product/10 : 0;
			product -= carry*10;

			// we add the previous carry to the current product
			product += prevCarry;
			prevCarry = (product > 9) ? product/10 : 0;
			product -= prevCarry*10;
			// we add the previous result to the current one
			product += (result[resultIndex] - '0');
			sumCarry = (product > 9) ? product/10 : 0;
			product -= sumCarry*10;
			//We get the carry for the next operation
			prevCarry += (carry + sumCarry); 
			// we get the product of the multiplication of two factors and its carry
			result[resultIndex] = (char)(product + '0');
			resultIndex--; 
		}while( longest >=1 );
		result[resultIndex] = (char)(prevCarry + '0');
		prevCarry = 0;
		units++;
		longest = f1Size >= f2Size ? f1Size : f2Size ;

	}while( shortest >= 1 );
	//most of the times you'll have spare zeros in the left, so we rotate 
	while(result[0] == '0' && strlen(result) > 1)
		memmove(result, result+1, resultSize*sizeof(char)+1);

	return result;
}

char* readBigNumber(char *fileName, const unsigned int SLICELENGTH)
{

	FILE *bigNum = fopen(fileName, "r");
	char c[2], *primeSlice;
	unsigned int counter = 0;
	primeSlice = calloc(SLICELENGTH+1, sizeof(char));
	
	while( (counter < SLICELENGTH) && ( ( c[0]=fgetc(bigNum) )!= EOF ) )
			if( isdigit(c[0]) )
				primeSlice[counter++] = c[0];

	primeSlice[SLICELENGTH] = '\0';
	fclose(bigNum);
	return primeSlice;
}
