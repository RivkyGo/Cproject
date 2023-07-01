#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_MACRO_NAME 81
#define SIZE_CONTENT 810



typedef struct  MacroList * ptrMacro;

typedef struct MacroList{
    char content[SIZE_CONTENT];
    int index;
    char macroName[SIZE_MACRO_NAME];
    ptrMacro next;
}macroLine;




void writesFile(FILE *, char *);		/*input: Opens the file for updating and inserts a line into it*/

void addToMacroList(char *, ptrMacro *); 	/* input:pointer to pointer that point the head of list, name of macro, pointer to text file with the content of the 								 macro*/

void freeMacroList(ptrMacro *);			/*input:pointer to pointer that point the head of list*/

ptrMacro sameMacro(char *,  ptrMacro);		/* input: string- name of macro,pointer to the head of the list
						   Output: '1'if there was a macro with the same name as the string in the list of macros and another '0'*/
