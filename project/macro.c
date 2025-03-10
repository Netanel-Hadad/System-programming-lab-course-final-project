#include "macro.h"

/* create and add a new macro node to the macro table */
void addToMacroTable(Macro **head, char *name, char *content)
{
	Macro *newMacro, *last;

	ALLOCATE_MEMORY(newMacro, Macro, sizeof(Macro))	
	newMacro->name = name;
	newMacro->content = content;
	newMacro->next = NULL;

	last = *head;  
    if (*head == NULL)
    {
       *head = newMacro;
       return;
    }  
      
    while (last->next != NULL)
        last = last->next;
  
    last->next = newMacro;
}

/* given a macro name, return its content. NULL if doesnt exist */
char *getMacroContent(Macro *head, char *name)
{
	Macro *current = head;
	while(current != NULL)
	{
		if(current->name != NULL && strcmp(current->name, name) == 0){
			return current->content;
		}
		current = current->next;
	}
	return NULL;
}

/* create the macro table */
int handleMacro(Macro **head, char *fileName, FILE *asFile)
{
	char line[MAX_LINE_LENGTH], lineCopy[MAX_LINE_LENGTH];
	char *tok, *name = NULL, *content = NULL, *temp = NULL, flag = 0, passedMacroStage = 1;
	int lineCount = 1;	

	/*
	flag = 1 => reading part of a a macro decleration
	flag = 0 => current line is not a part of a macro decleration
	*/

	while(fgets(line, MAX_LINE_LENGTH, asFile))
	{
		/* skip empty line */
		if(isEmptyString(line)){
			lineCount++;			
			continue;
		}

		strcpy(lineCopy, line);
		tok = strtok(line, " \t\n\0");/* tok is the first word of the line */
		if(flag)
		{
			if(strcmp(tok, MACRO_DECLERATION_END) == 0)/* found 'endmacr' */
			{
				flag = 0;
				/* check for extraneus text after 'endmacr' */
				if((tok = strtok(NULL, " \n\0")) != NULL){
					EXTRANEOUS_TEXT_ERROR(fileName, lineCount);
					FAILED_MACRO_STAGE;
				}

				/* adding the new macro to the macro table */
				if(content == NULL)
				{
					/* the macro was decleared with out any content,
						so we will create a placeholder value for content */
					ALLOCATE_MEMORY(content, char, 2)
					strcpy(content, "A");
				}

				content[strlen(content) - 1] = '\0';
				addToMacroTable(head, name, content);

				name = NULL;
				content = NULL;
			}
			else/* line is part of a macro decleration, so we need to add it to 'content' */
			{
				if(passedMacroStage)/* add current line to content only if file has yet to fail */
				{
					if(content == NULL)
					{
						ALLOCATE_MEMORY(content, char, strlen(lineCopy))
						strcpy(content, lineCopy);
					}
					else
					{
						if((temp = (char *)realloc(content, strlen(content) + strlen(lineCopy))) == NULL){
							MEMORY_ALLOCATION_ERROR;
						}
						content = temp;
						strcat(content, lineCopy);
					}
				}
			}
		}
		else/* flag = 0 */
		{
			if(strcmp(tok, MACRO_DECLERATION_START) == 0)/* found 'macr' */
			{	
				flag = 1;
				tok = strtok(NULL, " \t\n\0");
				if(isValidMacroName(*head, tok, fileName, lineCount))
				{
					ALLOCATE_MEMORY(name, char, (strlen(tok) + 1))
					strcpy(name, tok);

					/* check for extraneous text after the macro name */
					if((tok = strtok(NULL, " \n")) != NULL){
						EXTRANEOUS_TEXT_ERROR(fileName, lineCount);
						FAILED_MACRO_STAGE;
					}
				}
				else/* invalid macro name */
				{
					lineCount++;
					continue;
				}
			}
			else if(isMacroName(*head, tok)){/* line starts with macro definition */
				if((tok = strtok(NULL, " \n")) != NULL){/* check for extraneuos text */
					EXTRANEOUS_TEXT_ERROR(fileName, lineCount);
					FAILED_MACRO_STAGE;
				}
			}
		}
		lineCount++;
	}

	/* if found no errors at the macro stage and created the macro table,
	proced to create the '.am' file */
	if(passedMacroStage){
		writeAMfile(head, fileName, asFile);
	}

	return passedMacroStage;
}

/* free memory allocated for the macro table */
void freeMacroTable(Macro *head)
{
	Macro *current;

	while(head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}
}

/* returns 1 if the name in the macro declereation is a valid name */
int isValidMacroName(Macro *head, char *name, char *fileName, int line)
{
	/* macro name cant have the same name as another macro, or a valid command, 
		instruction, 'macr'/'endmacr', register or a value */
	if(isMacroName(head, name)){
		MACRO_NAME_IN_USE_ERROR(fileName, line);
		return 0;
	}
	else if(isOpcode(name) != -1){
		NAME_IS_VALID_COMMAND_ERROR(fileName, line);
		return 0;
	}
	else if(isInstruction(name)){
		NAME_IS_VALID_INSTRUCTION_ERROR(fileName, line);
		return 0;
	}
	else if(strcmp(name, MACRO_DECLERATION_START) == 0 || strcmp(name, MACRO_DECLERATION_END) == 0){
		NAME_IS_MACRO_DECLERATION_COMMAND_ERROR(fileName, line);
		return 0;
	}
	else if(name[0] == 'r' && (name[1] >= '0' && name[1] < (AMOUNT_OF_REGISTERS + '0')) && strlen(name) == 2)
	{
		NAME_IS_REGISTER_ERROR(fileName, line);
		return 0;
	}
	else if(name[0] == '#')
	{
		NAME_IS_VALUE_ERROR(fileName, line);
		return 0;
	}
	return 1;
}

/* create the '.am' file */
void writeAMfile(Macro **head, char *fileName, FILE *asFile)
{
	char line[MAX_LINE_LENGTH], lineCopy[MAX_LINE_LENGTH];
	char flag = 0;
	char *amFileName, *tok;
	FILE *amFile;

	/*
	flag = 1 => dont copy current line from the '.as' file to the new '.am' file
	flag = 0 => copy current line from the '.as' file to the new '.am' file
	*/

	rewind(asFile);

	/* create '(fileName).am' */
	OPEN_FILE(amFile, amFileName, fileName, AM_FILE_ENDING, "w");
	
	while((fgets(line, MAX_LINE_LENGTH, asFile)))
	{
		if(!isEmptyString(line))/* skip empty line */
		{
			strcpy(lineCopy, line);
			tok = strtok(line, " \t\n\0");

			if(flag)
			{
				if(strcmp(tok, MACRO_DECLERATION_END) == 0){/* found 'endmacr' */
					flag = 0;
				}
			}
			else/* flag = 0 */
			{
				if(strcmp(tok, MACRO_DECLERATION_START) == 0){/* found 'macr' - start of a macro decleration */
					flag = 1;
				}			
				else if(isMacroName(*head, tok))/* macro definition - write its content in the '.am' file */
				{
					fprintf(amFile, "%s\n", getMacroContent(*head, tok));
				}
				else{/* regular line, copy it to the '.am' file*/
					fprintf(amFile, "%s", lineCopy);
				}
			}
		}
	}

	fclose(amFile);
}

/* check by name if a macro exists */
int isMacroName(Macro *head, char *name)
{
	Macro *current = head;

	while(current != NULL)
	{
		if(current->name != NULL && strcmp(current->name, name) == 0){
			return 1;
		}
		current = current->next;
	}

	return 0;
}

