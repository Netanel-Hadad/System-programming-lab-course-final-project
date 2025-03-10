#ifndef ASSEMBLER_LABEL
#define ASSEMBLER_LABEL

#include "errors.h"
#include "data.h"
#include "macro.h"
#include <string.h>
#include <stdlib.h>

#define CODE_TYPE_VAL 0
#define DATA_TYPE_VAL 1

enum location{code = CODE_TYPE_VAL, data = DATA_TYPE_VAL};
enum type{external = EXTERNAL, relocatable = RELOCATABLE};

typedef struct label{
	char name[LABEL_NAME_MAX_LENGTH];
	int value;/* address in IC/DC */
	enum location _location;
	enum type _type;
	int isEntry;
	struct label *next;
} label;

void addToLabels(label **, char *, int, enum location, enum type);
int isLabelExist(label *, char *);
int isValidLabelName(label *, char *, Macro *);
int getLabelValue(label *, char *);
label *getLabel(label *, char *);
void freeLabels(label *);
void reverseList(label **);

#endif
