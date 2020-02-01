#ifndef BIGNUMS_H_
#define BIGNUMS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char* longSubtraction(char minuend[], char subtrahend[]);
char* longMultiplication( char* factor1,  char* factor2);
char *longDivision(char *dividend, char divisor[]);
char* longAddition( char* summand1,  char* summand2);
char* readBigNumber(char *fileName, const unsigned int SLICELENGTH);
char *increment(char* numberPlusPlus);
void decrement(char* numberPlusPlus);
char* formatNumber(char *n, int slice, char separator);

#endif
