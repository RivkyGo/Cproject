#include "Input.h"
char command[SIZE_OF_LINE];


int jumpSpace(char * line, int index)
{
	char c;
	while((c = line[index]) != '\n' && isspace(c))
		index++;
	return index;
}


int getCommand(char * line, int index)
{
	int i =0;
	while(!isspace(line[index]))		/*collects the command*/
	{
		command[i] = line[index];
		index ++;
		i ++;	
	}
	command[i] = '\0';
	return index;
}
