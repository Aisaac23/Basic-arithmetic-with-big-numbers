#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

char *longFloatingPointMultiplication(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

char *longMultiplication(char *minuend, char *subtrahend);
unsigned long long count_decimal_digits(char *fp_number);
unsigned long long count_integer_digits(char *fp_number);


int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )
	{
		number1 = argv[1];
		result = longFloatingPointMultiplication(number1, argv[2]);
	}	
	else if(argc == 5)
	{
		if( !fileExists( argv[1] ) || !fileExists( argv[3] ) || !isUnsignedInteger(argv[2]) || isUnsignedInteger(argv[4]) )
			exit(EXIT_FAILURE);

		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));

		if( !isUnsignedInteger( number1 ) || !isUnsignedInteger( number1 ) )
			exit(EXIT_FAILURE);

		printf("%s / %s=\n\n", number1, number2);
		result = longFloatingPointMultiplication(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("Result: %s\n", result);
	if( result != NULL )
		free(result);

	return EXIT_SUCCESS;
}

char *longFloatingPointMultiplication(char *factor1, char *factor2)
{
	unsigned long long factor1Length, factor2Length, factor1_digit_count, factor2_digit_count, total_decimal_digits;
	char *result, *factor1_digit_buffer, *factor2_digit_buffer, *point_ptr;
	
	//Error handling
	if( factor1 == NULL || factor2 == NULL )
		return NULL;
	factor1Length = strlen(factor1);

	factor2Length = strlen(factor2);
	
	//Error handling
	if( factor1Length == 0 && factor2Length == 0 )
		return NULL;
	if( factor1Length == 0 )
		return factor2;
	if( factor2Length == 0 )
		return factor1;

	factor1_digit_count = count_decimal_digits(factor1);
	factor2_digit_count = count_decimal_digits(factor2);
	total_decimal_digits = factor1_digit_count + factor2_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter
	factor1_digit_buffer = calloc( factor1Length, sizeof(char) );
	factor2_digit_buffer = calloc( factor2Length, sizeof(char) );
	
	strcpy(factor1_digit_buffer, factor1);
	point_ptr = strchr(factor1_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), factor1_digit_count+1);
	
	strcpy(factor2_digit_buffer, factor2);
	point_ptr = strchr(factor2_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), factor2_digit_count+1);
	
	result = longMultiplication(factor1_digit_buffer, factor2_digit_buffer);
	
	point_ptr = (result+strlen(result)-total_decimal_digits);
	memmove(point_ptr+1, point_ptr, total_decimal_digits);
	point_ptr[0] = '.';
	
	return result;
}

unsigned long long count_decimal_digits(char *fp_number)
{
	unsigned long long total_digits = 0, length, point_loc;
	char *point;

	length = strlen(fp_number);
	point = strchr(fp_number, '.');
	
	if (point == NULL)
		return 0;
	point_loc = (point - fp_number +1);
	total_digits = length - point_loc; 

	return total_digits;
}

unsigned long long count_integer_digits(char *fp_number)
{
	unsigned long long integer_digits = 0, length;
	char *point;

	length = strlen(fp_number);
	point = strchr(fp_number, '.');
	
	if (point == NULL)
		return length;

	integer_digits = (point - fp_number); 

	return integer_digits;
}


char* longMultiplication( char* factor1,  char* factor2)
{	
	//Error handling
	if( factor1 == NULL || factor2 == NULL )
		return NULL;

	unsigned long long f1Size, f2Size, resultSize;
	unsigned long long longest, shortest, resultIndex, units;
	unsigned int product = 0, prevCarry = 0, sumCarry = 0, carry = 0;
	char *result; 
	
	f1Size = strlen(factor1);
	f2Size = strlen(factor2);
	
	//Error handling
	if( f1Size == 0 || f2Size == 0 )
		return NULL;
	
	resultSize = f1Size + f2Size;// 99x999=98,901 -> 2+3 = 5 digits long
	
	result = calloc( resultSize+1, sizeof(char) );
	memset(result, '0', resultSize);
	result[resultSize] = '\0';
	
	//Picking the shortest and longest number in length of characters
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

	return result;
}


char* readBigNumber(char *fileName, const unsigned int SLICELENGTH)
{

	FILE *bigNum = fopen(fileName, "r");
	char c[2], *primeSlice;
	unsigned int counter = 0;
	primeSlice = calloc(SLICELENGTH+1, sizeof(char));
	
	while( (counter < SLICELENGTH) && ( ( c[0]=fgetc(bigNum) )!= EOF ) )
			if( isdigit( c[0] ) )
				primeSlice[counter++] = c[0];

	primeSlice[SLICELENGTH] = '\0';
	fclose(bigNum);

	return primeSlice;
}
