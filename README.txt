Vincent Jia

Files:
main.c: will run everything including parse, print, and double declaration check.
parser.c: parses the tree to check if the code input is valid. Updated to accomodate procedure calls
parser.h: header file for all of the function prototypes for parser. Updated to accomodate procedure calls
README.txt: File containing information about files.
printer.c: prints the program out.
printer.h: header file for all the function prototypes for print.
scanner.c: Reads the text and returns tokens.
scanner.h: Header file for scanner.c
tester.sh: used in the testing of the program.
tree.h: contains structs for every node in the parse tree.
memory.c: contains the main code for the executor with updated procedure calls
memory.h: contains the function prototypes for memory.c1 with updated procedure call prototypes
executor.c: executes the program using similar methods to the parser. Garbage collection is added to the execution process. 
executor.h: header file for executor methods.
garbage.c: implements garbage collecting methods and displays the number of records to be collected.
garbage.h: header file for garbage methods.
Correct Folder: all of the test cases, data, and expected values of the program.

Interpreter built in C that will intake psuedocode and interpret and return the intended result.

To test the program, compile the program and run "./main Correct/1.code Correct/1.data" on the terminal which will display the results of the program.