#ifndef ASSEMBLER_DATA
#define ASSEMBLER_DATA

#include "string.h"

#define MAX_LINE_LENGTH 81
#define LABEL_NAME_MAX_LENGTH 31

#define MACRO_DECLERATION_START "macr"
#define MACRO_DECLERATION_END "endmacr"

#define STRING_INSTRUCTION ".string"
#define DATA_INSTRUCTION ".data"

#define ENTRY_INSTRUCTION ".entry"
#define EXTERN_INSTRUCTION ".extern"

#define AMOUNT_OF_REGISTERS 8

#define IC_START_ADDRESS 100
#define LAST_MEMORY_ADDRESS 4096

#define INSTANT_ADDRESSING 1
#define DIRECT_ADDRESSING 2
#define INDIRECT_REGISTER_ADDRESSING 4
#define DIRECT_REGISTER_ADDRESSING 8

#define EXTERNAL 1
#define RELOCATABLE 2
#define ABSOLUTE 4

#define NUM_OF_OPCODES 16

#define AS_FILE_ENDING ".as"
#define AM_FILE_ENDING ".am"
#define OB_FILE_ENDING ".ob"
#define EXT_FILE_ENDING ".ext"
#define ENT_FILE_ENDING ".ent"

typedef struct op_code {
	char *opcode;
	int numOfOperands;
	
} op_code;

int isOpcode(char *);
int isInstruction(char *);
int isSavedWord(char *);
int getNumOfOperands(int);
char *getOpcodeName(int);
int validDataValue(char *, int);

#endif
