/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from each file.

Examples:

./longSubtraction 343456778384378290000000 34434999588887878867487736273762731116372

./longSubtraction bignumber.txt 100 bignumber2.txt 10

*/

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
	/*Subtraction parts: 	9878-> minuend
				  98-> subtrahend*/
	unsigned long long int minuendLength, subtrahendLength, resultLength, shortest;
	int subtraction, carry = 0, sign = 0;
	char *result;
	bool minuendIsLower = true;

	if( minuend == NULL || subtrahend == NULL )
		return NULL;

	minuendLength = strlen(minuend); 
	subtrahendLength = strlen(subtrahend);
	resultLength = (minuendLength >= subtrahendLength) ? minuendLength : subtrahendLength;// 0 - 100 = [-]100
	
	//Error handling
	if( minuendLength == 0 && subtrahendLength == 0 )
		return NULL;
	if( subtrahendLength == 0 )
		return minuend;
	if( minuendLength == 0 )
		return subtrahend;
	
	//we need to verify if subtrahend is greater than minuend in which case the result will be negative
	minuendIsLower = (minuendLength < subtrahendLength) ? true : false;// If it's greater in length, for sure it will be greater

	if( minuendLength == subtrahendLength )//if they are equal in length we need to compare its digits
	{
		for(int compIndex = 0; compIndex<resultLength && !minuendIsLower; compIndex++)
			if( minuend[compIndex] < subtrahend[compIndex] )
				minuendIsLower = true;
			else if( minuend[compIndex] > subtrahend[compIndex] )
				compIndex = resultLength;
	}

	if(minuendIsLower)
		sign = 1;

	//Initializing space for the result
	result = calloc( resultLength + 1 + sign, sizeof(char) );// plus one: '\0' or two if it's negative 
	memset(result, '0', resultLength);
	result[resultLength] = '\0';

	if(minuendIsLower)
		result[0] = '-';
	shortest = (subtrahendLength <= minuendLength) ? subtrahendLength : minuendLength; // get the shortest out of both

	do
	{
		resultLength--;
		if(shortest > 0)
		{
			shortest--;
			if( minuendIsLower )
			{	// if the next number is zero and we have carry == -1 then we set carry to 9.
				if( resultLength <= minuendLength -2 && carry==-1 ) // subtrahend-2 -> evaluate from the penultimate digit.
					carry = ( subtrahend[resultLength] == '0' ) ? 9 : -1;	
				subtraction = ( subtrahend[resultLength] - '0') - (minuend[shortest] - '0' ) + carry;
			}
			else
			{
				if( resultLength <= subtrahendLength -2 && carry==-1 )// subtrahend-2 -> evaluate from the penultimate digit.
					carry = ( minuend[resultLength] == '0') ? 9 : -1;	
				subtraction = ( minuend[resultLength] - '0') - ( subtrahend[shortest] - '0' ) + carry;
			}

		}
		else if ( resultLength >= 0 )//This is used when one of the numbers has greater length than the other.
		{
			if( minuendIsLower )
				subtraction = (subtrahend[resultLength] - '0') + carry;
			else
				subtraction = (minuend[resultLength]-'0') + carry;
		}
		
		if( subtraction < 0 || carry == 9 )//if the result was negative or the carry had to be 9, then we set carry to -1 for the next digit
		{
			if( subtraction < 0)
				subtraction += 10;
			carry = -1;
		}
		else
			carry = 0;		
	
		result[ resultLength+sign ] = (char)( subtraction + '0' ) ;// starting one extra byte to the right of we have sign

	}while(resultLength > 0);//resultLength is unsigned so it'd cause a runtime error if it gets to -1


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
