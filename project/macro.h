#ifndef ASSEMBLER_MACRO
#define ASSEMBLER_MACRO

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "string.h"
#include "data.h"
#include "utilities.h"

#define FAILED_MACRO_STAGE \
	passedMacroStage = 0;\
	lineCount++;\
	continue\

typedef struct Macro {
	char *name;
	char *content;
	struct Macro *next;
} Macro;

void addToMacroTable(Macro **, char *, char *);
char *getMacroContent(Macro *, char *);
int handleMacro(Macro **, char *, FILE *);
void freeMacroTable(Macro *);
int isValidMacroName(Macro *, char *, char *, int);
void writeAMfile(Macro **, char *, FILE *);
int isMacroName(Macro *, char *);

#endif

