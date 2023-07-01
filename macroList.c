#include "macroList.h"


void addToMacroList(char * name, ptrMacro *head)
{
	ptrMacro new;
	new=(ptrMacro) malloc(sizeof(macroLine)); 	/*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new macro \n");
	 	exit(0);
	}
	strcpy(new-> macroName, name);		/*Adds a new macro to the top of the list*/
	new->next = *head;
   	*head = new;
	new -> index = 0;
	return;
}







ptrMacro sameMacro(char * macro_name, ptrMacro head )
{
	while(head) 					
	{
		if(!strcmp(macro_name ,head -> macroName))	/*If the macro name is already defined*/
			return head;
		head = head-> next;
	}
	return NULL;
}


void freeMacroList(ptrMacro *head)			/*Memory release*/
{
	ptrMacro p;

	while(*head)
	{
        	p = *head;
		*head=p->next;
		free(p);
	}
}





void writesFile(FILE * file, char * line)
{
    int i=0;
    while( line[i]!='\0')
    {

        fputc(line[i], file);
        i++;
    }
}

