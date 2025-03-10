#include "label.h"

/* add a new label to a labels array */
void addToLabels(label **head, char *name, int value, enum location location, enum type type)
{
	/* adding a new label to be the head of the list */
	label *temp;
	ALLOCATE_MEMORY(temp, label, sizeof(label));
	strcpy(temp->name, name);
	temp->value = value;
	temp->_location = location;
	temp->_type = type;
	temp->next = *head;
	*head = temp;
}

/* return 1 if 'name' is a name of a exitsing label */
int isLabelExist(label *head, char *name)
{
	label *current = head;

	while(current!= NULL){
		if(strcmp(current->name, name) == 0){
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/* return 1 if 'name' is a valid name for a new label */
int isValidLabelName(label *head, char *name, Macro *macro)
{
	int i = 0;
	/* name cant contain spaces or tabs */
	while(i < strlen(name)){
		if(name[i] == ' ' || name[i] == '\t'){
			return 0;
		}
		i++;
	}
	/* a new label name cant be name as a label that already exist, opcode, instruction, macro or 
		be longer then a label name max length (LABEL_NAME_MAX_LENGTH) */
	return (!isSavedWord(name) && !isLabelExist(head, name) && !isMacroName(macro, name) 
		&& (strlen(name) <= LABEL_NAME_MAX_LENGTH));
}

/* given a label name, return its value. return -1 if label wasnt found */
int getLabelValue(label *head, char *name)
{
	label *current = head;
	while(current != NULL){
		if(strcmp(current->name, name) == 0){
			return current->value;
		}
		current = current->next;
	}
	return -1;
}

/* return a label by a given name */
label *getLabel(label *head, char *name)
{
	label *current = head;
	while(current != NULL){
		if(strcmp(current->name, name) == 0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/* free a linked list of labels */
void freeLabels(label *labels)
{
	label *current;
	while(labels != NULL)
	{
		current = labels;
		labels = labels->next;
		free(current);
	}
}

/* reverse a labels linked list */
void reverseList(label **head)
{
	label *prev = NULL, *current = *head, *next;
    while(current != NULL) 
	{
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}
