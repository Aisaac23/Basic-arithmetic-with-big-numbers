#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

char *longFloatingPointSubtracction(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

char *longSubtraction(char *minuend, char *subtrahend);
unsigned long long count_decimal_digits(char *fp_number);
unsigned long long count_integer_digits(char *fp_number);


int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )
	{
		number1 = argv[1];
		result = longFloatingPointSubtracction(number1, argv[2]);
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
		result = longFloatingPointSubtracction(number1, number2);
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

char *longFloatingPointSubtracction(char *minuend, char *subtrahend)
{
	unsigned long long minuendLength, subtrahendLength, resultLength, minuend_digit_count; 
	unsigned long long subtrahend_digit_count, total_decimal_digits, total_integer_digits;
	char *result, *minuend_digit_buffer, *subtrahend_digit_buffer, *point_ptr, *decimal_result, *integer_result;
	bool carry = false; 

	/*Subtraction parts: 	9878-> minuend
				  98-> subtrahend*/
	
	//Error handling
	if( minuend == NULL || subtrahend == NULL )
		return NULL;
	minuendLength = strlen(minuend);

	subtrahendLength = strlen(subtrahend);
	
	//Error handling
	if( minuendLength == 0 && subtrahendLength == 0 )
		return NULL;
	if( minuendLength == 0 )
		return subtrahend;
	if( subtrahendLength == 0 )
		return minuend;

	
	resultLength = (minuendLength >= subtrahendLength) ? minuendLength : subtrahendLength;
	resultLength++; //The result could always need one extra byte for the sign.
	
	//OPERATIONS FOR THE DECIMAL PART:
	minuend_digit_count = count_decimal_digits(minuend);
	subtrahend_digit_count = count_decimal_digits(subtrahend);
	
	//get the greater digit count 
	total_decimal_digits = (minuend_digit_count >= subtrahend_digit_count) ? minuend_digit_count : subtrahend_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter
	minuend_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );
	subtrahend_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );

	//here: 9 + 9999 turns into -> 9000 + 9999
	memset(minuend_digit_buffer, '0', total_decimal_digits);
	memset(subtrahend_digit_buffer, '0', total_decimal_digits);
	minuend_digit_buffer[total_decimal_digits] = subtrahend_digit_buffer[total_decimal_digits] = '\0';

	//copying only the decimal part to the buffers
	point_ptr = strchr(minuend, '.');
	memcpy(minuend_digit_buffer, (point_ptr+1), minuend_digit_count);
	point_ptr = strchr(subtrahend, '.');
	memcpy(subtrahend_digit_buffer, (point_ptr+1), subtrahend_digit_count);

	//performing a normal longAddition
	decimal_result = longSubtraction(minuend_digit_buffer, subtrahend_digit_buffer);

	
	//check if there is a carry to the integer part
	if(strchr(decimal_result, '-') != NULL)
		carry = true;

	//We clean the buffer
	memset(minuend_digit_buffer, '\0', total_decimal_digits);
	memset(subtrahend_digit_buffer, '\0', total_decimal_digits);

	//OPERATIONS FOR THE INTEGER PART:
	minuend_digit_count = count_integer_digits(minuend);
	subtrahend_digit_count = count_integer_digits(subtrahend);
	
	//get the greater digit count 
	total_integer_digits = (minuend_digit_count >= subtrahend_digit_count) ? minuend_digit_count : subtrahend_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter
	minuend_digit_buffer = realloc(minuend_digit_buffer, (total_integer_digits+1)*sizeof(char) );
	subtrahend_digit_buffer = realloc(subtrahend_digit_buffer, (total_integer_digits+1)*sizeof(char) );
	
	//this time we don't need to set to '0', we just copy the integer part
	memcpy(minuend_digit_buffer, minuend, minuend_digit_count);
	memcpy(subtrahend_digit_buffer, subtrahend, subtrahend_digit_count);
	
	//getting the sum for the integer part of each summands
	integer_result = longSubtraction(minuend_digit_buffer, subtrahend_digit_buffer);
	
	//if there was a carry, we increment the integer part and then 10021 turns into -> 0021
	if(carry)
	{
		integer_result = longSubtraction(integer_result, "1");
		point_ptr = strchr(decimal_result, '-');
		point_ptr[0] = '0';
		while(decimal_result[0] == '0' && decimal_result[1] != '\0')// ignoring left-zeros, e.g 0390
			decimal_result++;

		minuend_digit_buffer = realloc(minuend_digit_buffer, (total_decimal_digits+2)*sizeof(char) );//'\0' and '1'
		memset(minuend_digit_buffer, '0', total_decimal_digits+1);
		minuend_digit_buffer[0] = '1';

		decimal_result = longSubtraction(minuend_digit_buffer, decimal_result);
		while(decimal_result[0] == '0' && decimal_result[1] != '\0')// ignoring left-zeros, e.g 0390
			decimal_result++;

	}

	result = calloc(resultLength+1,sizeof(char));
	result[resultLength] = '\0';
	
	//Copyin the integer part, concatenate a point and then the decimal part.
	total_integer_digits = strlen(integer_result);
	memcpy(result, integer_result, total_integer_digits);
	strcat(result, ".");
	memcpy((result+total_integer_digits+1), decimal_result, strlen(decimal_result));

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
	
		result[ resultLength+sign ] = (char)( subtraction + '0' ) ;

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
			if( isdigit( c[0] ) )
				primeSlice[counter++] = c[0];

	primeSlice[SLICELENGTH] = '\0';
	fclose(bigNum);

	return primeSlice;
}
