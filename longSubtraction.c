/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from the file.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

char* longSubtraction(char minuend[], char subtrahend[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 && isUnsignedInteger(argv[1]) && isUnsignedInteger(argv[2]) )//direct input
		result = longSubtraction(argv[1], argv[2]);
	else if(argc == 5)// input from file
	{
		if( !fileExists( argv[1] ) || !fileExists( argv[3] ) || !isUnsignedInteger(argv[2]) || isUnsignedInteger(argv[4]) )
			exit(EXIT_FAILURE);

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		if( !isUnsignedInteger( number1 ) || !isUnsignedInteger( number1 ) )
			exit(EXIT_FAILURE);

		printf("%s + %s=\n\n", number1, number2);
		result = longSubtraction(number1, number2);
	}
	else
	{
		printf("Data missing or with wrong format in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("%s\n", result);
	if(result)
		free(result);

	return EXIT_SUCCESS;
}

char *longSubtraction(char *minuend, char *subtrahend)
{
	unsigned long long int minuendLength, subtrahendLength, resultSize, shortest, newSize;
	int subtraction, carry = 0;
	char *result;
	bool subtrahendIsShorter = true;

	if( minuend == NULL || subtrahend == NULL )
		return NULL;

	minuendLength = strlen(minuend); 
	subtrahendLength = strlen(subtrahend);
	resultSize = (minuendLength >= subtrahendLength) ? minuendLength : subtrahendLength;
	
	//Error handling
	if( minuendLength == 0 && subtrahendLength == 0 )
		return NULL;
	if( subtrahendLength == 0 )
		return minuend;
	if( minuendLength == 0 )
		return subtrahend;


	result = calloc( resultSize+1, sizeof(char) );
	
	//we need to verify if subtrahend is greater than minuend in which case the result will be negative
	subtrahendIsShorter = (subtrahendLength <= minuendLength) ? true : false;// If it's greater in length, for sure it will be greater
	if( minuendLength == subtrahendLength )//if they are equal in length we need to compare its digits
	{
		for(int compIndex = 0; compIndex<resultSize && subtrahendIsShorter; compIndex++)
			if( minuend[compIndex] < subtrahend[compIndex] )
				subtrahendIsShorter = false;
			else if( minuend[compIndex] > subtrahend[compIndex] )
				compIndex = resultSize;
	}

	for(unsigned long long int i = 0; i<resultSize; i++)
			result[i] = '0';
	result[resultSize] = '\0';
	
	shortest = (subtrahendLength <= minuendLength) ? subtrahendLength : minuendLength; // get the shortest out of both
	do
	{
		resultSize--;
		if(shortest > 0)
		{
			shortest--;
			if( subtrahendIsShorter )
			{	// if the next number is zero and we have carry == -1 then we set carry to 9.
				if( resultSize < subtrahendLength-2 && carry==-1 ) 
					carry = ( minuend[resultSize] == '0') ? 9 : -1;	
				subtraction = (minuend[resultSize] - '0') - (subtrahend[shortest] - '0') + carry;
			}
			else
			{
				if( resultSize < strlen(minuend)-2 && carry==-1 )
					carry = ( subtrahend[resultSize] == '0') ? 9 : -1;	
				subtraction = (subtrahend[resultSize] - '0') - (minuend[shortest] - '0') + carry;
			}
			
		}
		else if ( resultSize >= 0 )//This is used when one of the numbers is has greater length than the other.
		{
			if( subtrahendIsShorter )
				subtraction = (minuend[resultSize] - '0') + carry;
			else
				subtraction = (subtrahend[resultSize]-'0') + carry;
		}
		
		if( subtraction < 0 || carry == 9 )//if the result was negative or the carry had to be 9 we set carry to -1 for the next digit
		{
			if( subtraction < 0 )
				subtraction += 10;
			carry = -1;
		}
		else
			carry = 0;		
	
		result[ resultSize ] = (char)( subtraction + '0' ) ;

	}while(resultSize > 0);//resultSize is unsigned so it'd cause a runtime error if it gets to -1
	
	//you could have spare zeros in the left, so we rotate 
	while(result[0] == '0' && strlen(result) > 1)
		memmove(result, result+1, strlen(result)*sizeof(char)+1);

	newSize = strlen(result);
	if( !subtrahendIsShorter ) //if the result was negative, we add the - sign.
	{
		result = realloc( result, newSize*sizeof(char)+2 );
		memmove(result+1, result, newSize*sizeof(char)+1 );
		result[0]='-';	
	}
	else //otherwise we just resize it
		result = realloc( result, newSize*sizeof(char)+1 );

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
