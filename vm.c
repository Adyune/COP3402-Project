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
    // read from command line and put values from input into PAS
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
        IR[PC] = PAS[PC];
        IR[PC + 1] = PAS[PC + 1];
        IR[PC + 2] = PAS[PC + 2];
        switch (IR[PC])
        {
        // LIT 0, M
        case 1:
            if (BP == GP){
                DP++;
                PAS[DP] = IR[PC + 2];
            }
            else {
                SP--;
                PAS[SP] = IR[PC + 2];
            }
            PC += 3;
            break;
        // OPR 0, # use another switch case to determine operation
        case 2:
            switch (IR[PC + 2])
            {
            // RTN
            case 0:
                SP = BP + 1;
                BP = PAS[SP - 2];
                PC = PAS[SP - 3];
                break;
            // NEG
            case 1:
                if (BP == GP)
                    PAS[DP] = -1 * PAS[DP];
                else
                    PAS[SP] = -1 * PAS[SP];
                PC += 3;
                break;
            // ADD
            case 2:
                if (BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] + PAS[DP + 1];
                }
                else {
                    SP++;
                    PAS[SP] = PAS[SP] + PAS[SP - 1];
                }
                PC += 3;
                break;
            // SUB
            case 3:
                if (BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] - PAS[DP + 1];
                }
                else {
                    SP++;
                    PAS[SP] = PAS[SP] - PAS[SP - 1];
                }
                PC += 3
                break;
            // MUL
            case 4:
                if (BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] * PAS[DP + 1];
                }
                else {
                    SP++;
                    PAS[SP] = PAS[SP] * PAS[SP - 1];
                }
                PC += 3;
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
            switch (IR[PC + 2])
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
