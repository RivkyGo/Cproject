#include "checkText.h"
#define SIZE_OF_RAM 255

void cross1(char *, char *);	/*Input: a string containing the name of the file with an extension .am,
					 a string containing the name of the file without an extension
				  Output:no output*/

void cross2(char *, char *,int, ptSymbol*, ptDCList *, ptICList *);/*Input: a string containing the name of the file with an extension .am,
									 a string containing the name of the file without an extension,
									 An integer that shows whether there was an entry (on - 1) or not (off - 0)
									a pointer to the head of the symbol table
									a pointer to the head of the The data structure
									a pointer to the head of the structure of the instructions
								  Output: no output*/
