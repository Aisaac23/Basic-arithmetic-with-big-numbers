/*The program receives as arguments, two unsigned integers, first the minuend and then the subtrahend.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *longSubtraction(char minuend[], char subtrahend[]);
int noArguments(int argc, char * argv[], char* version, char* author);

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	char *result;
	result = longSubtraction(argv[1], argv[2]);
	
	printf("\nResult: %s\n", result);
	
	free(result);
	return EXIT_SUCCESS;
}

char *longSubtraction(char *minuend, char *subtrahend)
{
	unsigned long long int minuendLength, subtrahendLength, resultSize, shortest, newSize;
	int subtraction, carry = 0;
	char *result;
	bool subtrahendIsShorter = true;

	minuendLength = strlen(minuend); 
	subtrahendLength = strlen(subtrahend);
	resultSize = (minuendLength >= subtrahendLength) ? minuendLength : subtrahendLength;
	
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
			{
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
	
	//most of the times you'll have spare zeros in the left, so we rotate 
	while(result[0] == '0' && strlen(result) > 1)
		memmove(result, result+1, strlen(result)*sizeof(char)+1);

	newSize = strlen(result);
	if( !subtrahendIsShorter ) //if the result was negative, we add the - sign.
	{
		result = realloc( result, newSize*sizeof(char)+2 );
		memmove(result+1, result, newSize*sizeof(char)+1);
		result[0]='-';	
	}
	else //otherwise we just resize it
		result = realloc( result, newSize*sizeof(char)+1 );

	return result;
}
