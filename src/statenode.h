/*
 *      statenode.h
 *
 * This file contains the struct to be used in the stateNode and a couple of 
 * functions to be implemented.
 *
 */
#ifndef STATENODE_H
#define STATENODE_H
#include "config.h"

struct stateNode{
	int heuristic;
	int state[BOARD_SIZE];
	int pathCost;
	struct stateNode * pred;
};


int cost(struct stateNode*);
int cmp(struct stateNode*, struct stateNode*);
void printNode(struct stateNode*);
void printNodePath(struct stateNode*);
void h(struct stateNode*, struct stateNode*);
int solvable(struct stateNode*, struct stateNode*);

#endif 
