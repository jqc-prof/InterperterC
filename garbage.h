#ifndef GARBAGE_H
#define GARBAGE_H

//Keep track of garbage
extern int referenceCount;

//Remove all uncollected records at end of program
void decrementGC();

//Display the amount of records that need to be collected
void displayGC();

#endif