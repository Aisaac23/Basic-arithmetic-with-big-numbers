#include "bignums.h"

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
char *longSubtraction(char *minuend, char *subtrahend)
{
	/*Subtraction parts: 	9878-> minuend
				  98-> subtrahend*/
	unsigned long long int minuendLength, subtrahendLength, resultLength, shortest, operation_index;
	int subtraction, carry = 0;
	char *result;
	bool minuendIsLower = true;

	if( minuend == NULL || subtrahend == NULL )
		return NULL;

	minuendLength = strlen(minuend); 
	subtrahendLength = strlen(subtrahend);
	resultLength = (minuendLength >= subtrahendLength) ? minuendLength+1 : subtrahendLength+1;// 0 - 100 = [-]100
	
	//Error handling
	if( minuendLength == 0 && subtrahendLength == 0 )
		return NULL;
	if( subtrahendLength == 0 )
		return minuend;
	if( minuendLength == 0 )
		return subtrahend;


	result = calloc( resultLength+1, sizeof(char) );// plus one: '\0'
	
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

	for(unsigned long long int i = 0; i<resultLength; i++)
			result[i] = '0';
	result[resultLength] = '\0';
	
	shortest = (subtrahendLength <= minuendLength) ? subtrahendLength : minuendLength; // get the shortest out of both
	operation_index = resultLength -1;// 
	do
	{
		operation_index--;
		if(shortest > 0)
		{
			shortest--;
			if( minuendIsLower )
			{	// if the next number is zero and we have carry == -1 then we set carry to 9.
				if( operation_index <= minuendLength -2 && carry==-1 ) // subtrahend-2 -> evaluate from the penultimate digit.
					carry = ( subtrahend[operation_index] == '0' ) ? 9 : -1;	
				subtraction = ( subtrahend[operation_index] - '0') - (minuend[shortest] - '0' ) + carry;
			}
			else
			{
				if( operation_index <= subtrahendLength -2 && carry==-1 )// subtrahend-2 -> evaluate from the penultimate digit.
					carry = ( minuend[operation_index] == '0') ? 9 : -1;	
				subtraction = ( minuend[operation_index] - '0') - ( subtrahend[shortest] - '0' ) + carry;
			}
			printf("****%i, carry: %i\n", subtraction, carry);
		}
		else if ( operation_index >= 0 )//This is used when one of the numbers has greater length than the other.
		{
			if( minuendIsLower )
				subtraction = (subtrahend[operation_index] - '0') + carry;
			else
				subtraction = (minuend[operation_index]-'0') + carry;
		}
		
		if( subtraction < 0 || carry == 9 )//if the result was negative or the carry had to be 9, then we set carry to -1 for the next digit
		{
			if( subtraction < 0)
				subtraction += 10;
			carry = -1;
		}
		else
			carry = 0;		
	
		result[ operation_index+1 ] = (char)( subtraction + '0' ) ;

	}while(operation_index > 0);//operation_index is unsigned so it'd cause a runtime error if it gets to -1
	
	printf("....%s\n", result);
	if( minuendIsLower ) //if the result was negative, we add the - sign.
	{
		operation_index = strlen(result);
		int zeros = 0;
		for(unsigned long long int i = 0; i<operation_index && result[i] == '0'; i++)
			if(result[i] == '0')
				zeros++;
		result[zeros-1]='-';	
	}

	return result;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
char* longMultiplication( char* factor1,  char* factor2)
{	
	//Error handling
	if( factor1 == NULL || factor2 == NULL )
		return NULL;

	unsigned long long f1Size = strlen(factor1), f2Size = strlen(factor2);
	unsigned long long resultSize = f1Size + f2Size;
	
	char *result; 
	unsigned int product = 0, prevCarry = 0, sumCarry = 0, carry = 0;
	
	//Error handling
	if( f1Size == 0 || f2Size == 0 )
		return NULL;

	result = calloc( resultSize+1, sizeof(char) );

	for(unsigned long long i = 0; i<resultSize; i++)
			result[i] = '0';
	result[resultSize] = '\0';
	
	//Picking the shortest number in length of characters
	unsigned long long longest, shortest, resultIndex, units;
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


char *longModule(char *dividend, char divisor[])
{
	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;
	
	//With this one I reused the longSubtraction's function code and made some modifications.  
	const unsigned long long dividendLength = strlen(dividend), divisorLength = strlen(divisor);
	char *z, *newDividend;

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

	newDividend = calloc( dividendLength+1, sizeof(char) ); // we need this one to change the value recieved as argument in dividend

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

			newDividend = longSubtraction( newDividend, newDivisor );

		}
		else
		{
			newDividend = longSubtraction( newDividend, divisor );
		}
	}

	return newDividend;
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

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division.
char *longDivision(char *dividend, char divisor[])
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

		}
		else
		{
			cuotient = increment(cuotient);
			newDividend = longSubtraction( newDividend, divisor );
		}
	}

	free(newDividend);

	return cuotient;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
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


	resultLength = (summand1Length >= summand2Length) ? summand1Length+1 : summand2Length+1;//99+9=108 (max, one more digit)
	
	result = calloc( resultLength, sizeof(char) );
	for(unsigned long long i = 0; i<resultLength; i++)
			result[i] = '0';
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
		
		// 
		result[resultLength] = (char)( sum + '0' );

	}while( resultLength > 0 );
	
	return result;
}


/*Receives as arguments, the name of an existing file and an integer with the amount of digits to extract from the file. It returns a string with all the digits that could be read from the file until "SLICELENGTH" or EOF*/
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH)
{
	
	FILE *bigNum = fopen(fileName, "r");
	char c[2], *primeSlice;
	unsigned int counter = 0;

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

//Recieves an string composed only by digits and increments the number that represents in one. It can handle overflow (e.g. 999+1)
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

//Recieves an string composed only by digits and decrements the number that represents in one. It won't go lower than zero.
void decrement(char* numberPlusPlus)
{
	if(numberPlusPlus != NULL)
	{
		unsigned long long index, len;
		bool sub = false;
		
		index = len = strlen(numberPlusPlus);
		for(int i = 0; i < len; i++)
			if(!isdigit(numberPlusPlus[i]))
				return;
		if(len == 0)
			return;
		do
		{
			index--;
			if(numberPlusPlus[index] > '0')
			{
				numberPlusPlus[index]--;
				sub = true;
			}
			else if( index > 0)
				numberPlusPlus[index] = '9';

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

char *longDivisionWithDecimalPart(char *dividend, char divisor[], unsigned int precision)
{
	unsigned long long dividendLength;
	char *localDividend, *result;
	bool remainderZero = false;
	
	dividendLength = strlen(dividend);
	localDividend = calloc(dividendLength + precision + 1, sizeof(char) );
	result = calloc(dividendLength + precision + 1, sizeof(char) );
	result[0] = '\0'; 
	localDividend[dividendLength + precision] = '\0';

	strcpy(localDividend, dividend);
	for(int decimals = 0; decimals <= precision && !remainderZero; decimals++)
	{
		char *temp;
		temp = longDivision(localDividend, divisor);
		strcat(result, temp);
		if( strcmp(localDividend, "0") != 0)
		{
			strcat(localDividend, "0");
			if( strchr(result, '.') == NULL)
				strcat(result, ".");
		}
		else
			remainderZero = true;
	}

	if( strchr(result, '.') == NULL )
	{
		strcat(result, ".0");
		memmove(dividend, localDividend, strlen(localDividend)+1);
	}
	else if( strcmp(localDividend, "0") != 0 )
	{
		/*Pending part: the program is capable of calculating the cuotient with decimal part but the reminder is partially wrong.*/
		memmove(dividend, localDividend, strlen(localDividend)+1);;
	}
	else
		memmove(dividend, localDividend, strlen(localDividend)+1);

	return result;
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
