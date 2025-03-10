#include "utilities.h"

/* return string from start to end index */
char *getPartialString(char *str, int start, int end)
{
    char *result;
    int i;

	ALLOCATE_MEMORY(result, char, (end-start+2));
    if((result = (char *)malloc(end-start+2)) == NULL){
        /* MEMORY_ALLOCATION_ERROR */
    }

    for(i = 0; (i+start) <= end; i++){
        result[i] = str[start + i];
    }
    result[i] = '\0';

    return result;
}

/* return the first character of a string, ignoring spaces and tabs */
char firstCharacter(char *str)
{
	int i;
	for(i = 0; i < strlen(str); i++){
		if(str[i] != ' ' || str[i] != '\t'){
			return str[i];
		}
	}
	return '\0';/* no character was found */
}


/* return 1 if string is empty or contain only spaces and tabs */
int isEmptyString(char *str)
{
	int i;

	if(strlen(str) == 0){
		return 1;
	}

	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] == '\n' || str[i] == '\0'){
			return 1;
		}else if(str[i] != ' ' && str[i] != '\t'){
			return 0;
		}
	}

	return 1;
}

/* set the bits that represents the opcode */
void convertOpcode(unsigned short int *code, char *oc)
{
    *code |= (isOpcode(oc) << OPCODE_SHIFT);
}

/* set the bits that represents the source operand */
void convertSourceOperand(unsigned short int *code, int so)
{
    *code |= (so << SOURCE_OPERAND_SHIFT);
}

/* set the bits that represents the target operand */
void convertTargetOperand(unsigned short int *code, int to)
{
    *code |= (to << TARGET_OPERAND_SHIFT);
}

/* set the bits that represents the incoding type */
void convertIncodingType(unsigned short int *code, int it)
{
    *code |= it;
}

/* convert decimal to octal, and return a 5 characters string */
int decimalToOctal(unsigned short int val)
{
    int octalNum = 0, i = 1;

    /* convert decimal to octal */
    while(val != 0)
    {
        int remainder = val % 8;
        octalNum += remainder * i;
        i = i * 10;
        val /= 8;
    }

    return octalNum;
}

/* add new machine code line to array */
void addMachineCode(unsigned short int **arr, int size, unsigned short int val)
{
    unsigned short int *temp;
    if(*arr == NULL){
		ALLOCATE_MEMORY(*arr, unsigned short int, sizeof(unsigned short int))
    }
    else{
        if((temp = (unsigned short int *)realloc(*arr, sizeof(unsigned short int) * (size + 1))) == NULL){
            MEMORY_ALLOCATION_ERROR;
        }
        *arr = temp;
    }
    (*arr)[size] = val;
}

/* return amonut of digits in number */
int amountOfDigits(int num)
{
	int result = 0;

	if(num == 0){
      return 1;
	}

    while (num != 0) 
	{
        result++;
        num /= 10;
    }

    return result;
}

/* add a new value to a table */
void addToTable(table **arr, int size, char *name, int value)
{
	table *temp;

	if((temp = (table *)(realloc(*arr, size * sizeof(table)))) == NULL){
		MEMORY_ALLOCATION_ERROR;
	}
	*arr = temp;

	strcpy((*arr)[size - 1].name, name);
	(*arr)[size - 1].value = value;
}
