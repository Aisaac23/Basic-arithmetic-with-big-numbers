#include "chkops.h"

//Checks in a list of available options if the chosen one exists.
int findMainOptions( int optionsCount, char *availableOptions[], char *chosenOption )
{
	if(chosenOption == NULL || optionsCount <= 0 || availableOptions == NULL)
		return false;
	
	for(int index = 0; index < optionsCount; index++)
		if( strcmp( availableOptions[index], chosenOption) == 0 )
			return index;
	return -1;
}

//Checks if the passed number (as string) is a valid integer or float type of number.
bool isNumber(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool point = false;
	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
		{
			if( !point && number[index] == '.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}
	return true;		
}

//Checks if passed argument is an unsigned integer number
bool isUnsignedInteger(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
			return false;
	return true;
}

//Checks if passed argument is a signed integer number
bool isSignedInteger(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool sign;
 
	sign = (number[0] == '+' || number[0] == '-') ? true : false;
	for( int index = 1; index < len; index++ )
		if( !isdigit(number[index]) )
				return false;
	
	return sign;
}

//First checks if passed argument is a number and then checks if it's a float number
bool isFloat(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool point = false;
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

//Checks if the passed file name exists and if it exists also checks if it can be opened.
bool fileExists(char* fileName)
{
	if(fileName == NULL)
		return false;

	FILE *tempFile;
	tempFile = fopen( fileName, "r");
	if( tempFile == NULL )
		return false;
	else
		fclose(tempFile);
	return true;
}

//Checks if the passed word consists only of letters and returns false on any occurrence of any other character. 
bool areLettersOnly(char* word)
{
	if(word == NULL)
		return false;

	int len = strlen(word);
	for( int index = 0; index < len; index++ )
		if( !isalpha(word[index]) )
			return false;
	return true;		
}
