#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "executor.h"
#include "garbage.h"

//Display the amount of records that need to be collected
void displayGC(){
    printf("gc:%i\n", referenceCount);
}

//Remove all uncollected records at end of program
void decrementGC(){
    while(referenceCount != 0){
        referenceCount--;
        displayGC();
    }
}