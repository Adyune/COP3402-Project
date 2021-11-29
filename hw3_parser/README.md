# COP3402-Project
Group Members:Adam Lei,
              Joseph Ratchford,
              Ra'id Naeem,
              Suhaib Hasan
              
              
To run the program use
gcc parser.c lex.o parser.c vm.o

Then,

./a.exe input.txt additionalInput

Note: if you're running this on eustis run
./a.out input.txt additionalInput

Note: additionalInput means typing in -l, -s, -a, and -v alongside ./a.out

-l : print the list and table of lexemes/tokens (HW2 output) to the screen
-s : print the symbol table
-a : print the generated assembly code (parser/codegen output) to the screen
-v : print virtual machine execution trace (HW1 output) to the screen

EX:
./a.out input.txt –l –a –v  Print all types of output to the console

./a.out input.txt –v        Print only the VM execution trace to the console

./a.out input.txt           Print nothing to console except read and write instructions
