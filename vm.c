#include <stdio.h>
#include <stdlib.h>

#define MAX_PAS_LENGTH 500

void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP);
int base(int L);

    // Process Address Space
    int PAS[MAX_PAS_LENGTH];
    // Instruction Register
    int IR[3];
    // Global Data Pointer
    int GP = 0;
    // Instruction Counter
    int IC = 0;
    // Data Pointer
    int DP = 0;
    // Heap
    int FREE = 0;
    // Base Pointer
    int BP = 0;
    // Program Counter
    int PC = 0;
    // Stack Pointer
    int SP = 0;

int main(int argc, char *argv[]){
    // Set each value in PAS to 0
    for (int i = 0; i < MAX_PAS_LENGTH; i++)
        PAS[i] = 0;
    // Create file pointer
    FILE *fp;
    //TODO read from command line and put values from input into PAS
    // Open the File
    fp = fopen(argv[1], "r");
    // Used to catch errors in opening the file
    if (fp == NULL){
        printf("File Not Found");
        exit(1);
    }
    // We don't know the size of the commands given to use thus we use a while loop
    // and scan until we reach the end of the file
    while (fscanf(fp, "%d %d %d", &IR[0], &IR[1], &IR[2]) != EOF){
        PAS[IC] = IR[0];
        PAS[IC + 1] = IR[1];
        PAS[IC + 2] = IR[2];
        //printf("%d %d %d\n", PAS[IC],  PAS[IC + 1],  PAS[IC + 2]); // Test if the commands were scanned correctly
        IC += 3;
    }
    // Close File
    fclose(fp);

    //set up registers based on location of IC
    GP = IC;
    DP = IC - 1;
    // FREE Can be allocated to any amount for this assignment
    FREE = IC + 20; 
    BP = IC;
    PC = 0;
    SP = MAX_PAS_LENGTH;

    // Print the column headers to console
    printf("\t\tPC\tBP\tSP\tDP\tData\n");
    printf("Inital values:\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);

    //TODO implement fetch & execute cycle
    int flag = 1; 
    // Use while loop with a switch case to determine the type of command
    while(flag){
        switch (PAS[PC])
        {
        // LIT 0, M
        case 1:
            break;
        // OPR 0, # use another switch case to determine operation
        case 2:
            switch (PAS[PC + 2])
            {
            // RTN
            case 0:
                break;
            // NEG
            case 1:
                break;
            // ADD
            case 2:
                break;
            // SUB
            case 3:
                break;
            // MUL
            case 4:
                break;
            // DIV
            case 5:
                break;
            // ODD
            case 6:
                break;
            // MOD
            case 7:
                break;
            // EQL
            case 8:
                break;
            // NEQ
            case 9:
                break;
            // LSS
            case 10:
                break;
            // LEQ
            case 11:
                break;
            // GTR
            case 12:
                break;
            // GEQ
            case 13:
                break;
            default:
                break;
            }
            break;
        // LOD L, M
        case 3:
            break;
        // CAL L, M
        case 4:
            break;
        // INC 0, M
        case 5:
            break;
        // INC 0, M
        case 6:
            break;
        // JMP 0, M
        case 7:
            break;
        // JPC 0, M
        case 8:
            break;
        // SYS 0, #
        case 9:
            switch (PAS[PC + 2])
            {
            // Write the top stack element or data element or data element
            case 1:
                break;
            // Read in input from the user and store it on top of the stack (or data section)
            case 2:
                break;
            // End of program
            case 3:
                flag = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

int base(int L)
{
    int arb = BP;
    // arb = activation record base
    while ( L > 0)     
    //find base L levels down
    {
        arb = PAS[arb];
        L--;
    }
    return arb;
}

// Pseudo code for implementing a fetch execution

/*01 – LIT    0,  M 		if bp == gp then {dp  dp + 1; pas[dp]  M;}
 else {sp  sp – 1; pas[sp]  M;}*/

if bp == gp
    {dp -> dp + 1;
     pas[dp]-> M;}
        else 
        {sp -> sp - 1;
         pas[sp] -> M;}


02 – OPR  0, #		(0 <= # <= 13)
0	RTN		sp  bp + 1;
bp  pas[sp - 2];
pc  pas[sp - 3];

				/*1	NEG		if bp == gp then pas[dp]  -1 * pas[dp]
									else  pas[sp]  -1 * pas[sp]

				2	ADD		if bp == gp */
	
if bp == gp 
{{dp <- dp - 1; 
          pas[dp] <- pas[dp] + pas[dp + 1]}
else   {sp <- sp + 1; 
           pas[sp] <- pas[sp] + pas[sp - 1]}
then {dp <- dp - 1; 
          pas[dp] <- pas[dp] + pas[dp + 1]}
else   {sp <- sp + 1; 
           pas[sp] <- pas[sp] + pas[sp - 1]}
	   

/*3	SUB		if bp == gp 
then { dp  dp - 1; 
          pas[dp]  pas[dp] - pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] - pas[sp - 1]}*/
	   
if bp == gp 
{ dp <- dp - 1; 
          pas[dp] <- pas[dp] - pas[dp + 1]}
else {sp < sp + 1; 
           pas[sp] <- pas[sp] - pas[sp - 1]}
	   

4	MUL		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] * pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] * pas[sp - 1]}

5	DIV		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] / pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] / pas[sp - 1]}

6	ODD		if bp == gp  then pas[dp]  pas[dp] mod 2
					 else  pas[sp]   pas[sp] mod 2



7	MOD		if bp == gp   
then {dp dsp - 1; 
          pas[dp]  pas[dp] mod pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] mod pas[sp - 1]}

8	EQL		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] == pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] == pas[sp - 1]}

9	NEQ		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] != pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] != pas[sp - 1]}

10	LSS		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] < pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] < pas[sp - 1]}

11	LEQ		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] <= pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] <= pas[sp - 1]}

12	GTR		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] > pas[dp + 1]}
else   {sp  sp + 1; 
           			 pas[sp]  pas[sp] > pas[sp - 1]}

13	GEQ		if bp == gp 
then {dp  dp - 1; 
          pas[dp]  pas[dp] >= pas[dp + 1]}
else   {sp  sp + 1; 
           pas[sp]  pas[sp] >= pas[sp - 1]}


03 – LOD L, M		if bp  == gp then 
{dp  dp + 1; pas[dp]  pas[gp + M];}
else   
{if base(L) == gp then
	{sp  sp - 1;  pas[sp]  pas[gp + M];}
else 
	{sp  sp - 1;  pas[sp]  pas[base(L) - M];}}

04 – STO L, M   		if bp == gp then 
{pas[gp + M]  pas[dp]; dp = dp – 1;}
else  
{if base(L) == gp then
	{pas[gp + M]  pas[sp]; sp = sp + 1;}
else
{pas[base(L) - M]  pas[sp]; sp  sp + 1;}}
 		    	 
05 - CAL   L, M  	pas[sp - 1]    base(L); 	/* static link (SL)
                       		pas[sp - 2]   bp;		/* dynamic link (DL)
	           		pas[sp - 3]   pc;	 		/* return address (RA)
                        		bp  sp - 1;
	          	 	pc  M;

06 – INC   0, M  		if bp == gp then dp  dp + M
else  sp  sp - M;

07 – JMP   0, M   	pc  M;

08 – JPC   0, M   	if bp == gp then
if pas[dp] == 0 then { pc  M; }
dp  dp - 1; 
				else
if pas[sp] == 0 then { pc  M; }
sp  sp + 1;

09 – SYS   0, 1    		if bp == gp then {printf(“%d”, pas[dp]); dp  dp -1;} 
else  {printf(“%d”, pas[sp]) ; sp  sp + 1;}

        SYS   0, 2		if bp == gp then {dp  dp + 1; scanf(“%d”, pas[dp]);}
                   else  {sp   sp - 1; scanf(“%d”, pas[sp]);}

        SYS    0, 3	 	Set Halt flag to zero (End of program).


void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP)
{
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
    // print data section
    for (i = GP; i <= DP; i++)
        printf("%d ", pas[i]);printf("\n");
    // print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
        printf("%d ", pas[i]);
    printf("\n");
}
