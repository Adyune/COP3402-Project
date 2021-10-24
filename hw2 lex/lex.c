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

#define MAX_NUM_RESERVED 14
#define MAX_NUM_OP 17
lexeme *list;
int lex_index;
int i = 0, j = 0, lex_index = 0;
char string[MAX_IDENT_LEN] = {0};
lexeme token;
char *reservedWords[MAX_NUM_RESERVED] = {"const", "var","procedure", "begin", "end", "while", 
						"do", "if", "then", "else", "call", "write", "read", "odd"};
char *operators[MAX_NUM_OP + 1] = {":=", "+", "-", "*", "/", "%", "==", "!=", "<"
								, "<=", ">", ">=", "(", ")", ",", ".", ";", "//"};
token_type reservedTokens[MAX_NUM_RESERVED] = {constsym, varsym, procsym, beginsym, endsym, whilesym, dosym, 
								ifsym, thensym, elsesym, callsym, writesym, readsym, oddsym};
token_type operatorSym[MAX_NUM_OP] = {assignsym, addsym, subsym, multsym, divsym, modsym, 
									eqlsym, neqsym, lsssym, leqsym, gtrsym, geqsym, 
									lparensym, rparensym, commasym, periodsym, semicolonsym};
void printlexerror(int type);
void printtokens();


lexeme *lexanalyzer(char *input)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	while(input[i] != '\0'){
		if (iscntrl(input[i]) || input[i] == ' '){
			i++;
			continue;
		}
		if (isdigit(input[i])){
			while(input[i] != '\0'){
				string[j] = input[i]; // Copy the character and increment counters
				i++, j++;
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
				else if(!isdigit(input[i])){
					token.value = atoi(string);
					token.type = numbersym;
					list[lex_index] = token;
					lex_index++;
					j = 0;
					// Reset string to be empty for the next token
					memset(string,0,sizeof(string));
					break;
				}
			}
		} 
		// Reserved Words + identifiers 
		else if (isalpha(input[i])){
			while(input[i] != '\0'){
				string[j] = input[i]; // Copy the character and increment counters
				i++, j++;
				if(j > MAX_IDENT_LEN){
					printlexerror(4);
					exit(1);
				}
				else if (!isalpha(input[i]) && !isdigit(input[i])){
					int isReserved = 0;
					for(int k = 0; k < MAX_NUM_RESERVED; k++){
						if (strcmp(string, reservedWords[k]) == 0){
							strcpy(token.name, string);
							token.type = reservedTokens[k];
							list[lex_index] = token;
							lex_index++;
							j = 0;
							// Reset string to be empty for the next token
							memset(string,0,sizeof(string));
							isReserved = 1;
							break;
						}
					}
					if (isReserved)
						break;
					else {
						strcpy(token.name, string);
						token.type = identsym;
						list[lex_index] = token;
						lex_index++;
						j = 0;
						// Reset string to be empty for the next token
						memset(string,0,sizeof(string));
						break;
					}
				}
			}
		}
		// Special symbols
		else{
			int isOp = 0;
			// At most special symbols are two characters long
			for (int tracker = 0;tracker < 2; tracker++){
				string[j] = input[i];
				i++, j++;printf("%d %s\n", tracker, string);
				for (int k = 0; k < MAX_NUM_OP;k++){
					if(strcmp(string, operators[k]) == 0){
						if (!iscntrl(input[i]) && (string[j - 1] == '<' || string[j - 1] == '>') && input[i] == '='){
							strcpy(token.name, string);
							strcat(token.name, "=");
							token.type = operatorSym[k + 1];
							list[lex_index] = token;
							lex_index++;
							j = 0, i++;
							// Reset string to be empty for the next token
							memset(string,0,sizeof(string));
							isOp = 1;
							break;
						}
						else if (string[j - 1] == '/' && input[i] == '/'){
							while (input[i] != '\n')
								i++;
							isOp = 1;
						}
						// Error if there is anything between the characters
						else if (((string[j - 1] == ':' || string[j - 1] == '!') && input[i] != '=') && tracker == 1 && !strcmp(string, "==") && isOp == 0){
							printlexerror(1);
							exit(1);
						}
						else{
							// Otherwise add to token list
							strcpy(token.name, string);
							token.type = operatorSym[k];
							list[lex_index] = token;
							lex_index++;
							j = 0;
							// Reset string to be empty for the next token
							memset(string,0,sizeof(string));
							isOp = 1;
							break;
						}
					}
					if(isOp)
						break;
				}
				if(isOp == 0 && tracker == 1){
					printf("heh");
					printlexerror(1);
					exit(1);
				}
			}
		}
	} 
	printtokens();
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
