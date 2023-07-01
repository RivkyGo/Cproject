#ifndef CHECKTEXT_H
#define CHECKTEXT_H
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "Input.h"

#define MEMORY_ACCESS 100
#define SIZE_LABEL_NAME 31
#define SIZE_STRUCT 33

typedef struct symbolNode * ptSymbol;
typedef struct symbolNode{
		char label_name[SIZE_LABEL_NAME];
		int location;
		int type;
		int typeE;
		ptSymbol next;
		}symbol;

typedef struct externalNode * ptExtern;
typedef struct externalNode{
    char label_name[SIZE_LABEL_NAME];
    int location;
    ptExtern next;
}externList;

typedef struct word{
			unsigned int are: 2;
			unsigned int rt: 4;
			unsigned int rs: 4;
			unsigned int opcode: 4;
			unsigned int number: 8;
			unsigned int string: 10;
			unsigned int addressS: 2;
			unsigned int addressT: 2;
			unsigned int wordType: 2;
		}coding;

typedef struct ICList * ptICList;

typedef struct ICList{
			coding ramWord;
			int decimalAddress;
			ptICList next;
			}listIC;	

typedef struct DCList * ptDCList;

typedef struct DCList{
			coding ramWord;
			int decimalAddress;
			ptDCList next;
			}listDC;	
		
int isDirective (char * ,int ,int );				 /* Input: an array containing the first word in the line, line number, index of the first word
						                Output: if this is a prompt statement - returns the index of the prompt
						                or the directive statement is false returns -2
					                   	or it is not a directive statement returns -1*/

int isPureDirective(char *);  					/*Input: an array with a word
                               					 Output: if it is a prompt returns the index of the prompt otherwise -1*/

int isOrder (char *);  			/*Input: an array containing the first word in the line, line number, index of the first word
			             Output: if it is an instruction sentence - returns the index of the instruction
                         		or it is not an instruction statement returns -1*/

int islabel(char *, int , int, ptSymbol* ); /* Input: array with word, line number, letter number
                                    		Output: if it is a label returns a positive number otherwise returns -1*/

void addToSymbolList(char *, ptSymbol *);  /*Input: an array with the name of a label
						a pointer to pointer to the head of the symbol table
                                		 Output: No output*/

void addToExternalList(ptSymbol,int );  /* Input: a pointer to a list, an address in memory
                                           Output: None*/

int sameSymbol(char *, ptSymbol );  /*Input: an array with the name of a label
					pointer to the head of the symbol table
                            		Output: Returns 0 if there was no identical label otherwise -2*/

void freeSymbolList( ptSymbol *);  /*Free memory- the Symbol list
					Input: pointer to pointer the head of the symbol table*/

void freeExternalList();  /*Freeing memory- the external list*/

int isRegister(char *);/*Input: a string, and checks if the word the string contains is the name of a register
			             Output: returns the register number if it exists.
		              	 otherwise returns -1*/
int correctDigit( char * );/*Input: String
				             Output: 0 if the string is not an integer
				             otherwise 1.*/

void addToListIC(ptICList *, ptICList *);  /*Add to instruction table
						Input: pointer to pointer to the head of instruction table
						pointer to pointer to the tail of instruction table*/

void freeListIC(ptICList *);  /*Free instruction tablememory
				Input: pointer to pointer to the head of instruction table*/

int address(char *);   /*Input: an array with a word
                         Output: returns -2 if syntax of word is wrong else if register returns 3 if struct returns 2 if label returns 1
                         if immediate address returns 0*/

int instrucGroup1(int , char *, int ,int,ptICList*, ptICList*);  /*Input: instruction command number, array with input line, line number, word number
								pointer to pointer to the head of instruction table
								pointer to pointer to the tail of instruction table
                                              			Output: returns -2 if the syntax of a word is incorrect else 0 if everything is correct*/

void memoryCoding(int , int , char *, char *, ptICList*, ptICList*);  /*Input: the source address, the destination address, an array with the word of the first 									operand, an array with the word of the second operand
									pointer to pointer to the head of instruction table
									pointer to pointer to the tail of instruction table
														  Output: None*/

int instrucGroup2(int , char *, int ,int, ptICList* ,  ptICList*);  /*Input: instruction command number, array with input line, line number, word number
									pointer to pointer to the head of instruction table
									pointer to pointer to the tail of instruction table
                                              				Output: returns -2 if the syntax of a word is incorrect else 1 if everything is correct*/

int instrucGroup3(int , char *, int ,int, ptICList* ,  ptICList*);  /*Input: instruction command number, array with input line, line number, word number
                                              				Output: returns -2 if the syntax of a word is incorrect else 1 if everything is correct*/

int directiveData(char *, int , int, ptDCList *, ptDCList *);  /*Input: an array with the input line, line number, word number
								pointer to pointer to the head of data table
								pointer to pointer to the tail of data table
                                         			Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

int directiveString(char *, int , int,ptDCList *,ptDCList * );  /*Input: an array with the input line, line number, word number
								pointer to pointer to the head of data table
								pointer to pointer to the tail of data table
                                        			 Output: returns -2 if there is an error in the sentence syntax otherwise 0*/
void addToListDC(ptDCList *,ptDCList *);  /*Add to list
						Input: pointer to pointer to the head of data table
						pointer to pointer to the tail of data table*/

void freeListDC(ptDCList *);  /*Free memory
				input: pointer to pointer to the head of data table*/

void addIc(ptDCList *);     /*Add to list
				Input:pointer to pointer to the head of data table*/

int directiveStruct(char *, int , int, ptDCList *,ptDCList * );  /*Input: an array with the input line, line number, word number
								pointer to pointer to the head of data table
								pointer to pointer to the tail of data table
                                         			Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

int directiveExtern(char * ,int ,int, ptSymbol * );  /*Input: an array with the input line, line number, word number
							pointer to pointer to the head of symbol table
                                         		Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

void addSymbolIc(ptSymbol*);  /*Add to list
				Input: pointer to pointer to the head of symbol table*/

int isEntry(char *  , int , int, ptSymbol);   /*Input: an array with the input line, line number, word number
						pointer to the head of symbol table
                                         Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

int memoryCoding2(int , int ,int , char *, ptSymbol);  /*Input: the address number, the row number, the index number, an array with a label
                                                Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

int labelGroup1( char *, int, int, ptSymbol );  /*Input: an array with the input line, line number, word number
						pointer to the head of symbol table
                                         Output: returns -2 if there is an error in the sentence syntax otherwise 0*/

int labelGroup2( char *, int, int, ptSymbol );   /*Input: an array with the input line, line number, word number
                                         Output: returns -2 if there is an error in the sentence syntax otherwise 0*/


/*ptSymbol findSymbol(char * , ptSymbol)
					Input: char*-label name, 
					pointer to the head of symbol table
					output:Pointer to the requested label in the symbol table*/




#endif
