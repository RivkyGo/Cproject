#include "checkText.h"
enum status {OFF,ON};
enum ARE {A, E, R , entry};

extern int IC;
extern int DC;
ptExtern headExternList = NULL;
ptICList ptr = NULL;
int flagEx =0;
enum status flagSymbol = OFF;

char *directives[5] = {"data\0","string\0","struct\0","entry\0","extern\0"};
char *orders[16] = {"mov\0","cmp\0","add\0","sub\0","not\0","clr\0","lea\0","inc\0","dec\0","jmp\0","bne\0","get\0","prn\0","jsr\0","rts\0","hlt\0"};
char *registers[8] = {"r0" , "r1" , "r2" , "r3" , "r4" , "r5" , "r6" , "r7"};

int isDirective (char *command, int row, int index)   /*A function that checks if the word is a prompt command*/
{
	int i;
	char *pt;
	if(command[0] == '.')  /*Checks if the first character is a period*/
	{
		pt=command;   /*A pointer that points to the first position in the array*/
		pt = pt + 1;  /* The array of commands will have a function without the dot*/
		for (i=0;i<5;i++)  /*Checks if the word is one of 5 prompt commands*/
		{
			if(strcmp(pt, directives[i])==0)
		        	return i;  /*return the index of directive sentence*/
		}
		printf("error in row %d index  %d - %s is not a directive sentence\n",row, index+1 ,pt);
		return -2;   /*When there is an error in the prompt it will return -2*/
	}
	else 
	{
		for (i=0;i<5;i++) /*If there is no period, it checks whether it is a directive sentence that is a reserved word that cannot be used as a label*/
		{
			if(strcmp(command, directives[i])==0)
			{
				printf("Error in row %d index  %d: missing a point before the directive statement\n",row, index);
		        	return -2;  /*If it is one of the directive sentences it will return -2*/
			}
		}
	}
	return -1; /*when it is not a prompt statement will return -1*/          
}

int isPureDirective(char * command)  /*A function that checks whether it is a prompt word*/
{
    int j=0;
    while(j<5)
    {
        if(!strcmp(command ,directives[j]))
            return j;    /*If it is a directive statement returns the index of the directive statement*/
        j++;
    }
    return -1;
}

int isOrder (char *command)  /*A function that checks whether the word is an instruction word*/
{
	int i;
	for (i=0;i<16;i++)
		{
			if(strcmp(command, orders[i])==0)
		        	return i;  /*return the opcode */
		}
	return -1; /*Not instruction sentence*/
}

int islabel(char *command, int row, int index,ptSymbol *headSymbol)  /*A function that checks whether the word is a label*/
{
	int i=0,j=0, last;
	char *flagColon;
	flagColon = strrchr(command, ':');  /*Returns the last occurrence of the character*/
	if(flagColon)  /*If there was a colon then it could be a label*/
	{
        last = flagColon - command;  /*Saves an index of the position of the colons*/
		if(command[last+1]!= '\0') /*Checks if there is no end-of-string character after the occurrence of the colon, i.e. there are more characters after the label*/
		{
			printf("error in row %d index  %d: %s - does not match any sentence syntax\n",row, index, command);
			return -2;
		}
        command[last]='\0'; /*Instead of the semicolon put a string termination character*/
		if(!isalpha(command[0])) /*Checks if the first word is a letter*/
		{
			printf("error in row %d index  %d: %s - in the  first character in the label no letter was found\n",row, index, command);
			return -2;
		}		
		while(i < last)  /*Run the array until the end of string character*/
		{
			if(!isalpha(command[i]) && !isdigit(command[i])) /*Checks if the characters are neither letters nor numbers*/
			{
				printf("error in row %d index  %d: %s - non-number and non-letter character was found\n",row, index, command);
				return -2;
			}
			i++;
		}
		if(isOrder(command) != -1) /*If it's an instruction sentence, that means a reserved word*/
		{
			printf("error in row %d index  %d: %s - An attempt to define a label in an assembly language reserved word\n",row, index, command);
			return -2;
		}
		if(strlen(command)>30) /*Label size greater than 30*/
		{
			printf("error in row %d index  %d: %s - The length of the label is greater than 30\n",row, index, command);
			return -2;
		}
		if(sameSymbol(command, *headSymbol)) /*Checks whether the label is reused*/
		{
			printf("error in row %d index  %d: %s - Restatement of label in use detected\n",row, index, command);
			return -2;
		}
		if(isRegister(command) != -1) /*Checks whether the label is a register ie a reserved word*/
		{
			printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, command);
			return -2;
		}
		while(j<5)
		{
			if(!strcmp(command ,directives[j]))  /*Checks if the word is a prompt - a reserved word*/
			{
				printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, command);
				return -2;
			}
			j++;
		}	 	 
		addToSymbolList(command, headSymbol); /*A valid label is inserted into the symbol table*/
		flagSymbol = ON;
		return index;
		
	}
	return -1; /* it is not a label*/
	
}

void addToSymbolList(char * name, ptSymbol * headSymbol)  /*A function that adds a label name to the symbol table*/
{
	ptSymbol new; /*New cell in the symbol table*/
	new=(ptSymbol) malloc(sizeof(symbol)); /*Allocation of memory*/
	if(!new) /*The assignment was successful*/
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}

	strcpy(new-> label_name , name); /*Copies the label name to the symbol table*/
	
	new->next = (*headSymbol);
	(*headSymbol) = new;
	new -> typeE = R; /*The content of the word depends on the memory location where it will actually be loaded The machine code of a target at the time of its execution*/
	return;
}

void addToExternalList(ptSymbol p, int location)
{
    ptExtern new;  /*New cell in the symbol table*/
    new=(ptExtern) malloc(sizeof(externList)); /*Allocation of memory*/
    if(!new)  /*The assignment was successful*/
    {
        printf("\n cannot allocate memory to a new symbol \n");
        exit(0);
    }

    strcpy(new-> label_name , p ->label_name );  /*Copies the label name to the extern table*/
    new ->location = location;
    new->next = headExternList;
    headExternList= new;
    return;
}

int sameSymbol(char * label_name, ptSymbol headSymbol)
{
	ptSymbol p = headSymbol;
	while(p != NULL) 					/*As long as p1 is not equal to null*/
	{
		if(!strcmp(label_name ,p -> label_name))  /*If there is an identical label*/
			return -2;
		p = p-> next;
	}
	return 0; /*No identical label exists*/
	
}


void freeSymbolList(ptSymbol * headSymbol)			/*Memory release*/
{
	ptSymbol p;
	while(*headSymbol)
	{
		p = *headSymbol;
		*headSymbol = p->next;
		free(p);
	}
}

void freeExternalList()			/*Memory release*/
{
    ptExtern p;
    while(headExternList)
    {
        p = headExternList;
        headExternList = p -> next;
        free(p);
    }
}
int isRegister(char * command)
{
	int i;
	for(i=0; i<8;i++)
	{
		if(strcmp(command, registers[i])==0)  /*Goes through the array of registers and checks if it is the name of a register*/
			return i;
	}
	return -1;
}

int correctDigit( char * command)
{
	int i=0;
	if(command[i] == '-' || command[i] == '+')
		i++;
	while(command[i] != '\0')
	{
		if(!isdigit(command[i]))
			return 0;
        i++;
	}
	if(!strcmp(command,""))
		return 0;   /*If it's not a valid number*/
	return 1;  /*If it's a valid number*/
}

void addToListIC(ptICList *headIcList,ptICList *tailIcList )
{
	ptICList new;
	new=(ptICList) malloc(sizeof(listIC)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}
	new -> next =NULL;
	if(*headIcList == NULL)
	{
		*headIcList = new;
		*tailIcList = *headIcList;	
	}
	else
	{
	(*tailIcList)->next = new;
	(*tailIcList) = new;
	}
	(*tailIcList)-> decimalAddress = IC + MEMORY_ACCESS;
	IC++;
	return;
}

void freeListIC(ptICList *headIcList)			/*Memory release*/
{
	ptICList p;
	while(*headIcList)
	{
		p = *headIcList;
		*headIcList=p->next;
		free(p);
	}
}


int address(char * word)   /*Classified for addresses*/
{
	int i=0;
	char * ptr;
	if(word[0]  == '#')  /*Can be addressed immediately*/
	{
		if(!correctDigit(word+1))  /*Checks if it's a valid number*/
			return -2;
		else
			return 0; /*addressed immediately*/
	}
	if(word[0] == 'r') /*Could be a register address*/
	{
		if(isRegister(word) != -1)
			return 3;  /*register address*/
	}
	ptr =strchr(word, '.');  /*Looking for the first place that has a dot*/
	if(ptr)  /*If there was a dot it could be a struct*/
	{
		i = 0;
		if((*(ptr+1)=='1'||*(ptr+1)=='2') && *(ptr+2) =='\0'&& isalpha(word[0]))
			while(word[i]!='.')
			{
				if(!isalpha(word[i]) && !isdigit(word[i]))  /*Checks whether the struct label is correct - contains letters or numbers*/
					break;
				i++;
			}
		if(word[i] =='.') /*If there was a dot then it could be a struct*/
			return 2; /*struct address*/
		return -2;
	}
	if(isalpha(word[0]))  /*Checks if the first latter is a letter*/
	{
		i = 0;
		while(word[i]!='\0')
		{
			if(!isalpha(word[i]) && !isdigit(word[i]))  /*Checks whether the struct label is correct - contains letters or numbers*/
				break;
			i++;
		}
		return 1;	/*Label address*/
	}
	return -2;	
}


int instrucGroup1(int opcode, char *line, int index,int row,ptICList *headIcList,ptICList *tailIcList)  /*A function that accepts lines followed by two operands*/
{
	
	char rs[SIZE_STRUCT] , rt[SIZE_STRUCT];
	int i = 0, result = 0, addressS, addressT;
	index = jumpSpace(line,index);
	while (line[index] != ','  && !isspace(line[index])) /*Copies the first word to the array*/
	{
		rs[i] = line[index];
		i++;
		index++;
	}
	rs[i] = '\0'; /*put at the end of the word a string termination character*/
	index = jumpSpace(line,index);
	if(line[index] == '\n') /*If no two operands were received*/
	{
		printf("in the row %d in the index %d in commands of this type, two operands must be written\n", row, index);
		result = -2;
	}
	else if(line[index] != ',') /*If there is no comma separating the two operands*/
	{
		printf("a comma is missing in the row %d in the index %d \n", row, index);
		result = -2;
	}
	else
	{
		i = 0;
        index++;
        index = jumpSpace(line,index);
		while(!isspace(line[index]) && i< SIZE_STRUCT)   /*Copies the second word to the array*/
		{
			rt[i] = line[index];
			i++;
			index++;
		}
		rt[i] = '\0';  /*put at the end of the word a string termination character*/
		index = jumpSpace(line,index);
		if(line[index] != '\n')  /*If the end of the line is not reached*/
		{
			printf("in the row %d in the index %d There are more characters after the operands\n ", row, index);
			result = -2;
		}	
	}
	if(result == -2)
		return -2;  /* error in the sentence*/
	addressS = address(rs); /* which address is it*/
	addressT = address(rt); /* which address is it*/
	if(addressS == -2 || addressT == -2)
	{
		printf("Error in line %d One of the operands is invalid\n", row);
		return -2;
	}
	if((opcode == 0 || opcode == 2 || opcode == 3 || opcode == 6) && addressT == 0 )		
	{
		printf("Error in line %d Target operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	if(opcode == 6 && (addressS == 0 || addressS == 3))
	{
		printf("Error in line %d source operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	addToListIC(headIcList, tailIcList); /*Opens a new cell in the instruction structure*/
	(*tailIcList) ->ramWord.opcode = opcode;
	(*tailIcList) ->ramWord.addressS = addressS;
	(*tailIcList) ->ramWord.addressT = addressT;
	(*tailIcList) ->ramWord.are = A;
	(*tailIcList) ->ramWord.wordType = 0; /*opcode,addressS,addressT,are*/
	memoryCoding(addressS , addressT, rs, rt,headIcList,tailIcList);
	return 0;
}


void memoryCoding(int addressS , int addressT, char * rs, char * rt, ptICList* headIcList, ptICList* tailIcList)
{
	if(strcmp(rs, "") && addressS == 0) /*Immediate response*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.number = atoi(rs+1);/*A number without an asterisk*/
		(*tailIcList) ->ramWord.wordType = 1;
		(*tailIcList) ->ramWord.are = A;
	}
	else if(addressS == 1)  /*label address*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
        	(*tailIcList) ->ramWord.number =0;
	}
	else if(addressS == 2)  /*struct address*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
        	(*tailIcList) ->ramWord.number =0;
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
		(*tailIcList) ->ramWord.number = (int)(rs[strlen(rs)-1]-48);
		(*tailIcList) ->ramWord.are = A;
	}
	else if(addressS == 3)  /*register address*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 2;
		(*tailIcList) ->ramWord.rs = isRegister(rs);
		(*tailIcList) ->ramWord.rt = 0;
		(*tailIcList) ->ramWord.are = A;
	}

	/* Target*/
	if(addressT == 0)  /*Immediate response*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.number = atoi(rt+1);
		(*tailIcList) ->ramWord.wordType = 1;
		(*tailIcList) ->ramWord.are = A;
	}
	if(addressT == 1)   /*label address*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
	}
	if(addressT == 2)  /*struct address*/
	{
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
		addToListIC(headIcList, tailIcList);	
		(*tailIcList) ->ramWord.wordType = 1;
		(*tailIcList) ->ramWord.number = (int)(rs[strlen(rs)-1]);
		(*tailIcList) ->ramWord.are = A;
	}
	if(addressT == 3)   /*register address*/
	{
		if(addressS == 3)	 /*register address*/
			(*tailIcList) ->ramWord.rt = isRegister(rt);
		else
		{
			addToListIC(headIcList, tailIcList);	
			(*tailIcList) ->ramWord.wordType = 2;
			(*tailIcList) ->ramWord.rt = isRegister(rt);
			(*tailIcList) ->ramWord.rs = 0;
			(*tailIcList) ->ramWord.are = A;
		}
	}
}


int instrucGroup2(int opcode, char *line, int index,int row, ptICList* headIcList,  ptICList* tailIcList) /*Instruction statements that accept one operand*/
{
	
	char rt[SIZE_STRUCT];
	int i = 0, addressT;
	index = jumpSpace(line,index);
	while(!isspace(line[index]))  /*Copy the first word*/
	{
		rt[i] = line[index];
		i++;
		index++;
	}
	rt[i] = '\0';  /*put at the end of the word a string termination character*/
	index = jumpSpace(line,index);  /*Cleans spaces*/
	if(line[index] != '\n') /*If the end of the line is not reached*/
	{
		printf("in the row %d in the index %d extra text after the operand\n", row, index);
		return -2;
	}
	addressT = address(rt); /* which address is it*/
	if(addressT == -2)
	{
		printf("Error in line %d the operands is invalid\n", row);
		return -2;
	}
	if((opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 10 || opcode == 11 || opcode == 13) && addressT == 0)		
	{
		printf("Error in line %d Target operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	addToListIC(headIcList, tailIcList);  /*Opens a new cell in the instruction structure*/
	(*tailIcList) ->ramWord.opcode = opcode;
	(*tailIcList) ->ramWord.addressS = 0;
	(*tailIcList) ->ramWord.addressT = addressT;
	(*tailIcList) ->ramWord.are = A;
	(*tailIcList) ->ramWord.wordType = 0;
	memoryCoding( 0 , addressT, "", rt, headIcList, tailIcList);
	return 1;
}


int instrucGroup3(int opcode, char *line, int index,int row, ptICList* headIcList,  ptICList* tailIcList)  /*Instructions that are not followed by an offset*/
{
	index = jumpSpace(line,index);
	if(line[index] != '\n')
	{
		printf("in the row %d in the index %d extra text after the operand\n", row, index);
		return -2;
	}
	addToListIC(headIcList, tailIcList);  /*Opens a new cell in the instruction structure*/
	(*tailIcList) ->ramWord.opcode = opcode;
	(*tailIcList) ->ramWord.addressS = 0;
	(*tailIcList) ->ramWord.addressT = 0;
	(*tailIcList) ->ramWord.are = A;
	(*tailIcList) ->ramWord.wordType = 0;
	return 1;
}


int directiveData(char * line , int index , int row,ptDCList *headDcList, ptDCList *tailDcList) /*.data sentences*/
{
 	
	int i=0, num , commaFlag=0 ,dataFlag = 0;
	char number[SIZE_OF_LINE];
	index = jumpSpace(line,index);
	while(line[index] !='\n' )
	{
        i = 0;
		while(line[index] != ','&& !isspace(line[index])) /*Copy the first word*/
		{
            dataFlag = 1;
			number[i] = line[index];
			i++;
			index++; 
		}
		number[i] = '\0';/*put at the end of the word a string termination character*/
		index = jumpSpace(line,index);
		if(strcmp(number,""))
			commaFlag=0;
		if(correctDigit(number))  /*Checks whether the number is correct*/
		{
			num=atoi(number); /*Converts to a number*/
			addToListDC(headDcList, tailDcList); /*Opens a cell in the data table*/
			(*tailDcList) ->ramWord.string = num;
			(*tailDcList) ->ramWord.wordType =3;
			if(line[index] == ',')
			{
				commaFlag = 1;
				index++;
			}
            index = jumpSpace(line,index);
		}
		else 
		{
			printf("in the line %d in the index %d The number is incorrect\n", row, index);
			return -2;
		}
	}
    if(dataFlag == 0)  /*If there was no comma between number and number*/
    {
        printf("in the line %d in the index %d Missing numbers after .data\n", row, index);
        return -2;
    }
    index = jumpSpace(line,index);
	if(line[index] == '\n' && commaFlag)  /*If the line ends and there was a comma at the end*/
	{
		printf("in the line %d in the index %d The syntax of the word is incorrect,there is an unnecessary comma at the end of the sentence\n", row, index);
		return -2;
	}
	return 0;
}

	
int directiveString(char * line , int index , int row, ptDCList *headDcList,ptDCList *tailDcList )  /*.string sentences*/
{
	int start, end;
	index = jumpSpace(line,index);
	start = strchr(line, '"') - line;  /*The first show there is " */
	end = strrchr(line, '"')-line;     /*The last show there is " */
	if( start == end)  /*The string is invalid*/
	{
		printf("in the row %d in the index %d The syntax of the string is incorrect\n", row, index);
		return -2;
	}
	index++;
	while(index < end)  /*	as long as they are inside the string  */
	{
		if(!isprint(line[index]))  /*If there is a character in the string that is not a printable character*/
		{
			printf("in the row %d in the index %d There is a character that cannot be printed\n", row, index);
			return -2;
		}
		addToListDC(headDcList, tailDcList);  /*Opens a cell in the data table*/
		(*tailDcList) ->ramWord.string = line[index];  /*note*/
		(*tailDcList) ->ramWord.wordType =3;
		index++;
	}
    index++;
	index = jumpSpace(line,index);  /*Cleans spaces*/
	if(line[index]!= '\n')
	{
		printf("in the row %d in the index %d extra text after the end of the string\n", row, index);
		return -2;
	}
	addToListDC(headDcList, tailDcList);  /*Opens a cell in the data table*/
	(*tailDcList) ->ramWord.string = '\0';
	(*tailDcList) ->ramWord.wordType =3;
	return 0;
}



void addToListDC(ptDCList *headDcList,ptDCList *tailDcList)
{
	ptDCList new;
	new=(ptDCList) malloc(sizeof(listDC)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}
	new -> next =NULL;
	if(!(*headDcList))
	{
		*headDcList = new;
		(*tailDcList) = *headDcList;	
	}
	else
	{
	(*tailDcList)->next = new;
	(*tailDcList) = new;
	}
	(*tailDcList)-> decimalAddress = DC;
	DC++;
	return;
}



void freeListDC(ptDCList *headDcList)			/*Memory release*/
{
	ptDCList p;
	while(*headDcList)
	{
		p = *headDcList;
		*headDcList = p->next;
		free(p);
	}
}

void addSymbolIc(ptSymbol *headSymbol)
{
    ptSymbol p;
    p = *headSymbol;
    while(p)
    {
        if(p-> type == 1)
            p -> location += IC +MEMORY_ACCESS;
        p = p->next;
    }
}

void addIc(ptDCList *headDcList)
{
	ptDCList p;
	p = (*headDcList);
	while(p)
	{
		p -> decimalAddress += IC +MEMORY_ACCESS;
		p = p->next;
	}
}

int directiveStruct(char *line ,int index , int row, ptDCList *headDcList,ptDCList *tailDcList )  /*.data sentences*/
{
	int i=0, num ,start, end;
	char number[SIZE_OF_LINE];
	index = jumpSpace(line,index);
	
		while(line[index] != ','&& !isspace(line[index]))  /*As long as it is not equal to a space or a comma*/
		{
			number[i] = line[index];  /*Copy the first word*/
			i++;
			index++; 
		}
		number[i] = '\0';
        if(number[0] == '\0')
        {
            printf("in line %d in the index %d Missing number after .struct\n", row, index);
            return -2;
        }
		index = jumpSpace(line,index);
		if(correctDigit(number))
		{
			num=atoi(number);
			addToListDC(headDcList, tailDcList);
			(*tailDcList) ->ramWord.string = num;
			(*tailDcList) ->ramWord.wordType =3;
		}
		else 
		{
			printf("in the row %d in the index %d The number is incorrect\n", row, index);
			return -2;
		}
		if(line[index] == ',')
			index++;
		else
		{
			printf("in the row %d in the index %d missing comma in command\n", row, index);
			return -2;
		}
		index = jumpSpace(line,index);
		start = strchr(line, '"') - line;  /*The first show there is " */
		end = strrchr(line, '"')-line;  /*The last show there is " */
		if(line[index] != '"' || start == end)
		{
			printf("in the row %d in the index %d The syntax of the string is incorrect\n", row, index);
			return -2;
		}
		index++;
		while(index < end)   /*as long as they are inside the string*/
		{
			if(!isprint(line[index]))
			{
				printf("in line %d in the index %d There is a character that cannot be printed\n", row, index);
				return -2;
			}
			addToListDC(headDcList,tailDcList);
			(*tailDcList) ->ramWord.string = line[index];
			(*tailDcList) ->ramWord.wordType =3;
			index++;
		}
        index++;
		index = jumpSpace(line,index);
		if(line[index]!= '\n')
		{
			printf("in line %d in the index %d extra text after the end of the string\n", row, index);
			return -2;
		}
		addToListDC(headDcList, tailDcList);
		(*tailDcList) ->ramWord.string = '\0';
		(*tailDcList) ->ramWord.wordType =3;
		return 0;	
		
}
	

int directiveExtern(char *line ,int index ,int row, ptSymbol *headSymbol  )   /*.extern sentences*/
{
	char label[SIZE_LABEL_NAME];
	int i=0, j=0;
	index = jumpSpace(line,index);
	if(!isalpha(line[index]))
	{
		printf("error in line %d index  %d:  Invalid label\n",row, index);
		return -2;
	}
	while(!isspace(line[index]))
	{
		label[i] = line[index];
		if(!isalpha(label[i]) && !isdigit(label[i]))
		{
				printf("error in row %d index  %d: %s - non-number and non-letter character was found\n",row, index, label);
				return -2;
		}
		i++;
		index++;
	}
	label[i] = '\0';
	if(isRegister(label) != -1)
	{
		printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, label);
		return -2;
	}
	while(j<5)		{  /*directives word*/
		if(!strcmp(label ,directives[j]))
		{
			printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, label);
			return -2;
		}
		j++;
	}
	if(isOrder(label) != -1)
	{
		printf("error in row %d index  %d: %s - An attempt to define a label in an assembly language reserved word\n",row, index, label);
		return -2;
	}
	if(sameSymbol(label, *headSymbol))
	{
		printf("error in row %d index  %d: %s - Restatement of label in use detected\n",row, index, label);
		return -2;
	}
	index = jumpSpace(line,index);
	if(line[index] != '\n')
	{
		printf("error in row %d index  %d: %s - extra text after the command\n",row, index, label);
		return -2;
	}
	addToSymbolList(label, headSymbol);
	(*headSymbol) -> location = 0;
	(*headSymbol) -> typeE = E;
	return 0;
}

ptSymbol findSymbol(char * label_name, ptSymbol headSymbol)
{
    ptSymbol p = headSymbol;
    while(p != NULL) 					/*As long as p1 is not equal to null*/
    {
        if(!strcmp(label_name ,p -> label_name))
        {
            return p;
        }
        p = p-> next;
    }
    return NULL;
}

int isEntry(char * line , int index , int row, ptSymbol headSymbol)
{
    ptSymbol p;
    char label[SIZE_LABEL_NAME];
    int i = 0;
    index = jumpSpace(line,index);
    while(!isspace(line[index]))
    {
        label[i] = line[index];
        i++;
        index++;
    }
    label[i] = '\0';
    p= findSymbol(label, headSymbol);
    if(!p)
    {
        printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
        return -2;
    }
    index = jumpSpace(line,index);
    if(line[index] != '\n')
    {
        printf("error in line %d  index  %d: %s - extra text after the label \n", row, index, label);
        return -2;
    }
    p->typeE = entry;
    return 0;
}

int labelGroup1( char * line,int index,int row,ptSymbol headSymbol)     /*In the first pass a sentence with a label*/
{
    char rs[SIZE_STRUCT] , rt[SIZE_STRUCT];
    int i = 0, result = 0, addressS, addressT;
    if(ptr)
        ptr = ptr -> next;
    index = jumpSpace(line,index);
    while (line[index] != ',' && !isspace(line[index]))
    {
        rs[i] = line[index];
        i++;
        index++;
    }
    rs[i] = '\0';
    index = jumpSpace(line,index);
        i = 0;
        index++;
        index = jumpSpace(line,index);
        while(!isspace(line[index]) && i< SIZE_STRUCT)
        {
            rt[i] = line[index];
            i++;
            index++;
        }
        rt[i] = '\0';
        index = jumpSpace(line,index);

    addressS = address(rs); /* which address is it*/
    addressT = address(rt); /* which address is it*/
    result = memoryCoding2( addressS,  row , index, rs, headSymbol);
    if(result)
        return -2;
    if(addressS != 3 || addressT != 3 ) {
        result = memoryCoding2(addressT, row, index, rt, headSymbol);
        if (result)
            return -2;
    }
    return 0;
}


int memoryCoding2(int address, int row ,int index, char * label,ptSymbol headSymbol)
{
    ptSymbol p;
    char * pt;
    if(address == 1) /* label*/
    {
        p = findSymbol(label, headSymbol);
        if(!p)
        {
            printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
            return -2;
        }

        ptr -> ramWord.number = p -> location;
        if(p ->typeE != E)
            ptr -> ramWord.are=R;
        else {
            flagEx= 1;
            ptr->ramWord.are = E;
            addToExternalList(p,ptr ->decimalAddress);
        }
    }
    else if(address == 2) /* struct*/
    {
        pt = strrchr(label , '.');
        *pt ='\0';
        p = findSymbol(label, headSymbol);
        if(!p)
        {
            printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
            return -2;
        }
        ptr -> ramWord.number = p -> location;
        if(p ->typeE != E)
            ptr -> ramWord.are=R;
        else {
            addToExternalList( p , ptr->decimalAddress);
            ptr->ramWord.are = E;
        }
            ptr = ptr->next;
    }
    if(ptr)
        ptr = ptr ->next;
    return 0;
}


int labelGroup2( char * line,int index,int row, ptSymbol headSymbol)
{
    char rt[SIZE_STRUCT];
    int i = 0, addressT, result ;
    if(ptr)
        ptr = ptr -> next;
    index = jumpSpace(line,index);
    while(!isspace(line[index]))
    {
        rt[i] = line[index];
        i++;
        index++;
    }
    rt[i] = '\0';
    index = jumpSpace(line,index);
    addressT = address(rt); /* which address is it*/
    result = memoryCoding2( addressT , row, index, rt,headSymbol );
    if( result == -2 )
        return -2;
    return 0;
}


















	












