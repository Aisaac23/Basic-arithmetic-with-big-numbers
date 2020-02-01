#include "bignums.h"

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the subtraction.
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

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the multiplication.
char* longMultiplication( char* factor1,  char* factor2)
{	
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

	//you could have spare zeros in the left, so we rotate
	while(result[0] == '0' && strlen(result) > 1)
		memmove(result, result+1, resultSize*sizeof(char)+1);

	return result;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the division.
char *longDivision(char *dividend, char divisor[])
{
	//With this one I reused the longSubtraction's function code and made some modifications.  
	unsigned long long dividendSize, divisorSize, CuotientIndex, divisorIndex, dividendIndex;
	char *cuotient, *z;
	bool remainderZero = false;
	int subtraction, loans;
	

	//Error handling
	if( divisor == NULL || dividend == NULL )
		return NULL;

	dividendSize = strlen(dividend);
	divisorSize = strlen(divisor);
	z = calloc(2, sizeof(char));
	z[0] = '0';
	z[1] = '\0';
	

	//Error handling
	if( divisorSize == 0 || dividendSize == 0 )
		return NULL;

	if(divisor[0] == '0')
		return NULL;

	if( dividendSize < divisorSize || dividend[0] == '0')
		return z;

	cuotient = calloc( dividendSize+1, sizeof(char) );
	
	for(long int i = 0; i<dividendSize; i++)
			cuotient[i] = '0';

	cuotient[dividendSize] = '\0';
	
	while(!remainderZero )
	{		
		//Subtraction loop, it's practically the same as in "LongSubtraction"
		divisorIndex = divisorSize; // we subtract from left to right.
		dividendIndex = dividendSize;
		do
		{
			divisorIndex--;
			dividendIndex--;
			subtraction = (dividend[dividendIndex] - '0') - (divisor[divisorIndex] - '0');
			if(subtraction < 0 && divisorIndex >= 0)//case when we have, for example: 54678-9 
			{
				subtraction +=10;
				
				int next = 1;
				loans = dividend[dividendIndex - next] - '0';//the one who loans 1
				
				while(loans == 0)// when in dividend we have, for exmple: 50008 - 9
				{
					dividend[ dividendIndex - next ] = '9';
					next++;
					loans = dividend[ dividendIndex - next ] - '0';	
				}
				dividend[ dividendIndex - next ] = (char)( (loans-1) + '0' );
			}
			dividend[ dividendIndex ] = (char)( subtraction + '0' ) ;
			
		}while(divisorIndex > 0);//divisorIndex is unsigned so it'd cause a runtime error if it gets to -1

		/*Here we check if we have a reminder of zero, in that case the we've finished long division*/
		remainderZero = true;
		unsigned long long zeros = 0;
		for(unsigned long long index = 0; index<dividendSize && remainderZero; index++)
			if( dividend[index] != '0')
				remainderZero = false;
			else
				zeros++;
		
		/*Here we add one to the cuotient, since we've been able to subtract once the divisor from the dividend*/
		CuotientIndex = dividendSize;
		bool added = false;
		do
		{
			CuotientIndex--;
			if(cuotient[CuotientIndex] < '9')
			{
				cuotient[CuotientIndex]++;
				added = true;
			}
			else if( CuotientIndex > 0)
				cuotient[CuotientIndex] = '0';

		}while( CuotientIndex > 0 && !added );
		
		//Here we check whether the dividend has became a number lower than de divisor
		int sorIndex = 0, sub;
		
		if( dividendSize - (zeros) < divisorSize )
			remainderZero = true;
		else if( dividendSize - (zeros) == divisorSize )
		{
			for( int endIndex=zeros ; endIndex < dividendSize; endIndex++)
			{
				sub = (dividend[endIndex]-'0') - (divisor[sorIndex++]-'0');
				if( sub <= 0 )
					remainderZero = sub < 0 ? true: false;
				else
					zeros = dividendSize;
			}
			
		}
  		else
			remainderZero = false;
	}
	
	//you could have spare zeros in the left, so we rotate
	while(dividend[0] == '0' && strlen(dividend) > 1)
		memmove(dividend, dividend+1, dividendSize*sizeof(char)+1 );
	while(cuotient[0] == '0' && strlen(cuotient) > 1)
		memmove(cuotient, cuotient+1, dividendSize*sizeof(char)+1 );

	return cuotient;
}

//Receives as arguments, two unsigned integers (as strings) and returns a string with the result of the addition.
char* longAddition( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultSize, shortest, carry = 0, newSize;
	char *result; 
	unsigned int sum = 0;
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
			else if( index > 0)
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

