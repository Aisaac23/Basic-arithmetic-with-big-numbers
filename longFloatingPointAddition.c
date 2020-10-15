#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

char *longFloatingPointAddition(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

char *increment(char* numberPlusPlus);

char* longAddition( char* summand1,  char* summand2);
unsigned long long count_decimal_digits(char *fp_number);
unsigned long long count_integer_digits(char *fp_number);


int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )
	{
		number1 = argv[1];
		result = longFloatingPointAddition(number1, argv[2]);
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
		result = longFloatingPointAddition(number1, number2);
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

char *longFloatingPointAddition(char *summand1, char *summand2)
{
	unsigned long long summand1Length, summand2Length, resultLength, summand1_digit_count, summand2_digit_count, total_digits;
	char *result, *summand1_digit_buffer, *summand2_digit_buffer, *point_ptr, *decimal_result, *integer_result;
	bool carry = false; 


	
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

	
	resultLength = (summand1Length >= summand2Length) ? summand1Length : summand2Length;
	resultLength++; //The result could always need one extra byte.
	
	//OPERATIONS FOR THE DECIMAL PART:
	summand1_digit_count = count_decimal_digits(summand1);
	summand2_digit_count = count_decimal_digits(summand2);
	
	//get the greater digit count 
	total_digits = (summand1_digit_count >= summand2_digit_count) ? summand1_digit_count : summand2_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter
	summand1_digit_buffer = calloc( total_digits+1, sizeof(char) );
	summand2_digit_buffer = calloc( total_digits+1, sizeof(char) );

	//here: 9 + 9999 turns into -> 9000 + 9999
	memset(summand1_digit_buffer, '0', total_digits);
	memset(summand2_digit_buffer, '0', total_digits);
	summand1_digit_buffer[total_digits] = summand2_digit_buffer[total_digits] = '\0';

	//copying only the decimal part to the buffers
	point_ptr = strchr(summand1, '.');
	memcpy(summand1_digit_buffer, (point_ptr+1), summand1_digit_count);
	point_ptr = strchr(summand2, '.');
	memcpy(summand2_digit_buffer, (point_ptr+1), summand2_digit_count);
	
	//performing a normal longAddition
	decimal_result = longAddition(summand1_digit_buffer, summand2_digit_buffer);

	//check if there is a carry to the integer part
	if(strlen(decimal_result) > total_digits)
		carry = true;

	//We clean the buffer
	memset(summand1_digit_buffer, '\0', total_digits);
	memset(summand2_digit_buffer, '\0', total_digits);

	//OPERATIONS FOR THE INTEGER PART:
	summand1_digit_count = count_integer_digits(summand1);
	summand2_digit_count = count_integer_digits(summand2);
	
	//get the greater digit count 
	total_digits = (summand1_digit_count >= summand2_digit_count) ? summand1_digit_count : summand2_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter
	summand1_digit_buffer = realloc(summand1_digit_buffer, (total_digits+1)*sizeof(char) );
	summand2_digit_buffer = realloc(summand2_digit_buffer, (total_digits+1)*sizeof(char) );
	
	//this time we don't need to set to '0', we just copy the integer part
	memcpy(summand1_digit_buffer, summand1, summand1_digit_count);
	memcpy(summand2_digit_buffer, summand2, summand2_digit_count);
	
	//getting the sum for the integer part of each summands
	integer_result = longAddition(summand1_digit_buffer, summand2_digit_buffer);
	
	//if there was a carry, we increment the integer part and then 10021 turns into -> 0021
	if(carry)
	{
		integer_result = increment(integer_result);
		memmove(decimal_result, (decimal_result+1), strlen(decimal_result));
	}

	result = calloc(resultLength+1,sizeof(char));
	result[resultLength] = '\0';
	
	//Copyin the integer part, concatenate a point and then the decimal part.
	total_digits = strlen(integer_result);
	memcpy(result, integer_result, total_digits);
	strcat(result, ".");
	memcpy((result+total_digits+1), decimal_result, strlen(decimal_result));

	free(decimal_result);
	free(integer_result);
	free(summand1_digit_buffer);
	free(summand2_digit_buffer);
	
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

char *increment(char* numberPlusPlus)
{
	char *n1_zero_ptr, *n2_zero_ptr;
	unsigned long long n1Length, n2Length;
	
	n1_zero_ptr = n1;
	while(n1_zero_ptr[0] == '0' && n1_zero_ptr[1] != '\0')
		n1_zero_ptr++;
	
	n2_zero_ptr = n2;
	while(n2_zero_ptr[0] == '0' && n2_zero_ptr[1] != '\0')
		n2_zero_ptr++;
	
	n1Length = strlen(n1_zero_ptr);
	n2Length = strlen(n2_zero_ptr);
	
	if(n1Length > n2Length)
		return 1;
	if(n1Length < n2Length)
		return -1;

	if( strcmp(n1_zero_ptr, n2_zero_ptr) == 0 )
		return 0;
	for( unsigned long long index = 0; index < n1Length; index++)
		if( n1_zero_ptr[index] < n2_zero_ptr[index] )
			return -1;
		else if( n1_zero_ptr[index] > n2_zero_ptr[index] )
			return 1;
	return 0;
}

char* longAddition( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultLength, shortest, carry = 0;
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

	//Pick the greater length from both summands
	resultLength = (summand1Length >= summand2Length) ? summand1Length+1 : summand2Length+1;//99+9=108 (max, one more digit)
	
	//Initializing the space for the result.
	result = calloc( resultLength+1, sizeof(char) );// plus one for the '\0'
	memset(result, '0', resultLength);
	result[resultLength] = '\0';
	
	//Picking the shortest length
	shortest = (summand1Length <= summand2Length) ? summand1Length : summand2Length;

	summand1IsShorter = (summand1Length <= summand2Length) ? true : false;

	do
	{
		resultLength--;
		if(shortest > 0)
		{
			shortest--;
			if( summand1IsShorter )
				sum = (summand1[shortest]-'0') + (summand2[resultLength-1]-'0');
			else
				sum = (summand1[resultLength-1]-'0') + (summand2[shortest]-'0');
		}
		else if ( resultLength >= 1 )// When shortest summand was added but there are digits left to add in the other one.
		{
			if( summand1IsShorter )
				sum = (summand2[resultLength-1]-'0');
			else
				sum = (summand1[resultLength-1]-'0');
		}
		else // When shortest and resultLength are ZERO, sum should be zero as well.
			sum = 0;
		
		sum+=carry;
		carry = (sum > 9) ? sum/10 : 0;
		sum = (sum > 9) ? sum%10 : sum;
		
		 
		result[resultLength] = (char)( sum + '0' );

	}while( resultLength > 0 );;//resultLength is unsigned so it'd cause a runtime error if it gets to -1
	
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
