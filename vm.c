#include <stdio.h>

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
    // We don't know the size of the commands given to use thus we use a while loop
    while (fscanf(fp, "%d %d %d", IR[0], IR[1], IR[2]) == 1){
        PAS[PC] = IR[0];
        PAS[PC + 1] = IR[1];
        PAS[PC + 2] = IR[2];
        PC += 3;
    }
    // Close File
    fclose(fp);
    //TODO set up registers based on location of IC

    //TODO implement fetch & execute cycle
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
