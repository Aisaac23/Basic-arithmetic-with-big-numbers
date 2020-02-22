#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chkops.h"

/*The program receives as arguments, two unsigned integers or two file names and how many digits the program should read from the file. First the dividend and then the divisor. The dividend should alwyas be >= the divisor otherwise the result will be zero.

Example:

./longDivision 343456778384378290000000 3443499958888
./longDivision bignumber.txt 100 bignumber2.txt 10

NOTE: the cuotient will replace the dividend, so you may want to copy its value to a different location.
*/

char *longDivision(char *dividend, char divisor[]);
char *readBigNumber(char *fileName, const unsigned int SLICELENGTH);
char *longDivisionWithDecimalPart(char *dividend, char divisor[], unsigned int precision);

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if( argc == 3 && isUnsignedInteger(argv[1]) && isUnsignedInteger(argv[2]) )
	{
		number1 = argv[1];
		result = longDivisionWithDecimalPart(number1, argv[2], 2);
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
		result = longDivision(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("Cuotient: %s, Remainder:%s\n", result, number1);
	if( result != NULL )
		free(result);

	return EXIT_SUCCESS;
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
	else if( dividendSize == divisorSize )
		for(unsigned long long index = 0; index<dividendSize; index++)
			if( dividend[index] < divisor[index] )
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
				zeros++;//we also count the leading zeros to use them later
		
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
		int sorIndex = 0;
		
		if( dividendSize - (zeros) < divisorSize )
			remainderZero = true;
		else if( dividendSize - (zeros) == divisorSize )
		{
			for( int endIndex=zeros ; endIndex < dividendSize && !remainderZero; endIndex++)
				if( dividend[endIndex] < divisor[sorIndex++] )
					remainderZero = true;
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
