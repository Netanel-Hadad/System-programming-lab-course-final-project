#include "second_run.h"

/* execute 'second run' stage on a given file, 
	return 0 if failed, 1 if passed */
int executeSecondRun(Macro *macroTable, char *fileName, unsigned short int **instructions, int IC,
	unsigned short int **data, int DC, label *labels)
{
	FILE *amFile;
	char line[MAX_LINE_LENGTH], *tok, *amFileName, *labelName;
	int i = 0, currentIC = 0, opcodeNum, firstOperandIsRegister = 0, lineCount = 1, passedSecondRun = 1, amountOfExternAppearances = 0;
	table *externAppearances = NULL;
		
	/* open '(fileName).am' */
	OPEN_FILE(amFile, amFileName, fileName, AM_FILE_ENDING, "r");

	/* handle entries */
	while(fgets(line, MAX_LINE_LENGTH, amFile))
	{
		tok = strtok(line, " \t\n");

		/* ignore label decleration, read the next word */
		if(tok[strlen(tok) - 1] == ':'){
			tok = strtok(NULL, " \t\n");
		}

		if(strcmp(tok, ENTRY_INSTRUCTION) == 0)/* .entry */
		{		
			tok = strtok(NULL, " \t\n");
			getLabel(labels, tok)->isEntry = 1;
		}
	}

	rewind(amFile);

	/* convert labels into machine code */
	while(fgets(line, MAX_LINE_LENGTH, amFile))
	{
		lineCount++;
		firstOperandIsRegister = 0;
		tok = strtok(line, " \t\n");

		/* ignore label decleration, read the next word */
		if(tok[strlen(tok) - 1] == ':'){
			tok = strtok(NULL, " \t\n");
		}

		if((opcodeNum = isOpcode(tok)) != -1)/* opcode */
		{
			i = 0;
			while(i < getNumOfOperands(opcodeNum))
			{
				currentIC++;
				tok = strtok(NULL, " ,\t\n");

				/* if operand is the source operand, need to delete the ',' in the end of it */
				labelName = getPartialString(tok, 0, strlen(tok) - 1 - (tok[strlen(tok) - 1] == ','));

				/* i = 0 -> currently working on the source operand,
				i = 1 -> currently working on the target operand

				therefore, we check if the command require only a target operand,
				and if so we skip i = 0, meaning skipping the source operand */
				if(!i && opcodeNum >= isOpcode("clr")){
					i++;
				}				

				/* if both operands are register, both of their values goes in the 
					same machine code line */
				if(tok[0] == 'r' || tok[0] == '*'){
					if(!i){
						firstOperandIsRegister = 1;
					}
					else if(i && firstOperandIsRegister){
						currentIC--;
					}
				}

				if(isLabelExist(labels, labelName))/* operand is a valid label */
				{
					/* label is extern */
					if(getLabel(labels, labelName)->_type == EXTERNAL)
					{
						(*instructions)[currentIC] |= EXTERNAL;
						addToTable(&externAppearances, ++amountOfExternAppearances, labelName, currentIC);
					}
					else{/* label declared in this file */
						(*instructions)[currentIC] = getLabelValue(labels, labelName);
						(*instructions)[currentIC] <<= OPERAND_VALUE_SHIFT;
						(*instructions)[currentIC] |= RELOCATABLE;
					}
				}
				else if(!isSavedWord(labelName) && labelName[0] != '#' && !(tok[0] == 'r' || tok[0] == '*'))/* invalid operand */
				{
					INVALID_OPERAND_ERROR(fileName, lineCount);
					passedSecondRun = 0;
				}

				i++;
			}/* end of while, reading operands from line */

			currentIC++;
		}
	}/* end of while, reading lines from file */

	fclose(amFile);

	if((IC + DC + IC_START_ADDRESS) > LAST_MEMORY_ADDRESS){/* file require more memory than what the pc has */
		SURPASSED_MEMORY_LIMIT(fileName);
		passedSecondRun = 0;
	}		

	if(passedSecondRun){
		exportFile(fileName, *instructions, IC, *data, DC, labels, externAppearances, amountOfExternAppearances);
	}

	freeLabels(labels);
	free(*instructions);
	free(*data);
	return passedSecondRun;
}
