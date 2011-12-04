/*
 *      astar.c
 *
 * The main driver for the A star search.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "statenode.h"
#include "queue.h"


struct stateNode* search(struct stateNode*, struct stateNode*);
void generate(queue* , struct stateNode*, struct stateNode*);
void readState(struct stateNode*, char*);
int newState(struct stateNode*,struct stateNode**,struct stateNode*, int, int);

/*
 *
 * name: main
 *
 * Recieves information from the user of the input and output files, and then
 * makes appropriate calls.
 *
 * @param	argc	the number of arguments passed (including the program)
 * @param	argv	the argument array of the program call
 * @return	error code
 */
int main(int argc, char ** argv){
	// The user can pass a parameter to the program for the file name.
	// If no parameter is given, the program will ask explicitly.
	char input[MAX_FILE_LEN];
	if(argc == 2){
		strcpy(input, argv[1]);
	}
	else{
		printf("\n Name of your input file (%d characters max): ", MAX_FILE_LEN);
		scanf("%s", input);
	}

	struct stateNode init;
	readState(&init, input);
	struct stateNode goal = {
		0, // heuristic 
		{GOAL_STATE}, // state
		0, // pathCost
		NULL // predecessor
	};

	h(&init, &goal);
	if(!solvable(&init, &goal)){
		printNode(&init);
		printf("\nUnsolvable\n\n");
		return 0;
	}
	printNodePath(search(&init, &goal));

	return 0;
}

/*
 *
 * name: readState
 *
 * Opens the file located by input and reads the state of the node into the
 * given stateNode.
 *
 * @param	sn	the stateNode to be stored into
 * @param	input	the file to be read from
 */
void readState(struct stateNode * sn, char * input){
	FILE* infile;
	infile = fopen(input, "r");
	if(infile != NULL){
		int i;
		for(i=0; i<BOARD_SIZE; i++){
			fscanf(infile, "%d ", &sn->state[i]);
		}
		sn->pathCost = 0;
		sn->pred = NULL;
		sn->heuristic = 0;
		fclose(infile);
	}
	else{
		printf("\n\nCould not open input file!\n");
		exit(1);
	}
}

/*
 *
 * name: search
 *
 * Uses the A* algorithm to search for the path to the given goal from the
 * initial stateNode.
 *
 * @param	initial	the stateNode to start the search from
 * @param	goal	the stateNode to be searched for
 * @return	the goal state found, with updated paths
 */
struct stateNode* search(struct stateNode* initial, struct stateNode* goal){
	struct stateNode * current, * next, * prior;

	queue open;
	queue closed;
	queue children;
	initialize(&open);
	initialize(&closed);
	initialize(&children);
	
	insertSorted(&open, initial);

	while(!isEmpty(&open)){
		current = min(&open);
		
		if(cmp(current, goal)){
			return current;
		}

		insert(&closed, current);
		// get all the children nodes of then current
		generate(&children, current, goal);

		// inspect each child to see which list to place it
		while(!isEmpty(&children)){
			next = min(&children);
			prior = in(&closed, next);

			if(prior != NULL){
				if(cost(next) < cost(prior)){
					searchAndDestroy(&closed, prior);
					insert(&closed, next);
				}
				else{
					free(next); // not any better so chunk it.
				}
			}
			else{
				insertSorted(&open, next);
			}
		}
	}
	return NULL;

}

/*
 *
 * name: generate
 *
 * Generates all valid moves from the given parent move and stores them in the
 * children queue.  The goal state is used to calculate h(n) for each child.
 *
 * @param	children	the queue to be stored into
 * @param	parent	the starting node to generate children from
 * @param	goal	the goal state
 */
void generate(queue * children, struct stateNode* parent, struct stateNode* goal){
	// find the 0 (empty) pos
	int i;
	for(i=0; i< BOARD_SIZE; i++){
		if(parent->state[i] == 0){
			break;
		}
	}

	struct stateNode * tempfant;

	// i is now the 0 pos
	if((i% SIZE) == 0){
		// do not subtract 1
		// since this is the left edge
		if(newState(parent, &tempfant, goal, i, i+1)){
			insert(children, tempfant);
		}
	}
	else if ((i% SIZE) == (SIZE-1)){
		// do not add 1
		// since this is the right edge
		if(newState(parent, &tempfant, goal, i, i-1)){
			insert(children, tempfant);
		}
	}
	else{
		// all moves are valid
		// since we're not on an edge
		if(newState(parent, &tempfant, goal, i, i-1)){
			insert(children, tempfant);
		}
		if(newState(parent, &tempfant, goal, i, i+1)){
			insert(children, tempfant);
		}
	}

	// these, on the other hand are manditory.
	// newState will not return true if the move is off the board
	if(newState(parent, &tempfant, goal, i, i-SIZE)){
		insert(children, tempfant);
	}
	if(newState(parent, &tempfant, goal, i, i+SIZE)){
		insert(children, tempfant);
	}
}

/*
 *
 * name: newState
 *
 * Using the given parameters, this function will store into *new the node it 
 * is to create from the seed.  It will swap positions given by empty and
 * swap and then calculate it's Manhattan distance from the given goal.  Will
 * return 0 if the given swap value is out of range of the board and not valid.
 *
 * @param	seed	the stateNode to be generated from
 * @param	new	the stateNode pointer to be stored into
 * @param	goal	the goal state
 * @param	empty	the position of the 0 or empty space on the board
 * @param	swap	the position of the tile to be slid into the empty
 */
int newState(struct stateNode * seed, struct stateNode ** new, 
		struct stateNode * goal, int empty, int swap){
	if(swap >= 0 && swap < BOARD_SIZE){
		// get a new chunk of memory
		*new = malloc(sizeof(struct stateNode));
		if(*new == NULL){
			printf("out of memory");
			exit(1);
		}

		// copy the board
		int i;
		for(i=0; i<BOARD_SIZE; i++){
			(*new)->state[i] = seed->state[i];
		}
		
		// swap the positions
		int tmp = (*new)->state[swap];
		(*new)->state[swap] = (*new)->state[empty];
		(*new)->state[empty] = tmp;
		
		// update the costs
		h(*new, goal);
		(*new)->pathCost = seed->pathCost + PATH_COST;	
		(*new)->pred = seed;
		return 1;
	}
	return 0;
}
