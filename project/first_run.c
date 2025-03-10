#include "first_run.h"

/* execute 'first run' stage on a given file, 
	return 0 if failed, 1 if passed */
int executeFirstRun(Macro *macroTable, char *fileName)
{
	label *labels = NULL, *tempLabel = NULL;
	char *amFileName, line[MAX_LINE_LENGTH], *tok;
	FILE *amFile;
	int IC = 0, DC = 0, lineCount = 0, labeldLine = 0, passedFirstRun = 1, 
		passedSecondRun, i = 0;
	unsigned short int *instructions = NULL, *data = NULL;

	/* labels holds all the labels declared in the source file.
	instructions and data hold machine codes for lines that were converted,
	currentCode holds the machine code of the line we are currently reading,
	labeldLine holds if the current line has a label decleration,
	i is used as an index. */

	/* open '(fileName).am' */
	OPEN_FILE(amFile, amFileName, fileName, AM_FILE_ENDING, "r");

	while(fgets(line, MAX_LINE_LENGTH, amFile))
	{
		i = 0;
		labeldLine = 0;
		lineCount++;
		tok = strtok(line, " \t\n");
		tempLabel = NULL;

		if(firstCharacter(line) == ';')/* igonre comments */
		{
			lineCount++;
			continue;
		}
	
		/* check for a label decleration at the start of the line */
		if(tok[strlen(tok) - 1] == ':')
		{
			if(isValidLabelName(labels, getPartialString(tok, 0, strlen(tok) - 2), macroTable))/* valid label decleration */
			{
				/* add a new label. Its location and type will be determined later on, 
					so for now we will use placeholder values */
				addToLabels(&labels, getPartialString(tok, 0, strlen(tok) - 2), IC, CODE_TYPE_VAL, RELOCATABLE);
				labeldLine = 1;
			}
			else/* invalid label decleration */
			{
				INVALID_LABEL_NAME_ERROR(fileName, lineCount, tok);
				/*TODO: print the exact problem with the name */
				FIRST_RUN_FAILURE;
			}
		}

		/* read the next word only if there was a label decleration */
		if(labeldLine){
			tok = strtok(NULL, " \t\n");
		}

		if(strcmp(tok, DATA_INSTRUCTION) == 0 || strcmp(tok, STRING_INSTRUCTION) == 0)/* .data OR .string*/
		{
			/* if a label was declered, update its data */
			if(labeldLine)
			{
				labels->_location = DATA_TYPE_VAL;
				labels->value = DC;
			}

			if(strcmp(tok, DATA_INSTRUCTION) == 0)/* .data */
			{
				while((tok = strtok(NULL, ",")) != NULL)
				{
					if(!validDataValue(tok, strlen(tok)))
					{
						printf("ERORR: '%s', line %d:\n\tinvalid value for '.data'.\n", fileName, lineCount);
						FIRST_RUN_FAILURE;
					}	
					else{
						addMachineCode(&data, DC++, atoi(tok));
					}
				}
			}
			else/* .string */
			{
				tok = strtok(NULL, " \t\n");

				if(tok == NULL || tok[0] != '\"' || tok[strlen(tok) - 1] != '\"'){
					printf("ERORR: '%s', line %d:\n\tinvalid value for '.string'.\n", fileName, lineCount);
					FIRST_RUN_FAILURE;
				}
				
				/* add the operand characters ascii codes to data */
				i = 1;
				while(i < (strlen(tok) - 1)){
					addMachineCode(&data, DC++, (int)(tok[i++]));
				}
				addMachineCode(&data, DC++, (int)('\0'));
			}
		}
		else if(strcmp(tok, ENTRY_INSTRUCTION) == 0 || strcmp(tok, EXTERN_INSTRUCTION) == 0)/* .entry OR .extern*/
		{
			/* entries will be handeld in the second run, externs will be added to the labels list */
			if(strcmp(tok, EXTERN_INSTRUCTION) == 0){
				tok = strtok(NULL, " \t\n");
				addToLabels(&labels, tok, 0, 0, EXTERNAL);
			}

	   		/* if there is a label decleration before them, throw a warning as its harmless yet useless.
			   read the next word, needed in order to check for extraneous words. */
			if(labeldLine){
				LABELED_STRING_OR_DATA_WARNING(fileName, lineCount);
			}
			tok = strtok(NULL, " \t\n");
		}
		else/* opcode */
		{
			if(isOpcode(tok) != -1)/* tok is a valid opcode */
			{
				if(!handleCommand(&instructions, &IC, fileName, line, tok, lineCount)){
					FIRST_RUN_FAILURE;
				}
			}
			else
			{
				INVALID_COMMAND_ERROR(fileName, lineCount);
				FIRST_RUN_FAILURE;
			}
		}

		/* check for extraneous text at the end of the line */
		if((tok = strtok(NULL, " \t\n")) != NULL)
		{
			EXTRANEOUS_TEXT_ERROR(fileName, lineCount);
			FIRST_RUN_FAILURE;
		}
	}/* end of while */

	/* instructions has a default start address. add it to all labels.
	   also, because all the data in the '.ob' file comes after the code,
	   add IC to the values of all the data labels */
	tempLabel = labels;
	while(tempLabel != NULL)
	{
		tempLabel->value += (IC_START_ADDRESS);
		if(tempLabel->_location == DATA_TYPE_VAL){
			tempLabel->value += IC;
		}
		tempLabel = tempLabel->next;
	}

	fclose(amFile);

	passedSecondRun = executeSecondRun(macroTable, fileName, &instructions, IC, &data, DC, labels);
	return passedFirstRun && passedSecondRun;
}

/* handle a command line. return 0 if line have an error */
int handleCommand(unsigned short int **instructions, int *IC, char *fileName, char *line, char *tok, int lineCount)
{
	int i = 0, hasSourceOperand = 0, hasTargetOperand = 0, opcodeNum, registerNum, firstOperandIsRegister = 0;
	unsigned short int currentCode = 0, sourceOperandCode = 0, targetOperandCode = 0, *currentOperandCode;

	convertOpcode(&currentCode, tok);
	convertIncodingType(&currentCode, ABSOLUTE);

	opcodeNum = isOpcode(tok);

	while(i < getNumOfOperands(opcodeNum))
	{
		/* i = 0 -> currently working on the source operand,
		i = 1 -> currently working on the target operand

		therefore, we check if the command require only a target operand,
		and if so we skip i = 0, meaning skipping the source operand */
		if(!i && opcodeNum >= isOpcode("clr")){
			i++;
		}

		if(!i){
			hasSourceOperand = 1;
			currentOperandCode = &sourceOperandCode;
		}
		else{
			hasTargetOperand = 1;
			currentOperandCode = &targetOperandCode;
		}

		tok = strtok(NULL, " ,\t\n");

		/* check for a missing argument */
		if(tok == NULL)
		{
			MISSIGN_ARGUMENT_ERROR(fileName, lineCount);
			return 0;
		}

		/* analyzing the current operand */
		if(tok[0] == '#')/* instant addressing */
		{
			/* check that the current command allows instant addressing,
			check both for the source and target operands */
			if((!i && opcodeNum >= isOpcode("lea")) || (i && (opcodeNum != isOpcode("cmp") && opcodeNum != isOpcode("prn"))))
			{
				INVALID_OPERAND_TO_COMMAND_ERROR(fileName, lineCount, getOpcodeName(opcodeNum), tok);
				return 0;
			}

			convertOperandAddressingMethod(&currentCode, i, INSTANT_ADDRESSING);

			/* convert the operand to machine code */
			*currentOperandCode = abs(atoi(getPartialString(tok, 1, strlen(tok))));
			if(atoi(getPartialString(tok, 1, strlen(tok))) < 0){
				*currentOperandCode = ~(*currentOperandCode) + 1;/* complement 2 */
			}
			*currentOperandCode <<= OPERAND_VALUE_SHIFT;
			convertIncodingType(currentOperandCode, ABSOLUTE);
		}
		else if(tok[0] == '*' || tok[0] == 'r')/* indirect register addressing OR direct register addressing */
		{
			/* check that the current command allow the current operand type */
			if(tok[0] == '*' && !i && opcodeNum >= isOpcode("lea"))
			{
				INVALID_OPERAND_TO_COMMAND_ERROR(fileName, lineCount, getOpcodeName(opcodeNum), tok);
				return 0;
			}
			else if((tok[0] == 'r' && (((!i && opcodeNum >= isOpcode("lea"))) || 
				(i && (opcodeNum == isOpcode("jmp") || opcodeNum >= isOpcode("bne") || opcodeNum >= isOpcode("jsr"))))))
			{
				INVALID_OPERAND_TO_COMMAND_ERROR(fileName, lineCount, getOpcodeName(opcodeNum), tok);
				return 0;
			}			

			convertOperandAddressingMethod(&currentCode, i, 
				(tok[0] == '*') ? INDIRECT_REGISTER_ADDRESSING : DIRECT_REGISTER_ADDRESSING);

			/* remove the * if needed */
			if(tok[0] == '*'){
				tok = getPartialString(tok, 1, strlen(tok));
			}

			/* check operand syntax */
			if(strlen(tok) >= 2 && tok[0] == 'r' && 
				((registerNum = atoi(getPartialString(tok, 1, strlen(tok)))) >= 0 && registerNum < AMOUNT_OF_REGISTERS))
			{
				if(!i || (i && !firstOperandIsRegister)){/* source operand is a register OR target operand is a register and source operand isnt */
					firstOperandIsRegister = i? !i: !i;
					*currentOperandCode = registerNum;
					*currentOperandCode <<= OPERAND_VALUE_SHIFT * (2 - i);
					convertIncodingType(currentOperandCode, ABSOLUTE);
				}
				else/* both operands are registers, both of their values goes to the same code */
				{
					hasTargetOperand = 0;
					currentOperandCode = &sourceOperandCode;
					/* shift *currentOperandCode to remove the incoding, then shift again to return the 
						source operand bits to bits 8-5*/
					*currentOperandCode >>= OPERAND_VALUE_SHIFT;
					*currentOperandCode <<= OPERAND_VALUE_SHIFT;
					registerNum <<= OPERAND_VALUE_SHIFT;/* target operand value goes to bits 5-3 */
					*currentOperandCode |= registerNum;
					convertIncodingType(currentOperandCode, ABSOLUTE);
				}
			}
			else{
				INVALID_REGISTER_ERROR(fileName, lineCount);
				return 0;
			}
		}
		else/* direct addressing */
		{
			/* most of the work on label operands will be done in the second run.
				here we only set the correct address method in the command machine code */
			convertOperandAddressingMethod(&currentCode, i, DIRECT_ADDRESSING);
		}
		
		i++;
	}/* end of while */

	/* adding the instruction words to 'instructions' */
	addMachineCode(instructions, (*IC)++, currentCode);
	if(hasSourceOperand){
		addMachineCode(instructions, (*IC)++, sourceOperandCode);
	}
	if(hasTargetOperand){
		addMachineCode(instructions, (*IC)++, targetOperandCode);
	}

	return 1;	
}

/* convert the command line machine code addressing method */
void convertOperandAddressingMethod(unsigned short int *code, int i, int value)
{
	if(!i){/* source operand */
		convertSourceOperand(code, value);
	}
	else{/* target operand */
		convertTargetOperand(code, value);
	}
}
