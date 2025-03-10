#include "export.h"

void exportFile(char *fileName, unsigned short int *instructions, int IC, unsigned short int *data, int DC,
	 label *labels, table *externAppearances, int amountOfExternAppearances)
{
	label *currentLabel;
	FILE *file;
	char *newFileName;
	int i = 0, j, foundEntry = 0;
		
	/* create '(fileName).ob' */
	OPEN_FILE(file, newFileName, fileName, OB_FILE_ENDING, "w");

	/* print all the machine codes from instructions and data 
		and their address to the '.ob' file */
	fprintf(file, "  %d %d\n", IC, DC);/* file title */
	PRINT_TO_OB(i, j, IC, instructions, IC_START_ADDRESS, file)/* print from instructions */
	i = 0;
	PRINT_TO_OB(i, j, DC, data, (IC + IC_START_ADDRESS), file)/* print from data */
	fclose(file);

	/* creating and printing into the '.ent' file, if necessery */
	reverseList(&labels);/* reverse labels so entries will be orderd by address */
	currentLabel = labels;
	while(currentLabel != NULL)
	{
		if(currentLabel->isEntry == 1)
		{
			if(!foundEntry)/* first entry, need to create the '.ent' file */
			{
				OPEN_FILE(file, newFileName, fileName, ENT_FILE_ENDING, "w");
				foundEntry = 1;
			}

			fprintf(file, "%s %d\n", currentLabel->name, currentLabel->value);
		}
		currentLabel = currentLabel->next;
	}

	if(foundEntry){
		fclose(file);
	}

	/* creating and printing into the '.ext' file, if necessery */
	if(amountOfExternAppearances > 0)
	{
		OPEN_FILE(file, newFileName, fileName, EXT_FILE_ENDING, "w");

		/* print all the appearances of externs */
		i = 0;
		while(i < amountOfExternAppearances)
		{
			fprintf(file, "%s ", externAppearances[i].name);
			for(j = 0; j < ADDRESS_DIGITS - amountOfDigits(IC_START_ADDRESS + externAppearances[i].value); j++){
				fprintf(file, "0");
			}
			fprintf(file, "%d\n",	IC_START_ADDRESS +  externAppearances[i].value);
			i++;
		}

		free(externAppearances);
		fclose(file);
	}
}
