#ifndef CHKOPS_H_
#define CHKOPS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*Four useful functions that helps you validate your passed arguments to main but you can also use them to validate some other variables.
*/
int findMainOptions( int optionsCount, char *availableOptions[], char *chosenOption );
bool isNumber(char* number);
bool fileExists(char* fileName);
bool areLettersOnly(char* word);
bool isSignedInteger(char* number);
bool isUnsignedInteger(char* number);
bool isFloat(char* number);

#endif
