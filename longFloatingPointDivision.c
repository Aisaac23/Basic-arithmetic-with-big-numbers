#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

char *longFloatingPointDivision(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);
unsigned long long count_decimal_digits(char *fp_number);

char *longDivisionWithReminder(char *dividend, char divisor[]);
int compareUnsignedIntegers(char* n1, char *n2);
char *increment(char* numberPlusPlus);
char *longSubtraction(char *minuend, char *subtrahend);
char* longAddition( char* summand1,  char* summand2);
char *longDivisionFloatingPointResult(char *dividend, char divisor[], unsigned int precision);


int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 )
	{
		number1 = argv[1];
		result = longFloatingPointDivision(number1, argv[2]);
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
		result = longFloatingPointDivision(number1, number2);
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

char *longFloatingPointDivision(char *dividend, char *divisor)
{
	unsigned long long dividendLength, divisorLength, dividend_digit_count, divisor_digit_count, total_decimal_digits;
	char *result, *dividend_digit_buffer, *divisor_digit_buffer, *point_ptr;
	
	//Error handling
	if( dividend == NULL || divisor == NULL )
		return NULL;
	dividendLength = strlen(dividend);

	divisorLength = strlen(divisor);
	
	//Error handling
	if( dividendLength == 0 && divisorLength == 0 )
		return NULL;
	if( dividendLength == 0 )
		return divisor;
	if( divisorLength == 0 )
		return dividend;

	dividend_digit_count = count_decimal_digits(dividend);
	divisor_digit_count = count_decimal_digits(divisor);
	//total_decimal_digits = divisor_digit_count + dividend_digit_count;
	total_decimal_digits = dividendLength >= divisorLength ? dividendLength-1 : divisorLength-1;
	
	//initialize two arrays of characters with the same size to add them up latter
	dividend_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );
	divisor_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );
	
	strncpy(dividend_digit_buffer, dividend, dividendLength);
	strncpy(divisor_digit_buffer, divisor, divisorLength);
	
	if(dividend_digit_count < divisor_digit_count)
		memset( (dividend_digit_buffer+dividendLength), '0', divisor_digit_count-dividend_digit_count);

	if(dividend_digit_count > divisor_digit_count)
		memset( (divisor_digit_buffer+divisorLength), '0', dividend_digit_count-divisor_digit_count);


	divisor_digit_buffer[total_decimal_digits+1] = dividend_digit_buffer[total_decimal_digits+1] = '\0';
	printf("*****%s, %s\n", dividend_digit_buffer, divisor_digit_buffer);
/*	memset(dividend_digit_buffer, '0', total_decimal_digits);*/
/*	memset(divisor_digit_buffer, '0', total_decimal_digits);*/
/*	*/

	total_decimal_digits = dividend_digit_count >= divisor_digit_count ? dividend_digit_count : divisor_digit_count;
/*	strncpy(dividend_digit_buffer, dividend, dividendLength);*/
	point_ptr = strchr(dividend_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), total_decimal_digits+1);
	
/*	strncpy(divisor_digit_buffer, divisor, divisorLength);*/
	point_ptr = strchr(divisor_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), total_decimal_digits+1);


	printf("%s, %s\n", dividend_digit_buffer, divisor_digit_buffer);
	result = longDivisionFloatingPointResult(dividend_digit_buffer, divisor_digit_buffer, total_decimal_digits);
	
/*	point_ptr = (result+strlen(result)-total_decimal_digits);*/
/*	memmove(point_ptr+1, point_ptr, total_decimal_digits);*/
/*	point_ptr[0] = '.';*/
	
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


char *longDivisionFloatingPointResult(char *dividend, char divisor[], unsigned int precision)
{
	unsigned long long dividendLength;
	char *localDividend, *result;
	
	dividendLength = strlen(dividend);

	localDividend = calloc(dividendLength + precision + 2, sizeof(char) );//
	result = calloc(dividendLength + precision + 2, sizeof(char) );

	result[0] = '\0'; 
	localDividend[dividendLength + precision] = '\0';

	strcpy(localDividend, dividend);
	char *temp;
	temp = longDivisionWithReminder(localDividend, divisor);

	if( strcmp(localDividend, "0") != 0 )
		for(unsigned int decimals = 0; decimals <= precision; decimals++)
		{
			strcat(result, temp);
			if( strchr(result, '.') == NULL)
				strcat(result, ".");

			if(decimals < precision)
				strcat(localDividend, "0");
			
			temp = longDivisionWithReminder(localDividend, divisor);
		}
	else
		strcpy(result, temp);

	if( strchr(result, '.') == NULL )
		strcat(result, ".0");

	return result;
}

char *longDivisionWithReminder(char *dividend, char divisor[])
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;
	
	//With this one I reused the longSubtraction's function code and made some modifications.  
	const unsigned long long dividendLength = strlen(dividend), divisorLength = strlen(divisor);
	char *cuotient, *z, *newDividend;

	z = calloc(2, sizeof(char));
	z[0] = '0';
	z[1] = '\0';
	
//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return NULL;

	if(divisor[0] == '0')
		return NULL;

	if( dividendLength < divisorLength || dividend[0] == '0')
		return z;
	else if( dividendLength == divisorLength )
		if( compareUnsignedIntegers(dividend, divisor) == -1 )
			return z;

	cuotient = calloc( dividendLength+1, sizeof(char) );//divisor min value (aside from zero) is 1, so cuotient'd be the same length as dividend.
	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend
	
	
	
	cuotient[0] = '0';
	cuotient[1] = '\0';

	strcpy(newDividend, dividend);
	
	while( compareUnsignedIntegers(newDividend, divisor) != -1 )
	{		
		if( strlen(newDividend) > divisorLength )
		{
			char *tensMultiply, *newDivisor;
			int lengthDiff;

			lengthDiff = strlen(newDividend) - strlen(divisor) - 1;

			tensMultiply = calloc( lengthDiff+1, sizeof(char) );
			newDivisor = calloc( strlen(newDividend)+1, sizeof(char) ); //to increment the amount of divisor if needed.

			tensMultiply[0] = '1';

			strcpy(newDivisor, divisor);
			
			for(int index = 0; index < lengthDiff; index++)
			{	
				strcat(newDivisor, "0");
				strcat(tensMultiply, "0");
			}

			cuotient = longAddition(cuotient, tensMultiply);
			newDividend = longSubtraction( newDividend, newDivisor );
			//printf("%s\n", newDividend);
		}
		else
		{
			cuotient = increment(cuotient);
			newDividend = longSubtraction( newDividend, divisor );
		}
	}
	strcpy(dividend, newDividend);
	
	//you could have spare zeros in the left, so we rotate
	while(dividend[0] == '0' && strlen(dividend) > 1)
		memmove(dividend, dividend+1, dividendLength*sizeof(char)+1 );
	while(cuotient[0] == '0' && strlen(cuotient) > 1)
		memmove(cuotient, cuotient+1, dividendLength*sizeof(char)+1 );

	return cuotient;
}

int compareUnsignedIntegers(char* n1, char *n2)
{
	int n1Len, n2Len;
	n1Len = strlen(n1);
	n2Len = strlen(n2);
	
	if(n1Len < n2Len)
		return -1;
	if(n1Len > n2Len)
		return 1;
	if( strcmp(n1, n2) == 0 )
		return 0;
	for( unsigned long long index = 0; index < n1Len; index++)
		if( n1[index] < n2[index] )
			return -1;
		else if( n1[index] > n2[index] )
			return 1;
	return -2;
}

char *increment(char* numberPlusPlus)
{
	unsigned long long index, len;
	bool added = false;
	char *result; 
	if(numberPlusPlus != NULL)
	{
		index = len = strlen(numberPlusPlus);
		for(int i = 0; i < len; i++)
			if(!isdigit(numberPlusPlus[i]))
				return numberPlusPlus;
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
			if( isdigit( c[0] ) )
				primeSlice[counter++] = c[0];

	primeSlice[SLICELENGTH] = '\0';
	fclose(bigNum);

	return primeSlice;
}
