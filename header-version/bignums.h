#ifndef BIGNUMS_H_
#define BIGNUMS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char *longAddition( char* summand1,  char* summand2);
char *longSubtraction(char minuend[], char subtrahend[]);
char *longMultiplication( char* factor1,  char* factor2);
char *longDivision(char *dividend, char *divisor);

char *longDivisionFloatingPointResult(char *dividend, char divisor[], unsigned int precision);
char *longDivisionWithReminder(char *dividend, char divisor[]);
char *longModule(char *dividend, char *divisor);

char *longFloatingPointSubtracction(char *minuend, char *subtrahend);
char *longFloatingPointMultiplication(char *factor1, char *factor2);
char *longFloatingPointDivision(char *dividend, char *divisor);
char *longFloatingPointAddition(char *summand1, char *summand2);

int compareUnsignedIntegers(char* n1, char *n2);

void increment(char* numberPlusPlus);
void decrement(char* numberMM);

unsigned long long count_decimal_digits(char *fp_number);
unsigned long long count_integer_digits(char *fp_number);

char *readBigNumber(char *fileName, const unsigned long long SLICELENGTH);
char *formatNumber(char *n, int slice, char separator);

#endif
