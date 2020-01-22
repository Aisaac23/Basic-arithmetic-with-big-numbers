#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
/*The program receives as arguments, two unsigned integers, first the dividend and then the divisor. The dividend should alwyas be >= the divisor.

Example:

./longDivision 343456778384378290000000 3443499958888

NOTE: the coutient will replace the dividend, so you may want to copy it to a different location.
*/

char *longDivision(char *dividend, char divisor[]);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);

int main(int argc, char* argv[])
{
	char *result = NULL, *number1 = NULL, *number2 = NULL;

	if(argc == 3)
	{
		number1 = argv[1];
		result = longDivision(number1, argv[2]);
	}	
	else if(argc == 5)
	{
		number1 = readBigNumber(argv[1], atoi(argv[2]));
		number2 = readBigNumber(argv[3], atoi(argv[4]));
		
		printf("%s / %s =\n\n", number1, number2);
		result = longDivision(number1, number2);
	}
	else
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	printf("Cuotient: %s, Remainder:%s\n", result, number1);
	if( !strcmp(result, "NAN") )
		free(result);
	return EXIT_SUCCESS;
}

char *longDivision(char *dividend, char divisor[])
{
	//With this one I reused the longSubtraction's function code and made some modifications.  
	unsigned long long dividendSize, divisorSize, CuotientIndex, divisorIndex, dividendIndex;
	char *cuotient;
	bool remainderZero = false;
	int subtraction, loans;
	
	dividendSize = strlen(dividend);
	divisorSize = strlen(divisor);
	
	if( divisorSize == 0 )
		return "NAN";
	if( dividendSize == 0 )
		return "0";

	if(divisor[0] == '0')
		return "NaN";
	if( dividendSize < divisorSize || dividend[0] == '0' )
		return "0";

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
		/**************************************/
		
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
		/*************************************/	
		
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
	
	//most of the times you'll have spare zeros in the left, so we rotate 
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
