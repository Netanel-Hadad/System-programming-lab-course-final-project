#include "macro.h"
#include "errors.h"
#include "data.h"
#include "first_run.h"
#include "utilities.h"

#define FILE_END\
	free(asFileName);\
	freeMacroTable(macroTable);\
	macroTable = NULL\

int main(int argc, char *argv[])
{
	int i;
	Macro *macroTable;
	FILE *asFile;
	char *asFileName;

	if(argc == 1){
		NO_FILES_RECIVED_ERROR;
		exit(0);
	}

	for(i = 1; i < argc; i++)
	{
		if((macroTable = (Macro *)malloc(sizeof(Macro))) == NULL ||
			(asFileName = (char *)malloc(strlen(argv[i]) + strlen(AS_FILE_ENDING))) == NULL){
			MEMORY_ALLOCATION_ERROR;
		}

		/* open the 'as' file named as the value recieved as argument */
		strcpy(asFileName, argv[i]);
		strcat(asFileName, AS_FILE_ENDING);
		if((asFile = fopen(asFileName, "r")) == NULL)
		{
			FILE_OPENING_ERROR(asFileName);
			continue;
		}

		/* macro stage */
		if(!handleMacro(&macroTable, argv[i], asFile))
		{
			/* macro stage failed */
			COMPILATION_ERROR(asFileName);
			FILE_END;
			continue;
		}	

		/* executing the first run. if passed the second run will be executed from 'first_run.c'.
			if failed one of them, 0 will be returned. if file passed the second run,
			export stage will be executed from 'second_run.c' */
		if(!executeFirstRun(macroTable, argv[i]))
		{
			COMPILATION_ERROR(asFileName);
			FILE_END;
			continue;
		}
		
		/* end of file */
		printf("'%s' compilation succeeded.\n\n", asFileName);
		FILE_END;
	}

	return 0;
}
