/* 
	This is the lex.c file for the UCF Fall 2021 Systems Software Project.
	For HW2, you must implement the function lexeme *lexanalyzer(char *input).
	You may add as many constants, global variables, and support functions
	as you desire.
	
	If you choose to alter the printing functions or delete list or lex_index, 
	you MUST make a note of that in you readme file, otherwise you will lose 
	5 points.
*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

int lexeme *list; // (double check this but I think here were were missing calling this an integer)
int lex_index;

void printlexerror(int type);
void printtokens();


lexeme *lexanalyzer(char *input)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	int i = 0, j = 0, lex_index = 0;
	char * string = malloc(sizeof(char) * MAX_IDENT_LEN + 1);
	lexeme token;
	// TODO: implement var, restricted, and operators/symbols
	while(input[i] != '\0'){
		if (iscntrl(input[i]) || input[i] == " "){
			i++;
			continue;
		}
		// If the character starts with a number 
		if(isdigit(input[i])){
			while(input[i] != '\0'){
				// Invalid Identifier
				if (isalpha(input[i])){
					printlexerror(2);
					exit(1);
				}
				// Excessive number length
				else if (j > MAX_NUMBER_LEN){
					printlexerror(3);
					exit(1);
				}
				// Implement helper function so this doesnt look gross?
				else if (iscntrl(input[i]) || input[i] == " " || !isdigit(input[i]) || input[i] != '\0')
				{
					token.value = atoi(string);
					token.type = numbersym;
					list[lex_index] = token;
					lex_index++;
					if (iscntrl(input[i]) || input[i] == " "){
						i++;
					}
					j = 0;
					break;
				} else {
					string[j] = input[i];
					i++, j++;
				}
			}
			continue;
		}
		// Case where the input starts with a letter and determines if it is a variable, procedure or a reserved word
		else if (isalpha(input[i])){
			while(input[i] != '\0'){
				if(j > MAX_IDENT_LEN){
					printlexerror(4);
					exit(1);
				}
				else if (!isalpha(input[i]) || !isdigit(input[i]) || iscntrl(input[i]) || input[i] == " "){
					// Check if the string is a reserved word
					if (strcmp(string, "const") == 0){

					}
					else if (strcmp(string, "var") == 0){

					}
					else if (strcmp(string, "procedure") == 0){

					}
					else if (strcmp(string, "call") == 0){

					}
					else if (strcmp(string, "if") == 0){

					}
					else if (strcmp(string, "then") == 0){

					}
					else if (strcmp(string, "else") == 0){

					}
					else if (strcmp(string, "while") == 0){

					}
					else if (strcmp(string, "do") == 0){

					}
					else if (strcmp(string, "begin") == 0){

					}
					else if (strcmp(string, "end") == 0){

					}
					else if (strcmp(string, "read") == 0){

					}
					else if (strcmp(string, "write") == 0){

					}
					else if (strcmp(string, "odd") == 0){

					}
					// If it's not a reserved word then it is a identifer 
					else {
						strcpy(token.name, string);
						token.type = identsym;
						list[lex_index] = token;
						lex_index++;
						if (iscntrl(input[i]) || input[i] == " "){
							i++;
						}
						j = 0;
						break;
					}
				}
				else{
					string[j] = input[i];
					i++, j++;
				}
			}
			continue;
		}
		// Checks for operators
		else {
			while (input[i] != "\0") {
				//check for all the operators except for =, <, >, !, and :
				if (input[i] == '*' || input[i] == '%' || input[i] == '/' || input[i] == '+' || 
					input[i] == '-' || input[i] == '(' || input[i] == ')' || input[i] == ',' || input[i] == '.' ||
					input[i] == ';') {
						string[j] = input[i];
						i++, j++;
				} 
				//dedicated if block for < and >, since they might be part of <= or >=
				else if (input[i] == '<' || input[i] == '>') {
					//check to see if the following character is '='
					if (input[i + 1] == '=') {
						string[j++] = input[i++]; //set string[j] to '<' or '>'. then increment j and i
						string[j++] = input[i++]; // j and i are incremented. set string[j] to '='. incremement j and i
					}
					//the following character is not an '='
					else {
						string[j] = input[i];
						i++, j++;
					}
				}
				// '!' and ':' by themselves are not valid operators. they must always be followed by '='
				// this block checks to see if that is the case
				else if (input[i] == '!' || input[i] == ":") {
					//check to see if the following character is '='
					if (input[i + 1] == '=') {
						string[j++] = input[i++]; //set string[j] to '!' or ':'. then increment j and i
						string[j++] = input[i++]; // j and i are incremented. set string[j] to '='. incremement j and i
					}
					//the following character is not an '='
					else {
						//invalid symbol
						printlexerror(1);
						exit(1);
					}
				}
				//invalid symbol
				else {
					printlexerror(1);
					exit(1);
				}

			}
			
		}
	}
	free(string);
	printtokens();
	return list;
}


void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}
