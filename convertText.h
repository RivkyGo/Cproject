#ifndef CONVERTTEXT_H
#define CONVERTTEXT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "checkText.h"
#include "AssemblerCrossing.h"
#include "Input.h"

#define SIZE_BASE 32


char* concat(const char *s1, const char *s2);       	/* input: two strings.  
							Output: one string that is the combination of both strings*/

void ConvertToBase(int);				/*Input: Integer.
							Output: no output.
							Description: converts the 10 bits of the number (lsb) to base 32 and puts it in a global string*/

void convertToObject(char * ,ptDCList ,ptICList);	/*Input: filename without extension
							a pointer to the head of the The data structure
							a pointer to the head of the structure of the instructions
							Output: no output.
							Description: The function creates an object (.ob) file as required*/

void convertToEntry(char *,ptSymbol );			/*Input: filename without extension
							a pointer to the head of the symbol table
							Output: no output.
							Description: The function creates an entry (.ent) file as required*/


void convertToExtern(char * );				/*Input: filename without extension
							Output: no output.
							Description: The function creates an extern (.ext) file as required*/


#endif
