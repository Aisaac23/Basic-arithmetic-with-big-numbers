#include "chkops.h"

/*Checks in a list of available options whether the chosen one exists; if it does, returns its position in the array, otherwise it returns -1.
Receives as argumens: the size of the array that contains the options available, an array of strings with the options, an option */
int findMainOptions( int optionsCount, char *availableOptions[], char *chosenOption )
{
	if(chosenOption == NULL || optionsCount <= 0 || availableOptions == NULL)
		return false;
	
	for(int index = 0; index < optionsCount; index++)
		if( strcmp( availableOptions[index], chosenOption) == 0 )
			return index;
	return -1;
}

//Checks whether the passed number (as string) is a valid UNSIGNED integer or float type of number.
bool isUnsignedNumber(char* number)
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
	return true;		
}

//Checks whether the passed number (as string) is a valid SIGNED integer or float type of number.
bool isSignedNumber(char* number)
{
	if(number == NULL)
		return false;

	int len = strlen(number);
	bool sign, point = false;

 	if(len == 0)
		return false;
	
	sign = (number[0] == '+' || number[0] == '-') ? true : false;
	for( int index = 0; index < len; index++ )
		if( !isdigit(number[index]) )
		{
			if( !point && number[index] == '.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}
	return sign;		
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

	sign = (number[0] == '+' || number[0] == '-') ? true : false;
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

	int len = strlen(number);
	bool sign, point = false;

 	if(len == 0)
		return false;
 
	sign = (number[0] == '+' || number[0] == '-') ? true : false;
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


//Checks whether the passed file name exists and if it exists also checks if it can be opened.
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

//Checks whether the passed file name is valid, this, by trying to open it in append mode.
bool isFileName(char* fileName)
{
	if(fileName == NULL)
		return false;

	FILE *tempFile;
	tempFile = fopen( fileName, "a");
	if( tempFile == NULL )
		return false;
	else
		fclose(tempFile);
	return true;
}

//Checks whether the passed word consists only of letters and returns false on any occurrence of any other character. 
bool isAlphabeticString(char* word)
{
	if(word == NULL)
		return false;

	int len = strlen(word);
 	if(len == 0)
		return false;


	for( int index = 0; index < len; index++ )
		if( !isalpha(word[index]) )
			return false;
	return true;		
}

//Checks whether the passed word consists only of letters and numbers; returns false on any occurrence of any other character. 
bool isAlphaNumericString(char* word)
{
	if(word == NULL)
		return false;

	int len = strlen(word);
 	if(len == 0)
		return false;

	
	for( int index = 0; index < len; index++ )
		if( !isalnum(word[index]) )
			return false;
	return true;		
}


//Checks whether the passed word consists only of ASCII characters; returns false on any occurrence of any character with a value grater than 127
bool hasWideChars(wchar_t *word)
{
	if(word == NULL)
		return false;

 	if(wcslen(word) == 0)
		return false;

	for(int i = 0; i<wcslen(word); i++)		
		if( ( (wint_t)word[i] ) > ASCII_MAX )
			return true;
	return false;
}

/*Receives a char pointer to a pointer where the options will be set in the same order they apear in the second passed argument "optionsString".
"char **options" should be initilized at least with n pointers (e.g. char *options[20]) before passing it and "optionsString" can be any string*/
void setAvailableOptions(char **options, char *optionsString)
{
	char *op, *toMakeTokens;
	int len;
	
	len = strlen(optionsString);
	toMakeTokens = (char*)calloc(len, sizeof(char) );
	strcpy(toMakeTokens, optionsString);
	toMakeTokens[len] = '\0';	
	
	op = strtok(optionsString, " ");
	while(op != NULL)
	{
		len = strlen(op);
		*(options) = calloc( len+1, sizeof(char) );
		strcpy( *(options) , op);
		(*(options))[len] = '\0';
		options++;
		op = strtok(NULL, " ");
	}
	return;
}

