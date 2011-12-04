/*
 *      statenode.c
 *
 * A couple helpful functions related to our stateNodes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "statenode.h"


/*
 *
 * name: printNode
 *
 * Pretty prints the given stateNode.
 *
 * @param	sn	the stateNode to be printed
 */
void printNode(struct stateNode* sn){
	int i,j;
	for(i=0;i<BOARD_SIZE;i++){
		if((i%SIZE) == 0){
			printf("\n+");
			for(j=0; j<SIZE; j++){
				printf("---+");
			}
			printf("\n|");
		}
		if(sn->state[i] == 0){
			printf("   |");
		}
		else{
			printf(" %d |", sn->state[i]);
		}
	}
	printf("\n+");
	for(j=0; j<SIZE; j++){
		printf("---+");
	}
	printf("\n");
	printf("PathCost = %d\n", sn->pathCost);
	printf("Heuristic = %d\n", sn->heuristic);
}


/*
 *
 * name: printNodePath
 *
 * Traverses the given stateNode's path back to the initial state.
 *
 * @param	sn	the stateNode to be printed from
 */
void printNodePath(struct stateNode* sn){
	struct stateNode* tmp;
	tmp = sn;
	while(tmp != NULL){
		printNode(tmp);
		tmp = tmp->pred;
	}
}



/*
 *
 * name: solvable
 *
 * Returns a value if the state can reach the goal state, 0 otherwise.
 *
 * @param	sn	the stateNode to be analyzed
 * @param	goal	the goal stateNode
 * @return	0 if false, 1 if true
 */
int solvable(struct stateNode* sn, struct stateNode *goal){
	int i, j, sum, goalN, snN;

	// calculate the goal's N value
	sum = 0;
	for(i=0; i< BOARD_SIZE; i++){
		if(goal->state[i] != 0){
			for(j=i; j < BOARD_SIZE; j++){
				if(goal->state[i] > goal->state[j] && goal->state[j]!= 0){
					sum++;
				}
			}
		}
	}
	goalN = (sum + SIZE) % 2;

	// calculate the state's N value
	sum=0;
	for(i=0; i< BOARD_SIZE; i++){
		if(sn->state[i] != 0){
			for(j=i; j <BOARD_SIZE; j++){
				if(sn->state[i] > sn->state[j] && sn->state[j] != 0){
					sum++;
				}
			}
		}
	}
	snN = (sum + SIZE) % 2;

	// it will be solvable if they are the same
	return (goalN == snN);
}

/*
 *
 * name: h
 *
 * Calculates the Manhattan distance for the stateNode sn to the goal and 
 * stores it to sn.
 *
 * @param	sn	the stateNode to be stored into
 * @param	goal	the goal to be considered
 */
void h(struct stateNode* sn, struct stateNode* goal){
	int i, j, sum;
	sum = 0;
	for(i=0; i<BOARD_SIZE; i++){
		if(sn->state[i] != 0){
			for(j=0; j<BOARD_SIZE; j++){
				if(goal->state[j] == sn->state[i]){
					sum += abs((i/SIZE) - (j/SIZE))
						+ abs((i%SIZE) - (j%SIZE));
					break;
				}
			}
		}
	}
	sn->heuristic = sum;
}

/*
 *
 * name: cmp
 *
 * Compares the given stateNodes a and b to see if they have states which
 * match.
 *
 * @param	a	the stateNode to be compared
 * @param	b	the stateNode to be compared
 * @return	0 if the states do not match, 1 if they do.
 */
int cmp(struct stateNode* a, struct stateNode* b){
	int i;
	for(i=0; i<BOARD_SIZE; i++){
		if(a->state[i] != b->state[i]){
			return 0;
		}
	}
	return 1;	
}

/*
 *
 * name: cost
 *
 * Returns the cost of the given stateNode based on the heuristic and 
 * current pathCost values of the stateNode.
 *
 * @param	sn	the stateNode to be calculated
 * @return	the cost of the stateNode.
 */
int cost(struct stateNode* sn){
	return (sn->heuristic + sn->pathCost);
}

