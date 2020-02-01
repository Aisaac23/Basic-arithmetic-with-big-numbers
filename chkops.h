#ifndef CHKOPS_H_
#define CHKOPS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
<<<<<<< HEAD
#include <wchar.h>

#define ASCII_MAX 127
/*Some useful functions that helps you validate multiple types of strings*/
int findMainOptions( int optionsCount, char *availableOptions[], char *chosenOption );
void setAvailableOptions(char **options, char *optionsString);
bool isUnsignedNumber(char* number);
bool isSignedNumber(char* number);
bool fileExists(char* fileName);
bool isAlphabeticString(char* word);
bool isSignedInteger(char* number);
bool isUnsignedInteger(char* number);
bool isSignedFloat(char* number);
bool isUnsignedFloat(char* number);
bool hasWideChars(wchar_t *word);
bool isFileName(char* fileName);
bool isAlphaNumericString(char* word);

=======

/*Four useful functions that helps you validate your passed arguments to main but you can also use them to validate some other variables.
*/
int findMainOptions( int optionsCount, char *availableOptions[], char *chosenOption );
bool isNumber(char* number);
bool fileExists(char* fileName);
bool areLettersOnly(char* word);
bool isSignedInteger(char* number);
bool isUnsignedInteger(char* number);
bool isFloat(char* number);
>>>>>>> 1e846f089ab73f7bed3f18754f0e86ba8ad262e5

#endif
