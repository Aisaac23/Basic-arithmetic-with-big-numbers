#include "bignums.h"

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
char* longAddition( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultLength, shortest, carry = 0;
	char *result, *zeros_ptr; 
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
	//this is done to perform the addition in place so we don't have to generate two extra summands of equal space.
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
	
	zeros_ptr = result;
	while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
		zeros_ptr++;
	
	resultLength = strlen(zeros_ptr);
	memmove(result, zeros_ptr, resultLength+1);
	
	result = realloc(result, (resultLength+1)*sizeof(char)  );
	
	return result;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
char *longSubtraction(char *minuend, char *subtrahend)
{
	/*Subtraction parts: 	9878-> minuend
				  98-> subtrahend*/
	unsigned long long int minuendLength, subtrahendLength, resultLength, shortest;
	int subtraction, carry = 0, sign = 0;
	char *result, *zeros_ptr;
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

	zeros_ptr = result;
	while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
		zeros_ptr++;
	
	resultLength = strlen(zeros_ptr);
	memmove(result, zeros_ptr, resultLength+1);
	
	result = realloc(result, (resultLength+1)*sizeof(char)  );

	return result;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
char* longMultiplication( char* factor1,  char* factor2)
{	
	//Error handling
	if( factor1 == NULL || factor2 == NULL )
		return NULL;

	unsigned long long f1Size, f2Size, resultLength;
	unsigned long long longest, shortest, resultIndex, units;
	unsigned int product = 0, prevCarry = 0, sumCarry = 0, carry = 0;
	char *result, *zeros_ptr;
	bool factor1_is_greater; 
	
	f1Size = strlen(factor1);
	f2Size = strlen(factor2);
	
	//Error handling
	if( f1Size == 0 || f2Size == 0 )
		return NULL;
	
	resultLength = f1Size + f2Size;// 99x999=98,901 -> 2+3 = 5 digits long
	
	result = calloc( resultLength+1, sizeof(char) );
	memset(result, '0', resultLength);
	result[resultLength] = '\0';
	
	//Picking the shortest and longest number in length of characters
	longest = f1Size >= f2Size ? f1Size : f2Size ;
	shortest = f2Size <= f1Size ? f2Size : f1Size ;
	units = 0;

	factor1_is_greater = (f1Size >= f2Size) ? true : false;
	/*We move from right to left in three different forms:
		1. By the resultIndex, placing the resulting digits on their respective place.
		2. By the shortes of the factors, multiplying each of its digits by all of the longests
		3. By the longests of the factors*/
	do
	{
		shortest--;
		resultIndex = (resultLength-1) - units;		
		do
		{
			// we get the product of the multiplication of two factors and its carry
			longest--;
			if( factor1_is_greater )
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

	zeros_ptr = result;
	while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
		zeros_ptr++;
	
	resultLength = strlen(zeros_ptr);
	memmove(result, zeros_ptr, resultLength+1);
	
	result = realloc(result, (resultLength+1)*sizeof(char) );

	return result;
}

/*Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division. For this specific function we lose the reminder, to keep the reminder on the dividend use longDivisionWithReminder. The dividend should alwyas be >= the divisor otherwise the result will be zero.

dependent of:
longSubtraction
longAddition
compareUnsignedIntegers
increment

*/
char *longDivision(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, tensMultiply_len;
	char *cuotient = NULL, *newDividend = NULL, *tensMultiply = NULL, *newDivisor = NULL;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return NULL;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		return "NaN";
	
	cuotient = calloc( dividendLength+1, sizeof(char) );//divisor min value is 1, so cuotient'd be the same length as dividend.
	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
		return cuotient;
	
	memset(cuotient, '0', dividendLength);
	cuotient[dividendLength] = '\0';
	strcpy(newDividend, dividend);
	newDividend[dividendLength] = '\0';

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		newDividend_Length = strlen(newDividend);

		if( strlen(newDividend) > divisorLength )
		{
			//newDividend_Length = strlen(newDividend);
			lengthDiff = newDividend_Length - 1;
			
			//Initializing the new divisor: 80,000
			newDivisor = realloc( newDivisor, (lengthDiff+1) * sizeof(char) ); //to increment the amount of divisor if needed.
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*1,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;//e.g: 4 [8000] - 1 [8] + 1[1]
			tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );// plus one: '\0'
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';
			
			//we add how many times divisor fits in newDividend and then we subtract from it.
			cuotient = longAddition(cuotient, tensMultiply);// "longAddition" is the second function that this one depends on.
			newDividend = longSubtraction( newDividend, newDivisor );// "longSubtraction" is the third
		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
		{
			increment(cuotient);//// "increment" is the fourth
			newDividend = longSubtraction( newDividend, divisor );
		}

	}

	free(newDividend);

	return cuotient;
}

/*Receives as arguments, two unsigned integers (as strings) and returns a string with the reminder of the division.

dependent of:
longSubtraction
compareUnsignedIntegers
*/
char *longModule(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, tensMultiply_len;
	char *newDividend = NULL, *tensMultiply = NULL, *newDivisor = NULL, *zeros_ptr;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return NULL;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		return "NaN";
	
	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
		return dividend;
	
	strcpy(newDividend, dividend);

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		zeros_ptr = newDividend;
		while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
			zeros_ptr++;
		
		newDividend_Length = strlen(zeros_ptr);
		memmove(newDividend, zeros_ptr, newDividend_Length+1);
		
		newDividend = realloc(newDividend, newDividend_Length+1 );

		if( strlen(newDividend) > divisorLength )
		{
			newDividend_Length = strlen(newDividend);
			lengthDiff = newDividend_Length - 1;
			
			//Initializing the new divisor: 80,000
			newDivisor = realloc( newDivisor, (lengthDiff+1) * sizeof(char) ); //to increment the amount of divisor if needed.
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*1,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;//e.g: 4 [8000] - 1 [8] + 1[1]
			tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );// plus one: '\0'
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';
			
			//we add how many times divisor fits in newDividend and then we subtract from it.
			newDividend = longSubtraction( newDividend, newDivisor );// "longSubtraction" is the third
		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
		{
			newDividend = longSubtraction( newDividend, divisor );
		}
	}

	return newDividend;
}

/*Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division, additionally stores the reminder on the pointer passed as dividend.
dependent of:
longSubtraction
longAddition
compareUnsignedIntegers
increment

*/
char *longDivisionWithReminder(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, tensMultiply_len;
	char *cuotient = NULL, *newDividend = NULL, *tensMultiply = NULL, *newDivisor = NULL, *zeros_ptr;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return NULL;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		return "NaN";
	
	cuotient = calloc( dividendLength+1, sizeof(char) );//divisor min value is 1, so cuotient'd be the same length as dividend.
	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
		return cuotient;
	
	//memset(cuotient, '0', dividendLength);
	//cuotient[dividendLength] = '\0';
	strcpy(newDividend, dividend);
	newDividend[dividendLength] = '\0';

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		newDividend_Length = strlen(newDividend);

		if( strlen(newDividend) > divisorLength )
		{
			//newDividend_Length = strlen(newDividend);
			lengthDiff = newDividend_Length - 1;
			
			//Initializing the new divisor: 80,000
			newDivisor = realloc( newDivisor, (lengthDiff+1) * sizeof(char) ); //to increment the amount of divisor if needed.
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*1,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;//e.g: 4 [8000] - 1 [8] + 1[1]
			tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );// plus one: '\0'
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';
			
			//we add how many times divisor fits in newDividend and then we subtract from it.
			cuotient = longAddition(cuotient, tensMultiply);// "longAddition" is the second function that this one depends on.
			newDividend = longSubtraction( newDividend, newDivisor );// "longSubtraction" is the third
		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
		{
			increment(cuotient);//// "increment" is the fourth
			newDividend = longSubtraction( newDividend, divisor );
		}


		zeros_ptr = cuotient;
		while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
			zeros_ptr++;
		
		dividendLength = strlen(zeros_ptr);
		memmove(cuotient, zeros_ptr, dividendLength+1);
		
		cuotient = realloc(cuotient, (dividendLength+1)*sizeof(char) );

	}
	newDividend_Length = strlen(newDividend);


	dividend[newDividend_Length] = '\0';

	strcpy(dividend, newDividend);


	return cuotient;
}

/*Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division using a FLOATING POINT precision.
dependent of:

longDivisionWithReminder

*/
char *longDivisionFloatingPointResult(char *dividend, char *divisor, unsigned int precision)
{
	unsigned long long dividendLength, integer_part_len, resultLength;
	char *localDividend, *result, *temp_result;
	
	dividendLength = strlen(dividend);

	localDividend = calloc(dividendLength + precision + 2, sizeof(char) );// one for the '\0' and one for the '.'
	result = calloc(dividendLength + precision + 2, sizeof(char) );

	result[0] = localDividend[dividendLength] = '\0'; 

	strcpy(localDividend, dividend);
	temp_result = longDivisionWithReminder(localDividend, divisor);//getting the integer part

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

/*Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the addition.

Function dependent:
longAddition
count_decimal_digits
count_integer_digits
increment

*/
char *longFloatingPointAddition(char *summand1, char *summand2)
{
	unsigned long long summand1Length, summand2Length, resultLength; 
	unsigned long long summand1_digit_count, summand2_digit_count, total_digits, decimal_result_len;
	char *result, *summand1_digit_buffer, *summand2_digit_buffer, *point_ptr, *decimal_result, *integer_result, *zeros_ptr;
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
	decimal_result_len = strlen(decimal_result);

	//check if there is a carry to the integer part
	if(decimal_result_len > total_digits)
		carry = true;
	else if(decimal_result_len < total_digits)
	{
		decimal_result = realloc(decimal_result, (total_digits+1)*sizeof(char) );
		zeros_ptr = strchr(decimal_result, '\0');
		memmove((zeros_ptr-decimal_result_len), decimal_result, decimal_result_len);
		memset(decimal_result, '0', total_digits-decimal_result_len);
	}

	//OPERATIONS FOR THE INTEGER PART:
	summand1_digit_count = count_integer_digits(summand1);
	summand2_digit_count = count_integer_digits(summand2);

	//get the greater digit count 
	total_digits = (summand1_digit_count >= summand2_digit_count) ? summand1_digit_count : summand2_digit_count;
	
	//initialize two arrays of characters with the same size to add them up latter, reusing the summandx_digit_buffer pointers
	summand1_digit_buffer = realloc(summand1_digit_buffer, (total_digits+1)*sizeof(char) );
	summand2_digit_buffer = realloc(summand2_digit_buffer, (total_digits+1)*sizeof(char) );
	
	//this time we don't need to set to '0', we just copy the integer part
	memcpy(summand1_digit_buffer, summand1, summand1_digit_count);
	memcpy(summand2_digit_buffer, summand2, summand2_digit_count);
	summand1_digit_buffer[summand1_digit_count] = '\0';
	summand2_digit_buffer[summand2_digit_count] = '\0';
	//getting the sum for the integer part of each summands

	integer_result = longAddition(summand1_digit_buffer, summand2_digit_buffer);

	//if there was a carry, we increment the integer part and then 10021 turns into -> 0021
	if(carry)
	{
		increment(integer_result);
		memmove(decimal_result, (decimal_result+1), strlen(decimal_result));
	}

	result = calloc(resultLength+1,sizeof(char));
	result[resultLength] = '\0';
	
	//Copyin the integer part, concatenate a point and then the decimal part.
	total_digits = strlen(integer_result);
	decimal_result_len = strlen(decimal_result);

	memcpy(result, integer_result, total_digits);
	strcat(result, ".");
	memcpy((result+total_digits+1), decimal_result, decimal_result_len );

	free(decimal_result);
	free(integer_result);
	free(summand1_digit_buffer);
	free(summand2_digit_buffer);
	
	return result;
}

/*Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the subtraction.

Function dependent:
longSubtraction
count_decimal_digits
count_integer_digits

*/

char *longFloatingPointSubtraction(char *minuend, char *subtrahend)
{
	unsigned long long minuendLength, subtrahendLength, resultLength, minuend_digit_count; 
	unsigned long long subtrahend_digit_count, total_decimal_digits, total_integer_digits, decimal_result_len;
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
	
	//initialize two arrays of characters with the same size
	minuend_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );
	subtrahend_digit_buffer = calloc( total_decimal_digits+1, sizeof(char) );

	//here: 9 - 9999 turns into -> 9000 - 9999
	memset(minuend_digit_buffer, '0', total_decimal_digits);
	memset(subtrahend_digit_buffer, '0', total_decimal_digits);
	minuend_digit_buffer[total_decimal_digits] = subtrahend_digit_buffer[total_decimal_digits] = '\0';


	//copying only the decimal part to the buffers
	point_ptr = strchr(minuend, '.');
	memcpy(minuend_digit_buffer, (point_ptr+1), minuend_digit_count);
	point_ptr = strchr(subtrahend, '.');
	memcpy(subtrahend_digit_buffer, (point_ptr+1), subtrahend_digit_count);

	//performing a normal longSubtraction
	decimal_result = longSubtraction(minuend_digit_buffer, subtrahend_digit_buffer);
	decimal_result_len = strlen(decimal_result);

	//check if there is a negative carry to the integer part
	if(strchr(decimal_result, '-') != NULL)
		carry = true;


	//OPERATIONS FOR THE INTEGER PART:
	minuend_digit_count = count_integer_digits(minuend);
	subtrahend_digit_count = count_integer_digits(subtrahend);
	
	//get the greater digit count 
	total_integer_digits = (minuend_digit_count >= subtrahend_digit_count) ? minuend_digit_count : subtrahend_digit_count;
	
	//initialize two arrays of characters with the same size
	minuend_digit_buffer = realloc(minuend_digit_buffer, (total_integer_digits+1)*sizeof(char) );
	subtrahend_digit_buffer = realloc(subtrahend_digit_buffer, (total_integer_digits+1)*sizeof(char) );
	
	//this time we don't need to set to '0', we just copy the integer part
	memcpy(minuend_digit_buffer, minuend, minuend_digit_count);
	memcpy(subtrahend_digit_buffer, subtrahend, subtrahend_digit_count);
	minuend_digit_buffer[minuend_digit_count] = subtrahend_digit_buffer[subtrahend_digit_count] = '\0';
	
	//getting the difference for the integer part
	integer_result = longSubtraction(minuend_digit_buffer, subtrahend_digit_buffer);


	//if there was a negative carry:
	if(carry)
	{
		//we eliminate the minus sign from the decimal part. From longSubtraction we know the sign is for sure in the first byte
		memmove(decimal_result, decimal_result+1,  decimal_result_len );

		if(strchr(integer_result, '-') == NULL)// if the integer_result was POSITIVE:
		{
			integer_result = longSubtraction(integer_result, "1"); // we subtract 1 from the integer result

			//we create a "unit" based on the longest decimal part, so: for .8934 - .99999 we create: 100000
			minuend_digit_buffer = realloc(minuend_digit_buffer, (total_decimal_digits+2)*sizeof(char) );//'\0' and '1'
			memset(minuend_digit_buffer, '0', total_decimal_digits+1);
			minuend_digit_buffer[0] = '1';
			minuend_digit_buffer[total_decimal_digits+1] = '\0';

			// we subtract our decimal_result from the "unit"
			decimal_result = longSubtraction(minuend_digit_buffer, decimal_result);
			memmove(decimal_result, decimal_result+1,  decimal_result_len );//we eliminate one extra zero from the beginning
			decimal_result_len = strlen(decimal_result); 
		}

		// if the integer_result was NEGATIVE, we just need to eliminate the minus sign and concatenate.
	}
	//if there was no carry, which means the decimal part resulted to be positive, we just proceed to concatenate:
	total_integer_digits = strlen(integer_result);
	result = calloc(total_integer_digits+decimal_result_len+2,sizeof(char));//+1 -> '\0' +1 -> '.'
	result[resultLength+1] = '\0';
	
	//Copyin the integer part, concatenate a point and then the decimal part.

	memcpy(result, integer_result, total_integer_digits);
	strcat(result, ".");

	memcpy((result+total_integer_digits+1), decimal_result, decimal_result_len);

	//we free the memory allocated in the pointers no longer used.
	free(minuend_digit_buffer); 
	free(subtrahend_digit_buffer); 
	free(decimal_result); 
	free(integer_result);

	return result;
}

/*Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the division.

Function dependent:
longDivisionFloatingPointResult
count_decimal_digits
count_integer_digits

*/
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

	total_decimal_digits = dividend_digit_count >= divisor_digit_count ? dividend_digit_count : divisor_digit_count;

	point_ptr = strchr(dividend_digit_buffer, '\0');
	point_ptr--;
	while(point_ptr[0] == '0')
		point_ptr--;
	point_ptr++;
	point_ptr[0] = '\0';

	point_ptr = strchr(divisor_digit_buffer, '\0');
	point_ptr--;
	while(point_ptr[0] == '0')
		point_ptr--;
	point_ptr++;
	point_ptr[0] = '\0';

	point_ptr = strchr(dividend_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), total_decimal_digits+1);
	

	point_ptr = strchr(divisor_digit_buffer, '.');
	memmove(point_ptr, (point_ptr+1), total_decimal_digits+1);


	printf("dividend_digit_buffer: %s, divisor_digit_buffer:%s\n", dividend_digit_buffer, divisor_digit_buffer);
	result = longDivisionFloatingPointResult(dividend_digit_buffer, divisor_digit_buffer, total_decimal_digits);
	
	return result;
}

/*Receives as arguments, two unsigned float-type numbers (as strings) and returns a string with the result of the multiplication.

Function dependent:
longMultiplication
count_decimal_digits

*/
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


/*Receives as arguments, the name of an existing file and an integer with the amount of digits to extract from the file. It returns a string with all the digits that could be read from the file until "SLICELENGTH" or EOF*/
char *readBigNumber(char *fileName, const unsigned long long SLICELENGTH)
{
	
	FILE *bigNum = fopen(fileName, "r");
	char c[2], *primeSlice;
	unsigned long long counter = 0;
	
	if(bigNum == NULL)
		return NULL;
	
	primeSlice = calloc(SLICELENGTH+1, sizeof(char));
	
	while( (counter < SLICELENGTH) && ( ( c[0]=fgetc(bigNum) )!= EOF ) )
			if( isdigit(c[0]) )
				primeSlice[counter++] = c[0];
	
	primeSlice[SLICELENGTH] = '\0';
	fclose(bigNum);

	return primeSlice;
}

//Counts the digits AFTER the point in a string that contains a number like: 10000.2243423
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

//Counts the digits BEFORE the point in a string that contains a number like: 10000.2243423
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


//Recieves an string composed only by digits and increments the number that represents in one. It can handle overflow (e.g. 999+1)
void increment(char* numberPlusPlus)
{
	unsigned long long index, len;
	bool added = false;
	if(numberPlusPlus != NULL)
	{
		index = len = strlen(numberPlusPlus);

		if(len == 0)
			return;	

		if(numberPlusPlus[len-1] < '9')
			numberPlusPlus[len-1]++;
		else
		{
			do
			{
				index--;
				if(numberPlusPlus[index] < '9')
				{
					numberPlusPlus[index]++;
					added = true;
				}
				else
					numberPlusPlus[index] = '0';

			}while( index > 0 && !added );
		
			if(!added)
			{
				numberPlusPlus = realloc( numberPlusPlus, len+2 ); // for '1' and '\0' which len does not consider.
				memmove(numberPlusPlus+1, numberPlusPlus, len+1); //all the characters plus '\0'
				numberPlusPlus[0] = '1';
			}
		}
	}
}

//Recieves an string composed only by digits and decrements the number that represents by one. It won't go lower than zero.
void decrement(char* numberMM)
{
	if(numberMM != NULL)
	{
		unsigned long long index, len;
		bool sub = false;
		
		while(numberMM[0] == '0' && numberMM[1] != '\0')// ignoring left-zeros, e.g 0390
			numberMM++;
		
		index = len = strlen(numberMM);
		if(len == 0)
			return;
		do
		{
			index--;
			if(numberMM[index] > '0')
			{
				numberMM[index]--;
				sub = true;
			}
			else if( index > 0)
				numberMM[index] = '9';

		}while( index > 0 && !sub );
	}
	return; 
}

/*It allows you to divide a number (as string) into equally sized groups (e.g. 19,000,000,000,000). It receives the string to be formated, the size of each group and the char that will serve as separator. Note: to use the new line char, you need to pass 10 instead of \n. */
char* formatNumber(char *n, int slice, char separator)
{
	if(n == NULL || slice == 0 || separator > 127)
		return n;

	char *newNumber, sep[2];
	unsigned long long nLength = strlen(n);
	if(nLength > slice)
	{
		unsigned long long index, groups, newLength, skip;
		
		newLength = nLength%slice == 0 ? nLength + nLength/slice - 1 : nLength + nLength/slice;
		groups = nLength%slice == 0 ? nLength/slice-1: nLength/slice;
		skip = nLength%slice == 0 ? slice: nLength%slice;
		index = 0;

		newNumber = calloc( newLength+1, sizeof(char) );

		strncat( newNumber, n, skip*sizeof(char) );
		sep[0] = separator;
		sep[1] = '\0';
		while( index < groups )
		{
			strcat( newNumber, sep);
			strncat( newNumber, n+skip+index*slice, slice*sizeof(char) );
			index++;
		}

		return newNumber;
	}	
	return n;
}

//Compares two unsigned integers and returns 1 if n1 is greater, -1 if it's lower and 0 if they're equal.
int compareUnsignedIntegers(char* n1, char *n2)
{
	char *n1_zero_ptr, *n2_zero_ptr;
	unsigned long long n1Length, n2Length;
	
	n1_zero_ptr = n1;
	while(n1_zero_ptr[0] == '0' && n1_zero_ptr[1] != '\0' )
		n1_zero_ptr++;
	
	n2_zero_ptr = n2;
	while(n2_zero_ptr[0] == '0' && n2_zero_ptr[1] != '\0' )
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

//Checks whether passed argument is an UNSIGNED integer number
bool isUnsignedInteger(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
 	if(len == 0)
		return false;
	
	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
			return false;
	return true;
}

//Checks whether passed argument is a SIGNED integer number
bool isSignedInteger(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool sign;

 	if(len == 0)
		return false;

	sign = ( strchr(number, '+') != NULL || strchr(number, '-') != NULL) ? true : false;
	for( int index = 1; index < len; index++ )
		if( !isdigit(number[index]) )
				return false;
	
	return sign;
}

//First checks whether passed argument is a valid number and then checks if it's a UNSIGNED float number
bool isUnsignedFloat(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool point = false;


 	if(len == 0)
		return false;

	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
		{
			if( !point && number[index] == '.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}

	return point;
}

//First checks whether passed argument is a valid number and then checks if it's a SIGNED float number
bool isSignedFloat(char* number)
{
	if(number == NULL)
		return false;

	while(number[0] == '0' && number[1] != '\0' )
		number++;

	int len = strlen(number);
	bool sign, point = false;

 	if(len == 0)
		return false;
 
	sign = ( strchr(number, '+') != NULL || strchr(number, '-') != NULL) ? true : false;
	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
		{
			if( !point && number[index] == '.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}

	return point && sign;
}
