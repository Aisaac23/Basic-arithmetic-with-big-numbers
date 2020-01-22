#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*The program receives as arguments, two unsigned integers

Example:

./longAddition 343456778384378290000000 34434999588887878867487736273762731116372
*/
int noArguments(int argc, char * argv[], char* version, char* author);
char* longAddition( char* summand1,  char* summand2);

int main(int argc, char* argv[])
{	
	if(argc < 3)
	{
		printf("Thre could be some data missing in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	char *result;	
	result = longAddition(argv[1], argv[2]);
	printf("\nResult: %s\n\n", result);
	free(result);
	
}

char* longAddition( char* summand1,  char* summand2)
{
	unsigned long long summand1Length, summand2Length, resultSize, shortest, carry = 0, newSize;
	char *result; 
	unsigned int sum = 0;
	bool summand1IsShorter;	

	summand1Length = strlen(summand1);
	summand2Length = strlen(summand2);
	resultSize = (summand1Length >= summand2Length) ? summand1Length+1 : summand2Length+1;
	
	// We need to prepare the space for normalized summands and the result... 
	result = calloc( resultSize+1, sizeof(char) );
	for(unsigned long long i = 0; i<resultSize; i++)
			result[i] = '0';
	result[resultSize] = '\0';
	
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
		else if ( resultSize >= 1 )
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
