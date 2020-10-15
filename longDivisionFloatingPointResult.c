#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

#include "chkops.h"



char *longDivisionWithReminder(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);
int compareUnsignedIntegers(char* n1, char *n2);
char *increment(char* numberPlusPlus);
char *longSubtraction(char *minuend, char *subtrahend);
char* longAddition( char* summand1,  char* summand2);
char *longDivisionFloatingPointResult(char *dividend, char divisor[], unsigned int precision);

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 && isUnsignedInteger(argv[1]) && isUnsignedInteger(argv[2]) )
	{
		number1 = argv[1];
		result = longDivisionFloatingPointResult(number1, argv[2], 7);
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
		result = longDivisionWithReminder(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("Cuotient: %s\n", result);
	if( result != NULL )
		free(result);

	return EXIT_SUCCESS;
}

char *longDivisionFloatingPointResult(char *dividend, char *divisor, unsigned int precision)
{
	unsigned long long dividendLength, integer_part_len, resultLength;
	char *localDividend, *result, *temp_result;
	
	dividendLength = strlen(dividend);

	localDividend = calloc(dividendLength + precision + 2, sizeof(char) );// one for the '\0' and one for the '.'
	result = calloc(dividendLength + precision + 2, sizeof(char) );

	result[0] = localDividend[dividendLength + precision + 1] = '\0'; 

	strcpy(localDividend, dividend);
	temp_result = longDivisionWithReminder(localDividend, divisor);

	while(temp_result[0] == '0' && temp_result[1] != '\0')// ignoring left-zeros, e.g 0390
		temp_result++;
	integer_part_len = strlen(temp_result);

	if( strcmp(localDividend, "0") != 0 )// localDividend will be the reminder
	{
		for(unsigned long long decimals = 0; decimals < precision; decimals++)
		{
			strcat(result, temp_result);
			strcat(localDividend, "0");

			temp_result = longDivisionWithReminder(localDividend, divisor);
		}
		strcat(result, temp_result);
	}
	else
		strcpy(result, temp_result);
	
	resultLength = strlen(result);
	memmove((result+integer_part_len+1), (result+integer_part_len), (resultLength-integer_part_len+2));
	result[integer_part_len] = '.';

	if(result[integer_part_len+1] == '\0')
		strcat(result, "0");

	return result;
}

char *longDivisionWithReminder(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;

	//With this one I reused the longSubtraction's function code and made some modifications.  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length;
	char *cuotient = NULL, *newDividend = NULL, *tensMultiply = NULL, *newDivisor = NULL;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return NULL;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)
		return "NaN";
	
	cuotient = calloc( dividendLength+1, sizeof(char) );//divisor min value (aside from zero) is 1, so cuotient'd be the same length as dividend.
	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
		return cuotient;
	
	memset(cuotient, '0', dividendLength);
	cuotient[dividendLength] = '\0';
	strcpy(newDividend, dividend);

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		while(newDividend[0] == '0')// ignoring left-zeros, e.g 0390
			newDividend++;

		if( strlen(newDividend)-1 > divisorLength )
		{
			newDividend_Length = strlen(newDividend);
			lengthDiff = newDividend_Length - 1;
			
			//Initializing the new divisor: 80,000
			newDivisor = realloc( newDivisor, (lengthDiff+1) * sizeof(char) ); //to increment the amount of divisor if needed.
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*10,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;
			tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';
			
			cuotient = longAddition(cuotient, tensMultiply);
			newDividend = longSubtraction( newDividend, newDivisor ) );
		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions.
		{
			cuotient = increment(cuotient);
			newDividend = longSubtraction( newDividend, divisor );
		}
		
		while(cuotient[0] == '0' && cuotient[1] != '\0')// ignoring left-zeros, e.g 0390
				cuotient++;
	}

	memset(dividend, '0', dividendLength);
	memcpy(dividend, newDividend, strlen(newDividend)+1 );
	
	return cuotient;
}

int compareUnsignedIntegers(char* n1, char *n2)
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

char *increment(char* numberPlusPlus)
{
	unsigned long long index, len;
	bool added = false;
	char *result; 
	if(numberPlusPlus != NULL)
	{
		index = len = strlen(numberPlusPlus);

		if(len == 0)
			return numberPlusPlus;	
		
		result = calloc(len+1, sizeof(char));
		strcpy(result, numberPlusPlus);
		result[len] = '\0';
		do
		{
			index--;
			if(result[index] < '9')
			{
				result[index]++;
				added = true;
			}
			else if( index >= 0)
				result[index] = '0';

		}while( index > 0 && !added );
		
		if(!added)
		{
			result = realloc( result, len+2 );
			memmove(result+1, result, len+1);
			result[0] = '1';
		}
	}

	return result; 
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
