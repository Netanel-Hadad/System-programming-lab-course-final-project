#ifndef ASSEMBLER_EXPORT
#define ASSEMBLER_EXPORT

#include <stdlib.h>
#include <math.h>
#include "label.h"
#include "errors.h"
#include "data.h"
#include "utilities.h"

#define PRINT_TO_OB(index, secondaryIndex, counter, array, addToAddress, file)\
while(index < counter)\
{\
	for(secondaryIndex = 0; secondaryIndex < ADDRESS_DIGITS - amountOfDigits(addToAddress + index); secondaryIndex++){\
		fprintf(file, "0");\
	}\
	fprintf(file, "%d ", addToAddress + i);\
	for(secondaryIndex = 0; secondaryIndex < OCTAL_DIGITS - amountOfDigits(decimalToOctal(array[index])); secondaryIndex++){\
		fprintf(file, "0");\
	}\
	fprintf(file, "%d\n", decimalToOctal(array[index]) % (int)pow(10, OCTAL_DIGITS));\
	i++;\
}\

void exportFile(char *, unsigned short int *, int, unsigned short int *, int, label *, table *, int);

#endif
