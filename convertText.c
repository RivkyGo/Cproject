#include "convertText.h"


extern ptExtern headExternList;
extern int IC , DC;
char numInBase[3];

void ConvertToBase(int num)	/*Convert a number to base 32*/
{
	char base[]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
    int firstNum, secondNum ;
    secondNum = num%SIZE_BASE;
    num = num/SIZE_BASE ;
    firstNum = num%SIZE_BASE;
    numInBase[0] = base[firstNum];
    numInBase[1] = base[secondNum];
    numInBase[2] = '\0';
}

void convertToObject(char * file_name,ptDCList  headDcList, ptICList headIcList)		/*Creating an object file*/
{
	ptICList ptIc = headIcList;
	ptDCList ptDc = headDcList;
	FILE * file;
	int number , mask = 1, temp;
	char * fullFileName;
	fullFileName = concat(file_name, ".ob");	/*Creating a suffix .ob*/
	file = fopen(fullFileName, "w");
	if(!file)
	{
		printf("can not open %s file",fullFileName);
		return;
	}
	ConvertToBase(IC);				/*IC and DC printing in base 32*/
	fputs(numInBase, file);
	fputs(" ", file);
	ConvertToBase(DC );
	fputs(numInBase, file);
	fputs("\n", file);
	while(ptIc)					/*Printing the data structure in base 32*/
	{
		ConvertToBase( ptIc ->decimalAddress);
		fputs(numInBase, file);
		fputc('\t', file);
		if(ptIc ->ramWord.wordType ==0)		/*Handling a type word(opcod, source operand, target operand, ARE)*/
		{
			number= (int)ptIc ->ramWord.opcode;
			mask=1;
			number = number<<2;
			temp=(int)ptIc ->ramWord.addressS;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			mask=1;
			number = number<<2;
			temp=(int)ptIc ->ramWord.addressT;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			mask=1;
			number = number<<2;
			temp=(int)ptIc ->ramWord.are;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			mask=1; 
		}
		else if(ptIc ->ramWord .wordType ==1)  /*Handling a type word(8 bits, ARE)*/
		{
			number= (int)ptIc ->ramWord.number;
			mask=1;
			number = number<<2;
			temp=(int)ptIc ->ramWord.are;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);  
		}

		else if(ptIc ->ramWord .wordType ==2)/*Handling a type word(4 bits, 4 bits, ARE)*/
		{
			number= (int)ptIc ->ramWord.rs;
			number = number<<4;
			mask = 1;
			temp=(int)ptIc ->ramWord.rt;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
			number = number<<2;
			mask = 1;
			temp=(int)ptIc ->ramWord.are;
			number= number|(mask&temp);
			mask=mask<<1;
			number= number|(mask&temp);
		}   
		ConvertToBase(number);
		fputs(numInBase, file);
		fputc('\n', file);		/*Writing the encoding translation to the file*/
		ptIc = ptIc -> next;
	}
	while(ptDc)
	{
		ConvertToBase( ptDc ->decimalAddress);
		fputs(numInBase, file);
		fputc('\t', file);
		number= (int)ptDc ->ramWord.string;      /*Handling a type word(10 bits)*/
		ConvertToBase(number);
		fputs(numInBase, file);
		fputc('\n', file);
		ptDc = ptDc -> next;
	}
	fclose(file);
	free(fullFileName);
}







char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    if(result)
    {
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
    else
    {
        return NULL;
    }
}


void convertToEntry(char * file_name,ptSymbol headSymbol)
{
    ptSymbol p = headSymbol;
    FILE * file;
    char * fullFileName;
    fullFileName = concat(file_name, ".ent");
    file = fopen(fullFileName, "w");
    if(!file)
    {
        printf("can not open %s file",fullFileName);
        return;
    }
    while(p)
    {
        if(p ->typeE ==  3)						/*if the type of symbol is entry*/
        {
            fputs(p ->label_name, file);
            ConvertToBase( p ->location);	/*Converting the memory location to base 32*/
            fputc('\t', file);
            fputs(numInBase, file);
            fputc('\n', file);
        }
        if(p)
            p = p->next;
    }
    fclose(file);
    free(fullFileName);
}

void convertToExtern(char * file_name)
{
    ptExtern p = headExternList;
    FILE * file;
    char * fullFileName;
    fullFileName = concat(file_name, ".ext");
    file = fopen(fullFileName, "w");
    if(!file)
    {
        printf("can not open %s file",fullFileName);
        return;
    }
    while(p)
    {
        fputs(p ->label_name, file);
        ConvertToBase( p ->location);	/*Converting the memory location to base 32*/
        fputc('\t', file);
        fputs(numInBase, file);
        fputc('\n', file);
        if(p)
            p = p->next;
    }
    fclose(file);
    free(fullFileName);
}
