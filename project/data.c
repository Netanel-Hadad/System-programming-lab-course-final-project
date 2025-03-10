#include "data.h"

op_code OPCODES[] = {
	{"mov", 2},
	{"cmp", 2},
	{"add", 2},
	{"sub", 2},
	{"lea", 2},
	{"clr", 1},
	{"not", 1},
	{"inc", 1},
	{"dec", 1},
	{"jmp", 1},
	{"bne", 1},
	{"red", 1},
	{"prn", 1},
	{"jsr", 1},
	{"rts", 0},
	{"stop", 0}
};

/* if given string is not opcode name, return -1.
    otherwise, return its value(0 - 15). */
int isOpcode(char *str)
{
	int i;
	for(i = 0; i < NUM_OF_OPCODES; i++){
		if(strcmp(str, OPCODES[i].opcode) == 0){
			return i;
		}
	}
	return -1;
}

/* return 1 if str is instruction */
int isInstruction(char *str)
{
	if(strcmp(str, STRING_INSTRUCTION) == 0 || strcmp(str, DATA_INSTRUCTION) == 0 ||
		strcmp(str, EXTERN_INSTRUCTION) == 0 || strcmp(str, ENTRY_INSTRUCTION) == 0){
			return 1;
	}
	return 0;
}

/* return 1 if str is a saved word (opcode, label type, "macr", "endmacr") */
int isSavedWord(char *str)
{
	if(isOpcode(str) != -1 || strcmp(str, MACRO_DECLERATION_START) == 0 ||
		strcmp(str, MACRO_DECLERATION_END) == 0 || isInstruction(str)){
			return 1;
	}
	return 0;
}

/* return opcode #index number of operands */
int getNumOfOperands(int index)
{
	return OPCODES[index].numOfOperands;
}

/* return an opcode name by index */
char *getOpcodeName(int i)
{
	return OPCODES[i].opcode;
}

/* returns 1 if str is a valid value for '.data' decleration */
int validDataValue(char *str, int length)
{
    int i = 0;

	while(str[i] == ' '){
		i++;
	}

	if(str[i] == '+' || str[i] == '-'){
		i++;
	}

    while(i < length)
    {
        if((str[i] != '\n' && str[i] != ' ' && i != length-1) && (str[i] < '0' || str[i] > '9')){
            return 0;
        }
        i++;
    }

    return 1;
}
