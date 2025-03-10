#ifndef ASSEMBLER_FIRST_RUN
#define ASSEMBLER_FIRST_RUN

#include "errors.h"
#include "label.h"
#include "data.h"
#include "second_run.h"
#include "utilities.h"
#include <stdio.h>

#define FIRST_RUN_FAILURE\
	passedFirstRun = 0;\
	continue\

int executeFirstRun(Macro *, char *);
int handleCommand(unsigned short int **, int *, char *, char *, char *, int);
void convertOperandAddressingMethod(unsigned short int *, int, int);

#endif
