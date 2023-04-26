#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "memory.h"
#include "executor.h"
#include "tree.h"
#include "garbage.h"

/*
*
* Data Structs
*
*/

struct frame {
	char** iLookup;
	int* iValues;
	int iLen;

	char** rLookup;
	int** rValues;
	int rLen;
};

static struct frame *callStack[20];
static int fp;

static char* funcNames[20];
static struct nodeFunction* funcBodies[20];
static int fLen;

/*
*
* Helper functions
*
*/

// If iden is an integer, return the index. Otherwise, return -1
static int searchInteger(char* iden) {
	int location = -1;
	for (int i=0; i<callStack[fp]->iLen; i++) {
		if (strcmp(callStack[fp]->iLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a record, return the index. Otherwise, return -1
static int searchRecord(char* iden) {
	int location = -1;
	for (int i=0; i<callStack[fp]->rLen; i++) {
		if (strcmp(callStack[fp]->rLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// Return function definition
struct nodeFunction* searchFunction(char* iden) {
	int location = -1;
	for (int i=0; i<fLen; i++) {
		if (strcmp(funcNames[i], iden)==0) {
			location = i;
		}
	}
	return funcBodies[location];
}

/*
*
* Memory functions
*
*/

void pushExecutePop(char* name, char** args) {
	// Get the info for the function we are calling
	struct nodeFunction* function = searchFunction(name);
	
	// Make new frame
	fp++;
	callStack[fp] = (struct frame*) calloc(1, sizeof(struct frame));

	callStack[fp]->iLookup = malloc(0);
	callStack[fp]->iValues = malloc(0);
	callStack[fp]->iLen = 0;
	
	callStack[fp]->rLookup = malloc(0);
	callStack[fp]->rValues = malloc(0);
	callStack[fp]->rLen = 0;
	
	// Create formal parameters
	for (int i=0; i<2; i++) {
		if (args[i] != NULL) {
			fp--;
			int* value = callStack[fp]->rValues[searchRecord(args[i])];
			fp++;
			callStack[fp]->rLen++;
			callStack[fp]->rLookup = realloc(callStack[fp]->rLookup, callStack[fp]->rLen*sizeof(char*));
			callStack[fp]->rLookup[callStack[fp]->rLen-1] = function->params[i];
			callStack[fp]->rValues = realloc(callStack[fp]->rValues, callStack[fp]->rLen*sizeof(int*));
			callStack[fp]->rValues[callStack[fp]->rLen-1] = value;
		}
	}
	
	
	// Execute the body of the function
	executeDeclSeq(function->ds);
	executeStmtSeq(function->ss);
	
	// pop frame
	fp--;
	garbageCount -= count;
	count=0;
	// displayGC();
}



// Add function to map
void functionAdd(char* name, struct nodeFunction* f) {
	funcNames[fLen] = name;
	funcBodies[fLen] = f;
	fLen++;
}

// Initialize data structures
// This is just called once, by executeProcedure
void memory_init() {
	fp = 0;
	fLen = 0;
	
	callStack[fp] = (struct frame*) calloc(1, sizeof(struct frame));
			
	callStack[fp]->iLookup = malloc(0);
	callStack[fp]->iValues = malloc(0);
	callStack[fp]->iLen = 0;
	
	callStack[fp]->rLookup = malloc(0);
	callStack[fp]->rValues = malloc(0);
	callStack[fp]->rLen = 0;
}

// Handle an integer or record declaration
void declare(char* iden, int type) {
	if (type == INTEGER) {
		callStack[fp]->iLen++;
		callStack[fp]->iLookup = realloc(callStack[fp]->iLookup, callStack[fp]->iLen*sizeof(char*));
		callStack[fp]->iLookup[callStack[fp]->iLen-1] = iden;
		callStack[fp]->iValues = realloc(callStack[fp]->iValues, callStack[fp]->iLen*sizeof(int));
		callStack[fp]->iValues[callStack[fp]->iLen-1] = 0;
	} else {
		callStack[fp]->rLen++;
		callStack[fp]->rLookup = realloc(callStack[fp]->rLookup, callStack[fp]->rLen*sizeof(char*));
		callStack[fp]->rLookup[callStack[fp]->rLen-1] = iden;
		callStack[fp]->rValues = realloc(callStack[fp]->rValues, callStack[fp]->rLen*sizeof(int*));
		callStack[fp]->rValues[callStack[fp]->rLen-1] = calloc(1, sizeof(int));
	}
}

// Store a value to a variable. Remember, unindexed stores to a record go to index 0
void store(char* iden, int value) {
	int location = searchInteger(iden);
	if (location == -1) {
		storeRec(iden, 0, value);
	} else {
		callStack[fp]->iValues[location] = value;
	}
}

// Read a value from a variable. Remember, unindexed reads from a record read index 0
int recall(char* iden) {
	int location = searchInteger(iden);
	if (location == -1) {
		return recallRec(iden, 0);
	} else {
		return callStack[fp]->iValues[location];
	}
}

// Store a value to a record variable, at the given index
void storeRec(char* iden, int index, int value) {
	int location = searchRecord(iden);
	if (index+1 <= callStack[fp]->rValues[location][0]) {
		callStack[fp]->rValues[location][index+1] = value;
	} else {
		printf("Runtime Error: write to index %d outside of array %s bounds!\n", index, iden);
		exit(0);
	}
}

// Read a value from a record variable, from the given index
int recallRec(char* iden, int index) {
	int location = searchRecord(iden);
	if (index+1 <= callStack[fp]->rValues[location][0]) {
		return callStack[fp]->rValues[location][index+1];
	} else {
		printf("Runtime Error: read from index %d outside of array %s bounds!\n", index, iden);
		exit(0);
	}
	return 0;
}

// Handle "id := record id" type assignment
void record(char* lhs, char* rhs) {
	int locLhs = searchRecord(lhs);
	int locRhs = searchRecord(rhs);
	callStack[fp]->rValues[locLhs] = callStack[fp]->rValues[locRhs];
}

// Handle "id := new record[<expr>]" type assignment
void allocateRecord(char* iden, int size) {
	int location = searchRecord(iden);
	callStack[fp]->rValues[location] = calloc(size+1, sizeof(int));
	callStack[fp]->rValues[location][0] = size;
}