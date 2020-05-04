#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"
/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from the file.

Examples:

./longAddition 343456778384378290000000 34434999588887878867487736273762731116372

./longAddition bignumber.txt 100 bignumber2.txt 10

*/
char* longAddition( char* summand1,  char* summand2);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

int main(int argc, char* argv[])
{	
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 && isUnsignedInteger(argv[1]) && isUnsignedInteger(argv[2]) )//direct input
		result = longAddition(argv[1], argv[2]);
	else if(argc == 5)//input from file
	{
		if( !fileExists( argv[1] ) || !fileExists( argv[3] ) || !isUnsignedInteger(argv[2]) || isUnsignedInteger(argv[4]) )
			exit(EXIT_FAILURE);

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		if( !isUnsignedInteger( number1 ) || !isUnsignedInteger( number1 ) )
			exit(EXIT_FAILURE);

		printf("%s + %s=\n\n", number1, number2);
		result = longAddition(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s\n", result);
	if(result)
		free(result);

	return EXIT_SUCCESS;
}

char* longAddition( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultSize, shortest, carry = 0, newSize;
	char *result; 
	unsigned int sum = 0;//biggest number for this variable will be 18.
	bool summand1IsShorter;	
	
	//Error handling
	if( summand1 == NULL || summand2 == NULL )
		return NULL;
	summand1Length = strlen(summand1);
	summand2Length = strlen(summand2);
	
	//Error handling
	if( summand1Length == 0 && summand2Length == 0 )
		return NULL;
	if( summand1Length == 0 )
		return summand2;
	if( summand2Length == 0 )
		return summand1;


	resultSize = (summand1Length >= summand2Length) ? summand1Length+1 : summand2Length+1;
	
	result = calloc( resultSize+1, sizeof(char) );
	for(unsigned long long i = 0; i<resultSize; i++)
			result[i] = '0';
	result[resultSize] = '\0';
	
	//Picking the shortest number in length of characters
	shortest = (summand1Length <= summand2Length) ? summand1Length : summand2Length;
	summand1IsShorter = (summand1Length <= summand2Length) ? true : false;
	do
	{
		resultSize--;
		if(shortest > 0)
		{
			shortest--;
			if( summand1IsShorter )
				sum = (summand1[shortest]-'0') + (summand2[resultSize-1]-'0');
			else
				sum = (summand1[resultSize-1]-'0') + (summand2[shortest]-'0');
		}
		else if ( resultSize >= 1 )// When one of the summands was added but there are digits left tu add in the other one.
		{
			if( summand1IsShorter )
				sum = (summand2[resultSize-1]-'0');
			else
				sum = (summand1[resultSize-1]-'0');
		}
		else
			sum = 0;
		
		sum+=carry;
		carry = (sum > 9) ? sum/10 : 0;
		sum = (sum > 9) ? sum%10 : sum;
		result[resultSize] = (char)( sum + '0' );

	}while( resultSize > 0 );

	//you could have a spare zero in the left, so we rotate
	if(result[0] == '0' && strlen(result) > 1)
		memmove(result, result+1, strlen(result)*sizeof(char) );
	
	//Finally we resize the result to get only the useful numbers.
	newSize = strlen(result);
	result = realloc(result, newSize);
	
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
