#include <ctype.h>
#define SIZE_OF_LINE 81


int jumpSpace(char *, int );	/*Input: string and index in string
				Output: An integer representing an index in the string.
				Description: returns an index in the string in the place after the spaces that were after the string in the resulting index*/

int getCommand(char *,  int );	/*Input: string and index
				Output: An integer representing the index in the string at the position after the current command.
				A current command is one that starts at the string position at the received index and collects characters as long as they differ from a 				space.The command enters a global string*/

