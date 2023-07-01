#include "AssemblerCrossing.h"
#include "convertText.h"
extern int flagSymbol;
extern char command[SIZE_OF_LINE];
int IC = 0, DC = 0;
char line[SIZE_OF_LINE];
extern int flagEx;
extern ptICList ptr;



void cross1(char * name_of_file, char * shortFileName)
{
	int row = 0, index = 0, startIndex = 0, opcode , error =0 , result =0,directiveType, entryFlag = 0 ;
	ptSymbol headSymbol = NULL;
	ptDCList headDcList = NULL, tailDcList = NULL;
	ptICList headIcList = NULL, tailIcList = NULL;
	FILE * file;
	IC = 0;
	DC = 0;
	file = fopen(name_of_file, "r");	/*open file*/
	if (!file)
	{
		printf("error - %s no such file or directory\n",name_of_file);
		return;
	}
	while(!feof(file)) {
        if(IC +DC +MEMORY_ACCESS >= SIZE_OF_RAM)	/*deviation from ram*/
        {
            printf("Memory exception");
            break;
        }
        row++;						/*current line*/
        flagSymbol = 0;
        index = 0;
        startIndex = 0;
        fgets(line, SIZE_OF_LINE, file);
        if (!feof(file)) {
            index = jumpSpace(line, index);
            if (line[index] != '\n' && line[index] != ';') {	/*A blank line or a comment line*/
                startIndex = index;
                index = getCommand(line, index);		/*At this point we have a word within the command*/
                result = islabel(command, row, startIndex, &headSymbol);
                if (flagSymbol) {				/*There was a label*/
                    index = jumpSpace(line, index);
                    index = getCommand(line, index);
                }
                if (result == -2)		/*error in file*/
                    error = 1;
                directiveType = isDirective(command, row, startIndex);	/*Checking if a directive*/
                opcode = isOrder(command);
                if (directiveType >= 0) {
                    if (flagSymbol) {
                        headSymbol->type = 1;
                        headSymbol->location = DC;
                    }
                    if (directiveType == 0) /* data*/
                        result = directiveData(line, index, row, &headDcList, &tailDcList);
                    else if (directiveType == 1) /* string*/
                        result = directiveString(line, index, row, &headDcList, &tailDcList);
                    else if (directiveType == 2) /* struct*/
                        result = directiveStruct(line, index, row, &headDcList, &tailDcList);
                    else if (directiveType == 3) /* entry*/
                    {
                        if(flagSymbol) {		/*A case of .entry being preceded by a label*/
                            ptDCList pt;
                            pt = headDcList;
                            headDcList = headDcList->next;
                            free(pt);
                        }
                        entryFlag = 1;
                    }
                    else if(directiveType == 4 )/* extern*/
                    {
                        if(flagSymbol) {	/*A case of .extern being preceded by a label*/
                            ptDCList pt;
                            pt = headDcList;
                            headDcList = headDcList->next;
                            free(pt);
                        }
                        result = directiveExtern(line, index, row, &headSymbol);
                    }
                    if (result == -2)		/*error in file*/
                        error = 1;
                } else if (opcode >= 0) {	/*if instruction*/
                    if (flagSymbol) {
                        headSymbol->type = 0;
                        headSymbol->location = IC + MEMORY_ACCESS;
                    }
                    if (opcode == 14 || opcode == 15)
                        result = instrucGroup3(opcode, line, index, row,&headIcList, &tailIcList);
                    else if ((opcode >= 0 && opcode <= 3) || opcode == 6)
                        result = instrucGroup1(opcode, line, index, row,&headIcList, &tailIcList);
                    else
                        result = instrucGroup2(opcode, line, index, row,&headIcList, &tailIcList);
                    if (result == -2)
                        error = 1;
                } else if(error != 1)		/*unknown word*/
                {
                    printf("error in row %d index  %d: Command %s - not recognized \n", row, startIndex, command);
                    error = 1;
                }
            }
        }
    }
	if(error==1)		/*There was an error in the file*/
	{
		freeListDC(&headDcList);
		freeListIC(&headIcList);
		freeSymbolList(&headSymbol);
        return;
	}
    addSymbolIc(&headSymbol);
    addIc(&headDcList);
    fclose(file);
    cross2(name_of_file , shortFileName, entryFlag, &headSymbol, &headDcList, &headIcList);
}

void cross2(char * name_of_file , char * shortFileName, int entryFlag,ptSymbol *headSymbol, ptDCList *headDcList, ptICList *headIcList )
{
	int row = 0, index = 0, startIndex = 0, opcode , error =0 , result =0,directiveType;
	char * label;
	FILE * file;
    ptr = *headIcList;
    file = fopen(name_of_file, "r");
    if (!file)
    {
        printf("error - %s no such file or directory\n",name_of_file);
        return;
    }
    while(!feof(file))
    {
        row++;		/*current row*/
        flagSymbol = 0;
        index = 0;
        startIndex = 0;
        fgets(line, SIZE_OF_LINE, file);
        if (!feof(file))
        {
            index = jumpSpace(line, index);
            if (line[index] != '\n' && line[index] != ';')
            {
                startIndex = index;
                index = getCommand(line, index);/*At this point we have a word within the command*/
                label = strrchr(command, ':');
                if(label)
                {
                    index = label - command + startIndex +1;
                    index = jumpSpace(line, index);
                    index = getCommand(line, index);
                }
                directiveType = isDirective(command, row, startIndex);
                if(directiveType >=0)				/*directive case*/
                {
                    if(directiveType == 3)
                    {
                        if(isEntry(line ,index, row,*headSymbol))
                            error = 1;
                    }
                }
                else						/*instruction case*/
                {
                    opcode = isOrder(command);
                    if (opcode == 14 || opcode == 15)
                       ptr = ptr -> next;
                    else if ((opcode >= 0 && opcode <= 3) || opcode == 6)
                        result = labelGroup1( line, index, row, *headSymbol);
                    else
                        result = labelGroup2( line, index, row, *headSymbol);
                }
                if(result == -2)	/*Error on the second pass*/
                    error = 1;
            }
        }
    }
    if(!error)	/*no error on the second pass*/
    {
        convertToObject(shortFileName,*headDcList, *headIcList);
    	if(flagEx == 1)
        	convertToExtern(shortFileName);
    	if(entryFlag == 1)
        	convertToEntry(shortFileName, *headSymbol);
    }
    
    freeListDC(headDcList);
    freeListIC(headIcList);
    freeSymbolList(headSymbol);
    freeExternalList();
    fclose(file);
}
