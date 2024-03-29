#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "tree.h"

void executeProcedure(struct nodeProcedure* p);

void executeFuncSeq(struct nodeFuncSeq* fs);

void executeFunction(struct nodeFunction* f);

void executeDeclSeq(struct nodeDeclSeq* ds);

void executeDecl(struct nodeDecl* d);

void executeDeclInt(struct nodeDeclInt* di);
	
void executeDeclRec(struct nodeDeclRec* dr);

void executeStmtSeq(struct nodeStmtSeq* ss);

void executeStmt(struct nodeStmt* s);

void executeCall(struct nodeCall* c);

void executeAssign(struct nodeAssign* a);

int executeIndex(struct nodeIndex* index);

void executeOut(struct nodeOut* out);

void executeIf(struct nodeIf* ifStmt);

void executeLoop(struct nodeLoop* loop);

int executeCond(struct nodeCond* cond);

int executeCmpr(struct nodeCmpr* cmpr);

int executeExpr(struct nodeExpr* expr);

int executeTerm(struct nodeTerm* term);

int executeFactor(struct nodeFactor* factor);

int extern count;

#endif