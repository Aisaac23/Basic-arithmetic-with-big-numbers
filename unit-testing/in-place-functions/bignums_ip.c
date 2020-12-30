#include "bignums_ip.h"

void longAdditionInPlace( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultLength, shortest, carry = 0;
	unsigned int sum = 0;//biggest number for this variable will be 18.
	
	//Error handling
	if( summand1 == NULL || summand2 == NULL )
		return ;
	
	summand1Length = strlen(summand1);
	summand2Length = strlen(summand2);
	
	//Error handling
	if( summand1Length == 0 || summand2Length == 0 )
		return ;

	resultLength = summand1Length ;
	
	//Initializing the space for the result.
	char result[resultLength+1];
	result[resultLength] = '\0';
	
	//Picking the shortest length
	shortest = summand2Length;
	
	do
	{
		resultLength--;
		if(shortest > 0)
		{
			shortest--;
			sum = (summand1[resultLength]-'0') + (summand2[shortest]-'0');
		}
		else if ( resultLength >= 1 )// When shortest summand was added but there are digits left to add in the other one.
		{
			sum = (summand1[resultLength]-'0');
		}
		else // When shortest and resultLength are ZERO, sum should be zero as well.
			sum = 0;
		
		sum+=carry;
		carry = (sum > 9) ? sum/10 : 0;
		sum = (sum > 9) ? sum%10 : sum;
		
		 
		result[resultLength] = (char)( sum + '0' );

	}while( resultLength > 0 );;//resultLength is unsigned so it'd cause a runtime error if it gets to -1
	
	resultLength = strlen(result);
	
	memset(summand1, '0', summand1Length);
	strcpy(summand1, result);
	summand1[resultLength] = '\0';
	
	return;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
void longSubtractionInPlace(char *minuend, char *subtrahend)
{
	/*Subtraction parts: 	9878-> minuend
				  98-> subtrahend*/
	unsigned long long int minuendLength, subtrahendLength, resultLength, shortest;
	int subtraction, carry = 0, sign = 0;

	bool minuendIsLower = true;

	if( minuend == NULL || subtrahend == NULL )
		return;

	minuendLength = strlen(minuend); 
	subtrahendLength = strlen(subtrahend);
	resultLength = (minuendLength >= subtrahendLength) ? minuendLength : subtrahendLength;// 0 - 100 = [-]100
	
	//Error handling
	if( minuendLength == 0 && subtrahendLength == 0 )
		return ;
	if( subtrahendLength == 0 || minuendLength == 0)
		return ;
	
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
	char result[resultLength+1+sign];
	result[resultLength+sign] = '\0';

	if(minuendIsLower)
		result[0] = '-';
	shortest = (subtrahendLength <= minuendLength) ? subtrahendLength : minuendLength; // get the shortest out of both

	if( minuendIsLower && subtrahend[0] != '0' )
		return ;
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
	
	resultLength = strlen(result);
	if(minuendIsLower)
	{
		memset(subtrahend, '0', resultLength);
		strcpy(subtrahend, result);
		subtrahend[resultLength] = '\0';
	}
	else
	{
		memset(minuend, '0', resultLength);
		strcpy(minuend, result);
		minuend[resultLength] = '\0';
	}

	return;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
void longMultiplicationInPlace( char* factor1,  char* factor2)
{	
	//Error handling
	if( factor1 == NULL || factor2 == NULL )
		return;

	unsigned long long factor1Length, factor2Length, resultLength, cacheLength;
	unsigned long long longest, shortest, resultIndex, units;
	unsigned int product = 0, prevCarry = 0, sumCarry = 0, carry = 0;

	bool factor1_is_greater; 
	
	factor1Length = strlen(factor1);
	factor2Length = strlen(factor2);
	
	//Error handling
	if( factor1Length == 0 || factor2Length == 0 )
		return;
	
	resultLength = factor1Length + factor2Length;// 99x999=98,901 -> 2+3 = 5 digits long
	
	char result[resultLength+1];
	memset(result, '0', resultLength);
	result[resultLength] = '\0';

	if(factor1Length == factor2Length && factor1[0] != '0' && factor2[0] != '0' )
		return;
	else if( (factor1Length > factor2Length && factor1[0] != '0') || (factor2Length > factor1Length && factor2[0] != '0') )
		return;

	if(factor1Length == factor2Length)
	{
		cacheLength = factor2Length;
		units = 0;
		do
		{
			factor1Length--;
			resultIndex = (resultLength-1) - units;		
			do
			{
				factor2Length--;
				// we get the product of the multiplication of two factors and its carry
				product = (factor1[factor1Length]-'0') * (factor2[factor2Length]-'0');

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
			}while( factor2Length > 0 );

			result[resultIndex] = (char)(prevCarry + '0');
			prevCarry = 0;
			units++;
			factor2Length = cacheLength;

		}while( factor1Length > 0 );
	}
	else
	{
		//Picking the shortest and longest number in length of characters
		longest = factor1Length >= factor2Length ? factor1Length : factor2Length ;
		shortest = factor2Length <= factor1Length ? factor2Length : factor1Length ;

		cacheLength = longest;
		factor1_is_greater = (factor1Length >= factor2Length) ? true : false;
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
			longest = cacheLength ;

		}while( shortest >= 1 );
	}

	
	if(factor1Length == factor1Length + factor2Length || factor1Length == factor2Length)
	{
		memset(factor1, '0', resultLength);
		strcpy(factor1, result);
		factor1[resultLength] = '\0';
	}
	else
	{
		memset(factor2, '0', resultLength);
		strcpy(factor2, result);
		factor2[resultLength] = '\0';
	}

	return;
}

void longDivisionInPlace(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, tensMultiply_len, cuotientLength;
	char *zeros_ptr = NULL;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return ;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		strcpy(divisor, "NaN"), strcpy(dividend, "NaN") ;
	
	char cuotient[dividendLength+1], newDividend[dividendLength+1 +1 ];// '\0' and leading '0' 
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
	{
		strcpy(divisor, cuotient); return ;
	}

	memset(cuotient, '0', dividendLength);
	cuotient[dividendLength] = '\0';
	newDividend[0] = '0';
	strcpy(newDividend+1, dividend);
	newDividend[dividendLength+1] = '\0';

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		zeros_ptr = newDividend;
		while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
			zeros_ptr++;
		newDividend_Length = strlen(zeros_ptr);

		lengthDiff = newDividend_Length - 1;
		char newDivisor[lengthDiff+1];

		if( newDividend_Length > divisorLength )
		{
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*1,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;//e.g: 4 [8000] - 1 [8] + 1[1]
			//tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );// plus one: '\0'
			char tensMultiply[tensMultiply_len+1];
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';

			//we add how many times divisor fits in newDividend and then we subtract from it.
			longAdditionInPlace(cuotient, tensMultiply);// "longAddition" is the second function that this one depends on.
			longSubtractionInPlace( newDividend, newDivisor );// "longSubtraction" is the third

		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
		{
			incrementInPlace(cuotient);//// "increment" is the fourth
			longSubtractionInPlace( newDividend, divisor );
		}

	}

	cuotientLength = strlen(cuotient);

	dividend[cuotientLength] = '\0';
	strcpy(dividend, cuotient);

	return ;
}

void longModuleInPlace(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, cuotientLength;
	char *zeros_ptr = NULL;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return ;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		strcpy(divisor, "NaN"), strcpy(dividend, "NaN") ;
	
	char cuotient[dividendLength+1], newDividend[dividendLength+1 +1 ];// '\0' and leading '0' 
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
		strcpy(divisor, cuotient), strcpy(dividend, cuotient);
	
	memset(cuotient, '0', dividendLength);
	cuotient[dividendLength] = '\0';
	newDividend[0] = '0';
	strcpy(newDividend+1, dividend);
	newDividend[dividendLength+1] = '\0';

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )

	{	
		zeros_ptr = newDividend;
		while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
			zeros_ptr++;
		newDividend_Length = strlen(zeros_ptr);

		lengthDiff = newDividend_Length - 1;
		char newDivisor[lengthDiff+1];

		if( newDividend_Length > divisorLength )
		{
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//we add how many times divisor fits in newDividend and then we subtract from it.
			longSubtractionInPlace( newDividend, newDivisor );// "longSubtraction" is the third

		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
			longSubtractionInPlace( newDividend, newDivisor );
	}

	cuotientLength = strlen(newDividend);

	dividend[cuotientLength] = '\0';
	strcpy(dividend, newDividend);

	return ;
}

void longDivisionWithReminderInPlace(char *dividend, char *divisor)
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return;
	
	//This is the only one out of the four basic operations which is function-dependant  
	unsigned long long dividendLength, divisorLength, lengthDiff, newDividend_Length, tensMultiply_len, cuotientLength;
	char *zeros_ptr = NULL;
	
	dividendLength = strlen(dividend);
	divisorLength = strlen(divisor);
	
	//Error handling
	if( divisorLength == 0 || dividendLength == 0 )
		return ;
	
	if( compareUnsignedIntegers(divisor, "0") == 0)//"compareUnsignedIntegers" is the first function that this one depends on.
		strcpy(divisor, "NaN"), strcpy(dividend, "NaN") ;
	
	char cuotient[dividendLength+1], newDividend[dividendLength+1 +1 ];// '\0' and leading '0' 
	
	cuotient[0] = '0';
	cuotient[1] = '\0';
	
	if( compareUnsignedIntegers(dividend, "0") == 0)// when dividend is zero we return zero 
	{
		strcpy(divisor, cuotient); return ;
	}

	memset(cuotient, '0', dividendLength);
	cuotient[dividendLength] = '\0';
	newDividend[0] = '0';
	strcpy(newDividend+1, dividend);
	newDividend[dividendLength+1] = '\0';

	while( compareUnsignedIntegers(newDividend, divisor)  != -1 )
	{	
		zeros_ptr = newDividend;
		while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
			zeros_ptr++;
		newDividend_Length = strlen(zeros_ptr);

		lengthDiff = newDividend_Length - 1;
		char newDivisor[lengthDiff+1];

		if( newDividend_Length > divisorLength )
		{
			memset(newDivisor, '0', lengthDiff);
			newDivisor[lengthDiff] = '\0';
			strncpy(newDivisor, divisor, divisorLength);
			
			//Initializing the tens multiply so: 99999/8 turns into -> 99,999/(8*1,000)
			tensMultiply_len = strlen(newDivisor) - divisorLength + 1;//e.g: 4 [8000] - 1 [8] + 1[1]
			//tensMultiply = realloc( tensMultiply, (tensMultiply_len+1) * sizeof(char) );// plus one: '\0'
			char tensMultiply[tensMultiply_len+1];
			memset(tensMultiply, '0', tensMultiply_len);
			tensMultiply[tensMultiply_len] = '\0';
			tensMultiply[0] = '1';

			//we add how many times divisor fits in newDividend and then we subtract from it.
			longAdditionInPlace(cuotient, tensMultiply);// "longAddition" is the second function that this one depends on.
			longSubtractionInPlace( newDividend, newDivisor );// "longSubtraction" is the third

		}
		else // if the dividend and the divisor are the same length, then you only need up to 9 subtractions. E.g: 999/100 = 9, rem= 99
		{
			incrementInPlace(cuotient);//// "increment" is the fourth
			longSubtractionInPlace( newDividend, divisor );
		}

	}

	cuotientLength = strlen(cuotient);

	dividend[cuotientLength] = '\0';
	strcpy(dividend, cuotient);

	zeros_ptr = newDividend;
	while(zeros_ptr[0] == '0' && zeros_ptr[1] != '\0')// ignoring left-zeros, e.g 0390
		zeros_ptr++;

	strcpy(divisor, zeros_ptr );
	newDividend_Length = strlen(zeros_ptr);
	divisor[newDividend_Length] = '\0';

	return ;
}

void incrementInPlace(char* numberPlusPlus)
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
		}
	}
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

