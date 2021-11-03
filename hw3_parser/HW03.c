
// Pseudo Code 
PROGRAM
emit JMP 
add to symbol table (kind 3, “main”, 0, level = 0, 0, unmarked)
level = -1
BLOCK
if token != periodsym
error
emit HALT
for each line in code
if line has OPR 5 (CALL)
code[line].m = table[code[line].m].addr
code[0].m = table[0].addr
BLOCK
Increment level
procedure_idx = current symbol table index - 1
CONST-DECLARATION
x = VAR-DECLARATION
PROCEDURE-DECLARATION
table[procedure_idx].addr = current code index * 3
if level == 0
emit INC (M = x)
else
emit INC (M = x + 3)
STATEMENT
MARK
Decrement level
CONST-DECLARATION
if token == const
do
get next token
if token != identsym
error
symidx = MULTIPLEDECLARATIONCHECK(token)
if symidx != -1
error
save ident name
get next token
if token != assignsym
error
get next token
if token != numbersym
error
add to symbol table (kind 1, saved name, number, level, 0, unmarked)
get next token
while token == commasym
if token != semicolonsym
if token == identsym
error
else
error
get next token
VAR-DECLARATION
numVars = 0
if token == varsym
do
numVars++
get next token
if token != identsym
error
symidx = MULTIPLEDECLARATIONCHECK(token)
if symidx != -1
error
if level == 0
add to symbol table (kind 2, ident, 0, level, numVars-1, unmarked)
else
add to symbol table (kind 2, ident, 0, level, numVars+2, unmarked)
get next token
while token == commasym
if token != semicolonsym
if token == identsym
error
else
error
get next token
return numVars
PROCEDURE-DECLARATION
while token == procsym
get next token
if token != identsym
error
symidx = MULTIPLEDECLARATIONCHECK(token)
if symidx != -1
error
add to symbol table (kind 3, ident, 0, level, 0, unmarked)
get next token
if token != semicolonsym
error
get next token
BLOCK
if token != semicolonsym
error
get next token
emit RTN
STATEMENT
if token == identsym  
symIdx = FINDSYMBOL (token, kind 2)
if symIdx == -1
if FINDSYMBOL (token, 1) != FINDSYMBOL (token, 3)
error
else
error
get next token
if token != assignsym
error
get next token
EXPRESSION
emit STO (L = level – table[symIdx].level, M = table[symIdx].addr)
return
if token == beginsym
do
get next token
STATEMENT
while token == semicolonsym
if token != endsym
if token == identsym, beginsym, ifsym, whilesym, readsym, writesym,or 
callsym
error
else
error
get next token
return
if token == ifsym
get next token
CONDITION
jpcIdx = current code index
emit JPC
if token != thensym
error
get next token
STATEMENT
if token == elsesym
jmpIdx = current code index
emit JMP
code[jpcIdx].m = current code index * 3
STATEMENT
code[jmpIdx].m = current code index * 3
else
code[jpcIdx].m = current code index * 3
return
if token == whilesym
get next token
loopIdx = current code index
CONDITION
if token != dosym
error
get next token
jpcIdx = current code index
emit JPC
STATEMENT
emit JMP M = loopIdx * 3
code[jpcIdx].m = current code index * 3
return
if token == readsym
get next token
if token != identsym
error
symIdx = FINDSYMBOL (token, kind 2)
if symIdx == -1
if FINDSYMBOL (token ,1) != FINDSYMBOL(token, 3)
error
else
error
get next token
emit READ
emit STO (L = level – table[symIdx].level, M = table[symIdx].addr)
return
if token == writesym
get next token
EXPRESSION
emit WRITE
return
if token == callsym
get next token
symIdx = FINDSYMBOL (token, kind 3)
if symIdx == -1
if FINDSYMBOL (token, 1) != FINDSYMBOL(token, 2)
error
else
error
get next token
emit CAL (L = level – table[symIdx].level, symIdx)
CONDITION
if token == oddsym
get next token
EXPRESSION
emit ODD
else
EXPRESSION
if token == eqlsym 
get next token
EXPRESSION
emit EQL
else if token == neqsym
get next token
EXPRESSION
emit NEQ
else if token == lsssym
get next token
EXPRESSION
emit LSS
else if token == leqsym
get next token
EXPRESSION
emit LEQ
else if token == gtrsym
get next token
EXPRESSION
emit GTR
else if token == geqsym
get next token
EXPRESSION
emit GEQ
else
error
EXPRESSION
if token == subsym
get next token
TERM
emit NEG
while token == addsym || token == subsym
if token == addsym
get next token
TERM
emit ADD
else
get next token
TERM
emit SUB
else
if token == addsym
get next token
TERM
while token == addsym || token == subsym
if token == addsym
get next token
TERM
emit ADD
else
get next token
TERM
emit SUB
if token == ( identifier number odd
error
TERM
FACTOR
while token == multsym || token == divsym || token == modsym
if token == multsym
get next token
FACTOR
emit MUL
else if token == divsym
get next token
FACTOR
emit DIV
else
get next token
FACTOR
emit MOD
FACTOR
if token == identsym
symIdx_var = FINDSYMBOL (token, 2)
symIdx_const = FINDSYMBOL(token, 1)
if symIdx_var == -1 && symIdx_const == -1
if FINDSYMBOL(token, 3) != -1
error
else
error
if symIdx_var == -1 (const)
emit LIT M = table[symIdx_const].val
else if symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level
emit LOD(L = level–table[symIdx_var].level, M = table[symIdx_var].addr)
else 
emit LIT M = table[symIdx_const].val
get next token
else if token == numbersym
emit LIT
get next token
else if token == lparentsym
get next token
EXPRESSION
if token != rparentsym
error
get next token
else
error
