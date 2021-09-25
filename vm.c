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
    FREE = IC + 40; 
    BP = IC;
    PC = 0;
    SP = MAX_PAS_LENGTH;

    // Print the column headers to console
    printf("\t\tPC\tBP\tSP\tDP\tData\n");
    printf("Inital values:\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);

    // fetch & execute cycle
    int flag = 1; 
    // Use while loop with a switch case to determine the type of command
    while(flag){
        IR[0] = PAS[PC];
        IR[1] = PAS[PC + 1];
        IR[2] = PAS[PC + 2];
        switch (IR[0])
        {
        // LIT 0, M
        case 1:
            if (BP == GP){
                DP++;
                PAS[DP] = IR[2];
            }
            else {
                SP--;
                PAS[SP] = IR[2];
            }
            print_execution(PC/3, "LIT", IR, PC, BP, SP, DP, PAS, GP);
            PC += 3;
            break;
        // OPR 0, # use another switch case to determine operation
        case 2:
            switch (IR[2])
            {
            // RTN
            case 0:
                SP = BP + 1;
                BP = PAS[SP - 2];
                print_execution(PC/3, "RTN", IR, PC, BP, SP, DP, PAS, GP);
                PC = PAS[SP - 3];
                break;
            // NEG
            case 1:
                if (BP == GP)
                    PAS[DP] = -1 * PAS[DP];
                else
                    PAS[SP] = -1 * PAS[SP];
                print_execution(PC/3, "NEG", IR, PC, BP, SP, DP, PAS, GP);
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
                print_execution(PC/3, "ADD", IR, PC, BP, SP, DP, PAS, GP);
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
                print_execution(PC/3, "SUB", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
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
                print_execution(PC/3, "MUL", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // DIV
            case 5:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP]/PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP]/PAS[SP - 1];
                }
                print_execution(PC/3, "DIV", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // ODD
            case 6:
                if(BP == GP){
                    PAS[DP] = PAS[DP] % 2;
                }
                else{
                    PAS[SP] = PAS[SP] % 2;
                }
                print_execution(PC/3, "ODD", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // MOD
            case 7:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] % PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] % PAS[SP-1];
                }
                print_execution(PC/3, "MOD", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // EQL
            case 8:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] == PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] == PAS[SP - 1];
                }
                print_execution(PC/3, "EQL", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // NEQ
            case 9:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] != PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] != PAS[SP - 1];
                }
                print_execution(PC/3, "NEQ", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // LSS
            case 10:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] < PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] < PAS[SP - 1];
                }
                print_execution(PC/3, "LSS", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // LEQ
            case 11:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] <= PAS[DP+1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] <= PAS[SP - 1];
                }
                print_execution(PC/3, "LEQ", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // GTR
            case 12:
                if(BP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] > PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] > PAS[SP -1];
                }
                print_execution(PC/3, "GTR", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // GEQ
            case 13:
                if(DP == GP){
                    DP--;
                    PAS[DP] = PAS[DP] >= PAS[DP + 1];
                }
                else{
                    SP++;
                    PAS[SP] = PAS[SP] >= PAS[SP - 1];
                }
                print_execution(PC/3, "GEQ", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            default:
                printf("ERROR OP");
                exit(1);
                break;
            }
            break;
              // LOD L, M
        case 3:
            if (BP == GP){
                DP++;
                PAS[DP] = PAS[GP + IR[2]];
            }
            else {
                if (base(IR[1]) == GP){
                    SP--;
                    PAS[SP] = PAS[GP + IR[2]];
                }
                else {
                    SP--;
                    PAS[SP] = PAS[base(IR[1]) - IR[2]];
                }
            }
            print_execution(PC/3, "LOD", IR, PC, BP, SP, DP, PAS, GP);
            PC += 3;
            break;
        // STO L, M
        case 4:
            if(BP == GP){
                PAS[GP + IR[2]] = PAS[DP];
                DP--;
            }
            else {
                if (base(IR[1]) == GP){
                    PAS[GP + IR[2]] = PAS[SP];
                    SP++;
                }
                else {
                    PAS[base(IR[1]) - IR[2]] = PAS[SP];
                    SP++;
                }
            }
            print_execution(PC/3, "CAL", IR, PC, BP, SP, DP, PAS, GP);
            break;
        // CAL 0, M
        case 5:
            PAS[SP - 1] = IR[1];
            PAS[SP -2] = BP;
            PAS[SP - 1] = PC;
            BP = SP -1;
            print_execution(PC/3, "CAL", IR, PC, BP, SP, DP, PAS, GP);
            PC = IR[2];
            break;
        // INC 0, M
        case 6:
            if(BP == GP){
                DP = DP + IR[2];
                    
            }
            else{
                SP = SP - IR[2];
            }
            print_execution(PC/3, "INC", IR, PC, BP, SP, DP, PAS, GP);
            break;
        // JMP 0, M
        case 7:
            print_execution(PC/3, "JMP", IR, PC, BP, SP, DP, PAS, GP);
            PC = IR[2];
            break;
        // JPC 0, M
        case 8:
            if(BP == GP){
                if(PAS[DP] == 0){
                    PC = IR[2];
                    DP--;
                }
                else{
                    PC += 3;
                }
            }
            else{
                if(PAS[SP] == 0){
                    PC = IR[2];
                    SP++;
                }
                else{
                    PC += 3;
                }
            }
            print_execution(PC/3, "JPC", IR, PC, BP, SP, DP, PAS, GP);
            break;
        // SYS 0, #
        case 9:
            switch (PAS[2])
            {
            // Write the top stack element or data element to screen
            case 1:
                if(BP == GP){
                    printf("%d", PAS[DP]);
                    DP--;
                }
                else{
                    printf("%d", PAS[SP]);
                    SP++;
                }
                print_execution(PC/3, "SYS", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // Read in input from the user and store it on top of the stack (or data section)
            case 2:
                if(BP == GP){
                    DP++;
                    scanf("%d", PAS[DP]);
                }
                else{
                    SP--;
                    scanf("%d", PAS[SP]);
                }
                print_execution(PC/3, "SYS", IR, PC, BP, SP, DP, PAS, GP);
                PC += 3;
                break;
            // End of program
            case 3:
                print_execution(PC/3, "SYS", IR, PC, BP, SP, DP, PAS, GP);
                flag = 0;
                break;
            default:
                printf("Error SYS");
                exit(1);
                break;
            }
            break;
        default:
            printf("ERROR MAIN");
            exit(1);
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
// Line can be calulated using PC/3
void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP)
{
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
    // print data section
    for (i = GP; i <= DP; i++)
        printf("%d ", pas[i]);
    printf("\n");
    // print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
        printf("%d ", pas[i]);
    printf("\n");
}
