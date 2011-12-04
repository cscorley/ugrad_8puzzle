/*
 *      queue.c
 *
 * This is the queue implementation which is sorted or unsorted depending 
 * on the chosen insert function.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "queue.h"
#include "statenode.h"

/*
 *
 * name: isEmpty
 *
 * Checks to see if the given queue has any nodes in it.
 *
 * @param	q	the queue to be checked
 * @return	0 if false, 1 if true
 */
int isEmpty(queue * q) {
	return (q->count == 0);
}

/*
 *
 * name: initialize
 *
 * Simply insures that the queue's count is set to 0 before beginning.
 *
 * @param	q	the queue to be initialized
 */
void initialize(queue * q){
	q->count = 0;
}


/*
 *
 * name: in
 *
 * Tests to see if the given stateNode sn has a matching stateNode located
 * within the queue, and  * if it is returns a pointer to the stateNode in
 * the queue.
 *
 * @param	q	the queue to be checked
 * @param	sn	the stateNode to searched for
 * @return	the stateNode found in the queue
 */
struct stateNode* in(queue * q, struct stateNode * sn) {
	struct node * temp;
	temp = q->head;
	while(temp != NULL){
		if(cmp(temp->state, sn)){
			return temp->state;
		}
		temp = temp->next;
	}
	return NULL;
}

/*
 *
 * name: insert
 *
 * Inserts the given stateNode sn at the tail of the queue.  This function
 * is to be used only when the queue is being used as a simple queue.
 *
 * @param	q	the queue to be inserted onto
 * @param	sn	the stateNode to be inserted
 */
void insert(queue * q, struct stateNode* sn) {
	struct node * newNode;
	newNode = malloc(sizeof(struct node));
	if(newNode == NULL){
		printf("out of memory");
		exit(1);
	}

	newNode->state = sn;
	newNode->next = NULL;

	if(!isEmpty(q)){
		q->tail->next = newNode;
		q->tail = newNode;
		q->count++;
	}
	else{
		q->head = newNode;
		q->tail = q->head;
		q->count = 1;
	}
}

/*
 *
 * name: insertSorted
 *
 * Inserts the given stateNode sn at the correct position in the sorted queue
 * based upon its heuristical and path cost values.  This function is to be
 * used only when the queue is being used as a priority queue.
 *
 * @param	q	the queue to be pushed onto
 * @param	sn	the stateNode to be inserted
 */
void insertSorted(queue * q, struct stateNode * sn) {
	struct node * newNode;
	newNode = malloc(sizeof(struct node));
	if(newNode == NULL){
		printf("out of memory");
		exit(1);
	}

	newNode->state = sn;

	if(!isEmpty(q)){
		// some older code which finds where to insert
		struct node * before;
		struct node * after;
		before = q->head;
		after = q->head;
		while(after != NULL && (cost(sn) > cost(after->state))){
			before = after;
			after = after->next;
		}

		newNode->next = after;

		if(after != q->head){
			before->next = newNode;
		}
		else{
			q->head = newNode;
		}

		q->count++;
	}
	else{
		q->head = newNode;
		q->head->next = NULL;
		q->tail = q->head;
		q->count = 1;
	}
}

/*
 *
 * name: min
 *
 * Gets the head of the queue, removes the head and returns the head's stateNode.
 *
 * @param	q	the queue to be removed from
 * @return	the stateNode at the head of the queue
 */
struct stateNode* min(queue * q){
	struct stateNode * returning;
	if(!isEmpty(q)){
		returning = q->head->state;
		struct node * deleting = q->head;
		q->head = q->head->next;
		q->count--;
		free(deleting);
	}
	return returning;
}

/*
 *
 * name: searchAndDestroy
 *
 * Attempts to find a given stateNode sn in the given queue and will remove
 * the node's and free up memory
 *
 * @param	q	the queue to be searched
 * @param	sn	the stateNode to be deleted
 */
void searchAndDestroy(queue* q, struct stateNode* sn){
	if(!isEmpty(q)){
		struct node * before;
		struct node * after;
		before = q->head;
		after = q->head;
		while(after != NULL){
			if(sn == after->state){
				before->next = after->next;
				free(after->state);
				free(after);
				q->count--;
				return;
			}
			before = after;
			after = after->next;
		}
	}
}
