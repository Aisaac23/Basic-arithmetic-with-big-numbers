#ifndef BIGNUMS_H_
#define BIGNUMS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

void longAdditionInPlace( char* summand1,  char* summand2);
void longSubtractionInPlace( char* summand1,  char* summand2);
void longSubtractionInPlaceSigned( char* summand1,  char* summand2);
void longMultiplicationInPlace( char* factor1,  char* factor2);
void longDivisionInPlace(char *dividend, char *divisor);
void longDivisionWithReminderInPlace(char *dividend, char *divisor);
void longModuleInPlace(char *dividend, char *divisor);
void incrementInPlace(char* numberPlusPlus);


char *longDivisionFloatingPointResult(char *dividend, char divisor[], unsigned int precision);

char *longFloatingPointSubtraction(char *minuend, char *subtrahend);
char *longFloatingPointMultiplication(char *factor1, char *factor2);
char *longFloatingPointDivision(char *dividend, char *divisor);
char *longFloatingPointAddition(char *summand1, char *summand2);

int compareUnsignedIntegers(char* n1, char *n2);

#endif
