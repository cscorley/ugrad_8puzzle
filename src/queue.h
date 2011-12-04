/*
 *      queue.h
 *
 * This file contains the struct to be used in the queue and a couple of 
 * functions to be implemented for the queue.
 *
 */

#ifndef queue_h
#define queue_h
#include "config.h"
#include "statenode.h"

struct node{
	struct stateNode * state;
	struct node * next;
};

typedef struct{
	int count;
	struct node * head;
	struct node * tail;
} queue;

void initialize(queue*);
struct stateNode* in(queue*, struct stateNode*);
int isEmpty(queue*);
void insert(queue*,struct stateNode*);
void insertSorted(queue*,struct stateNode*);
void searchAndDestroy(queue*,struct stateNode*);
struct stateNode * min(queue*);

#endif
