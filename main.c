#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "convertText.h"
#include "macroList.h"
#define SIZE_OF_LINE 81


int macroRetir(FILE *, FILE *);	/*input: A pointer to a source file, a pointer to a target file*/
extern int flagEx;
enum bool {false,true};
enum bool error = false;



int main(int argc, char const *argv[])
{
	FILE * file;
	FILE * newFile;
	char * fullFileName = NULL,* fullFileNewName =NULL,*fileName;
	int i, result;
	if(argc <= 1)						/*If no files were received*/
	{
		printf("No File where typed.\n");
		exit(0);
	}
	
	for(i=1; i<argc; i++)
	{
		flagEx=0;
		fullFileName = NULL;
		fullFileName = concat(argv[i],".as\0");		/*File extension*/

		if(!rename( argv[i], fullFileName))
		{ 
			printf(" -- File: %s -- \n",fullFileName);/**/
			file = fopen(fullFileName, "r");
			if(!file)
			{
				printf("Can not open %s permission denied\n",fullFileName);
				exit(0);
			}
			fullFileNewName = concat(argv[i],".am\0");/*File extension*/
			newFile = fopen(fullFileNewName, "w");
			if(!newFile)
			{
				printf("Can not open %s permission denied\n",fullFileNewName);
				exit(0);
			}
		    
		        result=macroRetir(file, newFile);		/*Macro retirement*/
			
		      	fclose(file);
			fclose(newFile);
			free(fullFileName);
			if(result)
				break;
			fileName = malloc(strlen(argv[i]));
			if(!fileName)
			{
				printf("memory allocation failed\n");
				exit(0);
			}
			strcpy(fileName,argv[i]);
			cross1(fullFileNewName ,fileName );	/*First assembler pass*/
			free(fullFileNewName);
			free(fileName);
		}
		else
			printf("No such file or directory %s .\n", argv[i]);
	}
    return 0;
}

int macroRetir (FILE *source, FILE *target)
{
	int i = 0, row = 0, index=0;
	char buffer[SIZE_OF_LINE], firstWord[SIZE_OF_LINE];
	ptrMacro pt, headMacroList = NULL;
	enum bool macro_flag = false;
	while (fgets(buffer, SIZE_OF_LINE, source)!= NULL)
	{
		row++;
		index = 0;
		i=0;
		if (!feof(source))
		{
			index = jumpSpace(buffer, index);
			while (index < SIZE_OF_LINE && !isspace(buffer[index])  ) {	/*Collecting the first word*/
		        	firstWord[i] = buffer[index];
		        	i++;
		        	index++;
		    	}
			firstWord[i] = '\0';
			if (!strcmp(firstWord, "macro"))
			{
				if (macro_flag == false) {				/*Not a nested macro*/
		            	macro_flag = true;
		            	i = 0;
		           	index = jumpSpace(buffer, index);
		            	while (!isspace(buffer[index]) && i < SIZE_OF_LINE) {	/*Collecting the second word*/
				        firstWord[i] = buffer[index];
				        i++;
				        index++;
		            	}
				firstWord[i] = '\0';
				if(isOrder(firstWord)>=0 || isRegister(firstWord) != -1|| isPureDirective(firstWord)>=0)	/*correctness check*/
				{
					printf("error in line %d index %d-Defining a macro in the name of a reserved word in the language",row, index);
		                	error = true;
				}
				if (!strcmp(firstWord, "")) {
				        printf("error in line %d - macro statement without name.\n", row);
				        error = true;
		            	}
				    else if (sameMacro(firstWord,headMacroList))
				    {
				        printf("error in line %d index %d - Conflict in the macro type, \na macro with the same name was previously defined",
				               row, index);
				        error = true;
				    }
				    index = jumpSpace(buffer, index);
				    if (buffer[index] != '\n') {
				        printf("error in line %d index %d - excess characters after the macro name", row, index);
				        error = true;
				    }
				    if(!error)
				        addToMacroList(firstWord,&headMacroList);

		        }
		        else
		        {
		            printf("error in line %d index %d - set up a nested macro.", row, index);
		            exit(0);
		        }
		    }
		    else if (!strcmp(firstWord, "endmacro"))							/*correctness check*/
		    {
		        if (macro_flag == false) {
		            printf("error in line %d index %d - attempt to close undeclared macro.", row, index);
				error=true;
		        }
		        else {
		            index = jumpSpace(buffer, index);
		            if (buffer[index] != '\n') {
		                printf("error in line %d index %d - excess characters after the endmacro.\n", row, i);
				error=true;
		            }
		            else {										/*Finish writing the macro content*/
		                headMacroList->content[headMacroList->index] = '\0';
		                headMacroList->index = headMacroList->index + 1;
		                macro_flag = false;
		            }
		        }
            }
            else if (macro_flag == true )
            {
                if(!error) {
                    i = 0;
                    while (i < SIZE_OF_LINE && buffer[i] != '\n' && headMacroList->index < SIZE_CONTENT) {
                        headMacroList->content[headMacroList->index] = buffer[i];
                        i++;
                        headMacroList->index = headMacroList->index + 1;
                    }
                    headMacroList->content[headMacroList->index] = '\n';
                    headMacroList->index = headMacroList->index + 1;
                }
            }
            else
            {
                pt = sameMacro(firstWord, headMacroList);
                if (pt)
                    writesFile(target, pt->content);
                else
                    fputs(buffer, target);
            }

        }
	if(i == SIZE_OF_LINE && buffer[i-1] != '\n')
	{
		printf("error in line %d - The line is longer than the allowed length\n", row);
		error=true;
	}
    }
    fputc('\n', target);
    freeMacroList(&headMacroList);
	return error;
}
















