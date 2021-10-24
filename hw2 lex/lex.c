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
// temp string to store names
char string[MAX_IDENT_LEN] = {0};
lexeme token;
// Arrays for loops
char *reservedWords[MAX_NUM_RESERVED] = {"const", "var","procedure", "begin", "end", "while", 
						"do", "if", "then", "else", "call", "write", "read", "odd"};
char *operators[MAX_NUM_OP] = {":=", "+", "-", "*", "/", "%", "==", "!=", "<"
								, "<=", ">", ">=", "(", ")", ",", ".", ";"};
token_type reservedTokens[MAX_NUM_RESERVED] = {constsym, varsym, procsym, beginsym, endsym, whilesym, dosym, 
								ifsym, thensym, elsesym, callsym, writesym, readsym, oddsym};
token_type operatorSym[MAX_NUM_OP] = {assignsym, addsym, subsym, multsym, divsym, modsym, 
									eqlsym, neqsym, lsssym, leqsym, gtrsym, geqsym, 
									lparensym, rparensym, commasym, periodsym, semicolonsym};
void printlexerror(int type);
void printtokens();


lexeme *lexanalyzer(char *input)
{
	// Allocate memory
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	while(input[i] != '\0'){
		// Skip whitespace
		if (iscntrl(input[i]) || input[i] == ' '){
			i++;
			continue;
		}
		// Check for operators
		if (!isdigit(input[i]) && !isalpha(input[i])){
			// check for comments and skip until next line
			if (input[i] == '/' && input[i + 1] == '/'){
				while(input[i] != 10)
					i++;
			}
			// Check for the edgecases (:=, ==, !=) where it can error
			else if (input[i] == ':' || input[i] == '=' ||input[i] == '!'){
				if (input[i] == ':' && input[i + 1] == '='){
					strcpy(token.name, ":=");
					token.type = assignsym;
					list[lex_index] = token;
					lex_index++;
					i += 2;
				}
				else if (input[i] == '=' && input[i + 1] == '='){
					strcpy(token.name, "==");
					token.type = eqlsym;
					list[lex_index] = token;
					lex_index++;
					i += 2;
				}
				else if (input[i] == '!' && input[i + 1] == '='){
					strcpy(token.name, "!=");
					token.type = neqsym;
					list[lex_index] = token;
					lex_index++;
					i += 2;
				}
				else {
					// If the operator is noot found, error and exit
					printlexerror(1);
					exit(1);
				}
			}
			// Addition
			else if (input[i] == '+'){
				strcpy(token.name, "+");
				token.type = addsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Subtract
			else if (input[i] == '-'){
				strcpy(token.name, "-");
				token.type = subsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Mult
			else if (input[i] == '*'){
				strcpy(token.name, "*");
				token.type = multsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Div
			else if (input[i] == '/'){
				strcpy(token.name, "/");
				token.type = divsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Mod
			else if (input[i] == '%'){
				strcpy(token.name, "%");
				token.type = modsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Less than, if the next token is '=' then less than or equal to
			else if (input[i] == '<'){
				if (input[i + 1] == '='){
					strcpy(token.name, "<=");
					token.type = leqsym;
					list[lex_index] = token;
					lex_index++;
					i += 2;
				}
				else {
					strcpy(token.name, "<");
					token.type = lsssym;
					list[lex_index] = token;
					lex_index++;
					i++;
				}
			}
			// Greater than, if the next token is '=' then greater than or equal to
			else if (input[i] == '>'){
				if (input[i + 1] == '='){
					strcpy(token.name, ">=");
					token.type = geqsym;
					list[lex_index] = token;
					lex_index++;
					i += 2;
				}
				else {
					strcpy(token.name, ">");
					token.type = gtrsym;
					list[lex_index] = token;
					lex_index++;
					i++;
				}
			}
			// R Parenthesis
			else if (input[i] == '('){
				strcpy(token.name, "(");
				token.type = lparensym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// L Parenthesis
			else if (input[i] == ')'){
				strcpy(token.name, ")");
				token.type = rparensym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Comma
			else if (input[i] == ','){
				strcpy(token.name, ",");
				token.type = commasym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Period
			else if (input[i] == '.'){
				strcpy(token.name, ".");
				token.type = periodsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Semicolon
			else if (input[i] == ';'){
				strcpy(token.name, ";");
				token.type = semicolonsym;
				list[lex_index] = token;
				lex_index++;
				i++;
			}
			// Error if the symbol is not recognized 
			else {
				printlexerror(1);
				exit(1);
			}
		}
		else if (isdigit(input[i])){
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
					// Loop through list of reserved words and 
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
