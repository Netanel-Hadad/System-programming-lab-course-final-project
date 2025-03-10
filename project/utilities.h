#ifndef ASSEMBLER_UTILITIES
#define ASSEMBLER_UTILITIES

#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "errors.h"

#define OPCODE_SHIFT 11
#define SOURCE_OPERAND_SHIFT 7
#define TARGET_OPERAND_SHIFT 3
#define OPERAND_VALUE_SHIFT 3
#define OCTAL_DIGITS 5
#define ADDRESS_DIGITS 4

/* memory allocation */
#define ALLOCATE_MEMORY(pointer, type, size)\
	if((pointer = (type *)malloc(size)) == NULL){\
		MEMORY_ALLOCATION_ERROR;\
	}\

/* open file */
#define OPEN_FILE(filePointer, fileNamePointer, fileName, fileEnding, openMode)\
	ALLOCATE_MEMORY(fileNamePointer, char, strlen(fileName) + strlen(fileEnding))\
	strcpy(fileNamePointer, fileName);\
	strcat(fileNamePointer, fileEnding);\
	if((filePointer = fopen(fileNamePointer, openMode)) == NULL){\
		FILE_OPENING_ERROR(fileName);\
	}\
	free(fileNamePointer)\

/* basic table */
typedef struct table {
	char name[LABEL_NAME_MAX_LENGTH];
	int value;
} table;

/* strings */
char *getPartialString(char *, int, int);
char firstCharacter(char *str);
int isEmptyString(char *);

/* data converting */
void convertOpcode(unsigned short int *, char *);
void convertSourceOperand(unsigned short int *, int);
void convertTargetOperand(unsigned short int *, int);
void convertIncodingType(unsigned short int *, int);
int decimalToOctal(unsigned short int);

/* others */
void addMachineCode(unsigned short int **, int, unsigned short int);
int amountOfDigits(int);
void addToTable(table **, int, char *, int);

#endif
