#ifndef ASSEMBLER_ERRORS
#define ASSEMBLER_ERRORS

#include <stdio.h>

/* ----- general errors ----- */

#define NO_FILES_RECIVED_ERROR\
	printf("ERROR: no file names recived as arguments.\n")\

#define MEMORY_ALLOCATION_ERROR\
	printf("ERROR: memory allocation error. Exiting.\n\n");\
	exit(0)\

#define FILE_OPENING_ERROR(fileName)\
	printf("ERROR: could not open file '%s'.\n\n", fileName);\
	free(fileName)\

#define SURPASSED_MEMORY_LIMIT(fileName)\
	printf("ERROR: file '%s' require more memory than PC has.\n\n", fileName);\

/* ----- compilation erros ----- */

#define COMPILATION_ERROR(fileName)\
	printf("'%s' compilation failed.\n\n", fileName)\

#define MACRO_NAME_IN_USE_ERROR(fileName ,lineCount)\
	printf("ERROR: '%s', line %d:\n\tmacro name already in use.\n", fileName, line)\

#define NAME_IS_VALID_COMMAND_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tmacro cant be named as a valid command.\n", fileName, line)\

#define NAME_IS_VALID_INSTRUCTION_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tmacro cant be named as a valid instruction.\n", fileName, line)\

#define NAME_IS_MACRO_DECLERATION_COMMAND_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tmacro cant be named as a macro decleration command.\n", fileName, line)\

#define NAME_IS_REGISTER_ERROR(fileName, line)\
	printf("ERROR: '%s', line %d:\n\tmacro cant be named as a register.\n", fileName, line)\

#define NAME_IS_VALUE_ERROR(fileName, line)\
	printf("ERROR: '%s', line %d:\n\tmacro cant be named as a value.\n", fileName, line)\

#define EXTRANEOUS_TEXT_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\textraneous text at the end of the line.\n", fileName, lineCount)\

#define INVALID_LABEL_NAME_ERROR(fileName, lineCount, labelName)\
	printf("ERROR: '%s', line %d:\n\t'%s' is an invalid label name\n", fileName, lineCount, labelName)\

#define INVALID_COMMAND_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tinvalid command.\n", fileName, lineCount)\

#define MISSIGN_ARGUMENT_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tmissing argument.\n", fileName, lineCount)\

#define INVALID_OPERAND_TO_COMMAND_ERROR(fileName, lineCount, cmd, arg)\
	printf("ERROR: '%s', line %d:\n\tinvalid operand '%s' for '%s' command.\n", fileName, lineCount, arg, cmd)\

#define INVALID_REGISTER_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tinvalid register.\n", fileName, lineCount)\

#define INVALID_OPERAND_ERROR(fileName, lineCount)\
	printf("ERROR: '%s', line %d:\n\tinvalid operand.\n", fileName, lineCount)\

/* ----- compilation warnings ----- */

#define LABELED_STRING_OR_DATA_WARNING(fileName, lineCount)\
	printf("WARNING: '%s', line %d:\n\tdeclaring a label before '.entry' or '.extern' is useless.\n", fileName, lineCount)\

#endif

