#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int level;
// Tracks tokens
int tokenCtr;
// Signals if the parser encounters and error
int errorFlag;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

// These are the functions we need to implement
void program(lexeme *list);
void block(lexeme *list);
void constDeclaration(lexeme *list);
int varDeclaration(lexeme *list);
void procedureDeclaration(lexeme *list);
void statement(lexeme *list);
void condition(lexeme *list);
void expression(lexeme *list);
void term(lexeme *list);
void factor(lexeme *list);

int multipleDeclarationCheck(lexeme token);
int findSymbol(lexeme token, int type);
void mark();

instruction *parse(lexeme *list, int printTable, int printCode)
{
	
	// Set the token counter to 0 for the first token
	tokenCtr = 0;
	// Set the errorFlag to 0
	errorFlag = 0;
	// allocate memory 
	code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
	table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
	program(list);
	/* this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS*/
	if (errorFlag == 0){
		code[cIndex].opcode = -1;	
		// Print the tables if the flags are == 1
		if (printTable)
			printsymboltable();
		if (printCode)
			printassemblycode();
		return code;
	}
	else 
		return NULL;
}

void program(lexeme *list){
	// printf("Program has been called\n");
	// Emit JMP to main
	emit(7, 0, 3);
	// Add main function to the table, address to be determined later
	addToSymbolTable(3, "main", 0, 0, 0, 0);
	level = -1;
	block(list);
	if (errorFlag == 1){
		return;
	}
	// Error if last token is not a period
	if (list[tokenCtr].type != periodsym){
		printparseerror(1);
		errorFlag = 1;
		return;
	}	
	// Emit Halt
	emit(9, 0, 3);
	// Get the proper address of the call functions
	for (int i = 0; i < cIndex; i++){
		if (code[i].opcode == 5){
			code[i].m = table[code[i].m].addr;
		}
	}
	code[0].m = table[0].addr;
}

void block(lexeme *list){
	if (errorFlag == 1){
		return;
	}
	// increment level
	level++;
	// printf("%d\n", level);
	int procedure_idx = tIndex - 1;
	// check for const declarations 
	constDeclaration(list);
	if (errorFlag == 1){
		return;
	}
	// check variables
	int x = varDeclaration(list);
	if (errorFlag == 1){
		return;
	}
	// check for procedure declarations
	procedureDeclaration(list);
	if (errorFlag == 1){
		return;
	}
	table[procedure_idx].addr = cIndex * 3;
	if (level == 0)
		// emit INC
		emit(6, 0, x);
	else	
		emit(6, 0, x + 3);
	statement(list);
	if (errorFlag == 1){
		return;
	}
	// Mark all var, const, and procedures once block is done
	mark();
	// deincrement level once the block is done
	level--;
}

void constDeclaration(lexeme *list){
	// printf("const has been called\n");
	// Check if the current token is a constant
	if (list[tokenCtr].type == constsym){
		// Do while will check for muliple const declarations
		do {
			tokenCtr++;
			// printf("%d\n", tokenCtr);
			// If the token is not an identifier then error
			if (list[tokenCtr].type != identsym){
				printparseerror(2);
				errorFlag = 1;
				return;
			}
			// If multipleDeclarationCheck returns a value other than -1 then error
			// Meaning a identifier with the same name, mark and level are the same 
			int symidx = multipleDeclarationCheck(list[tokenCtr]);
			if (symidx != -1){
				printparseerror(18);
				errorFlag = 1;
				return;
			}
			// temp string to store the name of the identifer to add to the sym table later
			char name[12];
			strcpy(name, list[tokenCtr].name);
			tokenCtr++;
			// Error of token is not an assignment
			if (list[tokenCtr].type != assignsym){
				printparseerror(2);
				errorFlag = 1;
				return;
			}
			tokenCtr++;
			// Error if the symbol is not a number
			if (list[tokenCtr].type != numbersym){
				printparseerror(2);
				errorFlag = 1;
				return;
			}
			// Add to symbol table and get the next token
			addToSymbolTable(1, name, list[tokenCtr].value, level, 0, 0);
			tokenCtr++;
			// If the token if a comma continue looping
		} while (list[tokenCtr].type == commasym);
		// Error if the token is not a semicolon
		if (list[tokenCtr].type != semicolonsym){
			// Print specific error if the token was a identifer
			if (list[tokenCtr].type == identsym){
				printparseerror(13);
				errorFlag = 1;
				return;
			} else {
				printparseerror(14);
				errorFlag = 1;
				return;
			}
		}
		tokenCtr++;
	}
}

int varDeclaration(lexeme *list){
	// printf("Var has been called\n");
	int numVars = 0;
	if (list[tokenCtr].type == varsym){
		do {
			numVars++;
			tokenCtr++;
			// Error if token is not an identifer
			if (list[tokenCtr].type != identsym){
				printparseerror(3);
				errorFlag = 1;
				return -1;
			}
			// Check for multiple declarations, error if there are any
			int symidx = multipleDeclarationCheck(list[tokenCtr]);
			if (symidx != -1){
				printparseerror(18);
				errorFlag = 1;
				return -1;
			}
			// Add to the symbol table depending on the level
			if (level == 0)
				addToSymbolTable(2, list[tokenCtr].name, 0, level, numVars - 1, 0);
			else
				addToSymbolTable(2, list[tokenCtr].name, 0, level, numVars + 2, 0);
			tokenCtr++;
			// Loop until reach a token thats not a comma
		} while (list[tokenCtr].type == commasym);
		if (list[tokenCtr].type != semicolonsym){
			// Error for identifer since that requires a comma
			if (list[tokenCtr].type == identsym){
				printparseerror(13);
				errorFlag = 1;
				return -1;
			// Error for lack of a semicolon
			} else {
				printparseerror(14);
				errorFlag = 1;
				return -1;
			}
		}
		tokenCtr++;
	}
	// return the number of variables
	return numVars;
}

void procedureDeclaration(lexeme *list){
	// printf("preocedure has been called\n");
	while (list[tokenCtr].type == procsym){
		tokenCtr++;
		// Error if there is no identifer
		if (list[tokenCtr].type != identsym){
			printparseerror(4);
			errorFlag = 1;
			return;
		}
		// Check if the procedure has already been declared, error if it is
		int symidx = multipleDeclarationCheck(list[tokenCtr]);
		if (symidx != -1){
			printparseerror(18);
			errorFlag = 1;
			return;
		}
		// add it to table
		addToSymbolTable(3, list[tokenCtr].name, 0, level, 0, 0);
		tokenCtr++;
		// error if there is no semicolon at the end
		if (list[tokenCtr].type != semicolonsym){
			printparseerror(4);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		// Get the block of code within the procedure 
		block(list);
		if (errorFlag == 1){
			return;
		}
		// error if there is no semicolor after 
		if (list[tokenCtr].type != semicolonsym){
			printparseerror(14);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		// Emit return
		emit(2, 0, 0);
	}
}

void statement(lexeme *list){
	if (errorFlag == 1){
		return;
	}
	// printf("statement has been called %d\n ", tokenCtr);
	if (list[tokenCtr].type == identsym){
		int symidx = findSymbol(list[tokenCtr], 2);
		if (symidx == -1){
			// If the token is a constant or a procedure error as variables can only be assigned
			if (findSymbol(list[tokenCtr], 1) != findSymbol(list[tokenCtr], 3)){
				printparseerror(6);
				errorFlag = 1;
				return;
			// Errors as the token is not found
			} else {
				printparseerror(19);
				errorFlag = 1;
				return;
			}
		}
		tokenCtr++;
		// Errors as assignment requires :=
		if (list[tokenCtr].type != assignsym){
			printparseerror(5);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		// get the expression of the assignment
		expression(list);
		if (errorFlag == 1){
			return;
		}
		// STO
		emit(4, level - table[symidx].level, table[symidx].addr);
		return;
	}
	if (list[tokenCtr].type == beginsym){
		do{
			tokenCtr++;
			statement(list);
			if (errorFlag == 1){
				return;
			}
		} while (list[tokenCtr].type == semicolonsym);
		if (list[tokenCtr].type != endsym){
			if (list[tokenCtr].type == identsym || list[tokenCtr].type == beginsym ||
				list[tokenCtr].type == ifsym || list[tokenCtr].type == whilesym ||
				list[tokenCtr].type == readsym || list[tokenCtr].type == writesym ||
				list[tokenCtr].type == callsym)
			{
				// Errors as tokens that use these are typically used in statements and they need semicolons
				printparseerror(15);
				errorFlag = 1;
				return;
			}
			else{
				// Errors if there is no end token
				printparseerror(16);
				errorFlag = 1;
				return;
			}
		}
		tokenCtr++;
		return;
	}
	if (list[tokenCtr].type == ifsym){
		tokenCtr++;
		condition(list);
		if (errorFlag == 1){
			return;
		}
		int jpcIdx = cIndex;
		// JPC
		emit(8, 0, 0);
		// Errors if the next token after the condition is not then
		if (list[tokenCtr].type != thensym){
			printparseerror(8);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		statement(list);
		if (errorFlag == 1){
			return;
		}
		if (list[tokenCtr].type == elsesym){
			int jmpidx = cIndex;
			//JMP
			emit(7, 0, 0);
			// Set address of the jpc
			code[jpcIdx].m = cIndex * 3;
			// Increment to the next token as this will error as the token will still be else when statement is called
			tokenCtr++;
			statement(list);
			if (errorFlag == 1){
				return;
			}
			// Set address of the jmp
			code[jmpidx].m = cIndex * 3;
		}
		else {
			// Set address of the jpc
			code[jpcIdx].m = cIndex * 3;
		}
		return;
	}
	if (list[tokenCtr].type == whilesym){
		tokenCtr++;
		int loopidx = cIndex;
		// get the condition of the loop
		condition(list);
		if (errorFlag == 1){
			return;
		}
		// Error as while must be followed by do 
		if (list[tokenCtr].type != dosym){
			printparseerror(9);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		int jpcidx = cIndex;
		// JPC
		emit(8, 0, 0);
		statement(list);
		if (errorFlag == 1){
			return;
		}
		// JMP
		emit(7, 0, loopidx * 3);
		code[jpcidx].m = cIndex * 3;
		// printf("while 1 code.m = %d\n", cIndex * 3);
		return;
	}
	if (list[tokenCtr].type == readsym){
		tokenCtr++;
		// Errors if the token is not an identifer to be read
		if (list[tokenCtr].type != identsym){
			printparseerror(6);
			errorFlag = 1;
			return;
		}
		int symidx = findSymbol(list[tokenCtr], 2);
		if (symidx == -1){
			// If the symbol is found but is a const or procedure call error 6
			if (findSymbol(list[tokenCtr], 1) != findSymbol(list[tokenCtr], 3)){
				printparseerror(6);
				errorFlag = 1;
				return;
			} else {
				// Other wise error due to undeclared identifier
				printparseerror(19);
				errorFlag = 1;
				return;
			}
		}
		tokenCtr++;
		// READ
		emit(9, 0, 2);
		// STO
		emit(4, level - table[symidx].level, table[symidx].addr);
		return;
	}

	if (list[tokenCtr].type == writesym){
		tokenCtr++;
		expression(list);
		if (errorFlag == 1){
			return;
		}
		// Write
		emit(9, 0, 1);
		return;
	}
	if (list[tokenCtr].type == callsym){
		tokenCtr++;
		int symidx = findSymbol(list[tokenCtr], 3);
		if (symidx == -1){
			// Errors if an identifier with the name of the token is found but its not an identifer
			if (findSymbol(list[tokenCtr], 1) != findSymbol(list[tokenCtr], 2)){
				printparseerror(7);
				errorFlag = 1;
				return;
			} else {
				// Error due to the identifier not being found
				printparseerror(19);
				errorFlag = 1;
				return;
			}
		}
		tokenCtr++;
		emit(5, level - table[symidx].level, symidx);
		// printf("Symbol Index: %d\n Symbol Address %d\n", symidx, table[symidx].addr);

	}
}

void condition(lexeme *list){
	// printf("condition has been called\n");
	if (list[tokenCtr].type == oddsym){
		tokenCtr++;
		expression(list);
		if (errorFlag == 1){
			return;
		}
		// ODD
		emit(2, 0, 6);
	}
	else {
		expression(list);
		if (errorFlag == 1){
			return;
		}
		if (list[tokenCtr].type == eqlsym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// EQL
			emit(2, 0, 8);
		}
		else if (list[tokenCtr].type == neqsym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// NEQ
			emit(2, 0, 9);
		}
		else if (list[tokenCtr].type == lsssym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// LSS
			emit(2, 0, 10);
		}
		else if (list[tokenCtr].type == leqsym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// LEQ
			emit(2, 0, 11);
		}
		else if (list[tokenCtr].type == gtrsym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// GTR
			emit(2, 0, 12);
		}
		else if (list[tokenCtr].type == geqsym){
			tokenCtr++;
			expression(list);
			if (errorFlag == 1){
				return;
			}
			// GEQ
			emit(2, 0, 13);
		}
		else {
			// Errors if the token is not a relational operator such as == 
			printparseerror(10);
			errorFlag = 1;
			return;
		}
	}
}

void expression(lexeme *list){
	// printf("expression has been called\n");
	if (list[tokenCtr].type == subsym){
		tokenCtr++;
		term(list);
		if (errorFlag == 1){
			return;
		}
		//NEG
		emit(2, 0, 1);
		while (list[tokenCtr].type == addsym || list[tokenCtr].type == subsym){
			if (list[tokenCtr].type == addsym){
				tokenCtr++;
				term(list); // Get next factor for math
				if (errorFlag == 1){
					return;
				}
				// ADD
				emit(2, 0, 2);
			}
			else {
				tokenCtr++;
				term(list); // Get next factor for math
				if (errorFlag == 1){
					return;
				}
				// SUB
				emit(2, 0, 3);
			}
		}
	}
	else {
		if (list[tokenCtr].type == addsym)
			tokenCtr++;
		term(list);
		if (errorFlag == 1){
			return;
		}
		while (list[tokenCtr].type == addsym || list[tokenCtr].type == subsym){
			if (list[tokenCtr].type == addsym){
				tokenCtr++;
				term(list); // Get next factor for math
				if (errorFlag == 1){
					return;
				}
				// ADD
				emit(2, 0, 2);
			}
			else {
				tokenCtr++;
				term(list); // Get next factor for math
				if (errorFlag == 1){
					return;
				}
				// SUB
				emit(2, 0, 3);
			}
		}
		// Bad Arithmatic Error
		if (list[tokenCtr].type == identsym || list[tokenCtr].type == numbersym || list[tokenCtr].type == oddsym){
			printparseerror(17);
			errorFlag = 1;
			return;
		}
	}
}

void term(lexeme *list){
	// printf("term has been called\n");
	// Get a factor for the math
	factor(list);
	if (errorFlag == 1){
		return;
	}
	// check for multiplication, division, and modulo since those have higher priority than + -
	while (list[tokenCtr].type == multsym || list[tokenCtr].type == divsym ||
		   list[tokenCtr].type == modsym)
	{
		if (list[tokenCtr].type == multsym){
			tokenCtr++;
			factor(list); // Get next factor for math
			if (errorFlag == 1){
				return;
			}
			// MUL
			emit(2, 0, 4);
		} else if (list[tokenCtr].type == divsym){
			tokenCtr++;
			factor(list); // Get next factor for math
			if (errorFlag == 1){
				return;
			}
			// DIV
			emit(2, 0, 5);
		} else {
			tokenCtr++;
			factor(list); // Get next factor for math
			if (errorFlag == 1){
				return;
			}
			// MOD
			emit(2, 0, 7);
		}
	}
}

void factor(lexeme *list){
	// printf("factor has been called\n");
	// Case where the token is an identifer
	if (list[tokenCtr].type == identsym){
		// Check for the identifer as a variable and constant
		int symIdx_var = findSymbol(list[tokenCtr], 2);
		int symIdx_const = findSymbol(list[tokenCtr], 1);
		// If no symbol is found then
		if (symIdx_var == -1 && symIdx_const == -1){
			// Print a specific error if the ident was a procedure
			if (findSymbol(list[tokenCtr], 3) != -1){
				printparseerror(11);
				errorFlag = 1;
				return;
			// Print an error for invalid identifer
			} else {
				printparseerror(19);
				errorFlag = 1;
				return;
			}
		}
		// Emit to the code 
		if (symIdx_var == -1){ // const
			// LIT
			emit(1, 0, table[symIdx_const].val);
		} else if (symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level){
			// LOD
			emit(3, level - table[symIdx_var].level, table[symIdx_var].addr);
		} else {
			// LIT
			emit(1, 0, table[symIdx_const].val);
		}
		tokenCtr++;
	} else if (list[tokenCtr].type == numbersym){
		// LIT
		emit(1, 0, list[tokenCtr].value);
		tokenCtr++;
	} else if (list[tokenCtr].type == lparensym){
		tokenCtr++;
		// Get the code inside of the parenthesis 
		expression(list);
		if (errorFlag == 1){
			return;
		}
		// Error if no right parenthesis is found
		if (list[tokenCtr].type != rparensym){
			printparseerror(12);
			errorFlag = 1;
			return;
		}
		tokenCtr++;
		// Error since only math expressions can only have const, var, num, and parenthesis
	} else{
		printparseerror(11);
		errorFlag = 1;
		return;
	}
}

/*This function should do a linear pass through the symbol table looking for 
the symbol name given. If it finds that name, it checks to see if it’s 
unmarked (no? keep searching). If it finds an unmarked instance, it checks 
the level. If the level is equal to the current level, it returns that index. 
Otherwise it keeps searching until it gets to the end of the table, and if 
nothing is found, returns -1*/
int multipleDeclarationCheck(lexeme token)
{
	// Loop through symbol table
	for (int i = 0; i < tIndex; i++){
		// check if name is the same
		if (strcmp(table[i].name, token.name) == 0){
			// check if the symbol is unmarked
			if(table[i].mark == 0){
				// check if the symbol is at the same level
				if (table[i].level == level){
					return i;
				}
			}
		}
	}
	return -1;
}
/*
This function does a linear search for the given name. An entry only matches 
if it has the correct name AND kind value AND is unmarked. Then it tries to 
maximize the level value */
int findSymbol(lexeme token, int type)
{
	int symbolIndex = -1;
	for (int i = 0; i < tokenCtr; i++){
		if (strcmp(table[i].name, token.name) == 0 && table[i].kind == type && table[i].mark == 0){
			// This will get the index of the symbol of the highest level
			if (symbolIndex < table[i].level)
				symbolIndex = i;
		}
	}
	return symbolIndex;
}

/* This function starts at the end of the table and works backward. It ignores 
marked entries. It looks at an entry’s level and if it is equal to the current 
level it marks that entry. It stops when it finds an unmarked entry whose 
level is less than the current level */
void mark(){
	// printf("\n");
	// Loop and mark entries at current level
	for (int i = tIndex -1; i >= 0; i--){
		// Symbol is considered marked if the mark var is = 1
		if (table[i].mark == 1)
			continue;
		// Stop looking if current entry has a level less than current level
		if (table[i].level < level){
			break;
		}
		if (table[i].level == level){
			// printf("%s marked!\n", table[i].name);
			table[i].mark = 1;
		}
	}
}

void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}


void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}