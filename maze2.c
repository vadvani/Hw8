#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maze.h"

typedef struct stackElem *Stack;

/*Veena Advani
CPSC 223 - Hw9
Due: April 15th, 2015
Citation: modified stack code from CS website. also modified some of DFS code as well.
*/

struct position {
	int row;
	int col;
};

struct stackElem {
	struct position *p;
	struct stackElem* next;
};

struct image{
	int width;
	int height;
	int** image;
};

void stackPush (Stack *s, int row, int col) {
	struct stackElem *e;
	struct position* p;
	e = malloc(sizeof(struct stackElem));
	assert(e);
	p = malloc(sizeof(struct position));
	assert(p);

	p->row = row;
	p->col = col;

	e->p = p; /*does this work?  just set e->p to point to the position struct?*/

	e->next = *s;
	*s = e;
}

int stackEmpty (const Stack *s) {
	return (*s == 0);
}

/*REMEMBER - NEED TO FREE POSITION STRUCT AT SOME POINT LATER*/
struct position* stackPop (Stack *s) {
	struct position *retP;
	struct stackElem *e;

	assert(!stackEmpty(s));
	retP=(*s)->p;

	e = *s;
	*s = e->next;

	free(e);

	return retP;
}

struct image* imageCreate(void) {
	struct image* i;
	int c;

	i = malloc(sizeof(struct image));
	assert(i);

	scanf ("P5 %d %d 255\n", &(i->width), &(i->height));
	
	i->image = malloc(sizeof(int*) * height); /*using right sizeof here???*/
	assert(i->image);
	for (int j = 0; j < i->width; j++) {
		i->image[j] = malloc(sizeof(int) * width);
		assert(i->image[j]);
	}

	for (int j=0; j<height; j++) {
		for (int k=0; k<width; k++) {
			c = getchar();
			i->image[j][k].val = c;
		}
	}

	return i;
}

void printImage(struct image* i) {
	printf("P5 %d %d 255\n", i->width, i->height);
	for (int j=0; j<height; j++) {
		for (int k=0; k<width; k++) {
			printf("%d", i->image[j][k]);
		}
	}
}
int countNeighbors(struct image* i, int row, int col) {
	 int count;
	 for (int i = row - 1; i <= row + 1; i++) {
	 	for (int j = col - 1; j <= col + 1; j++) {
	 		if ((j == col) && (i == row)) {
	 			continue;
	 		} else {
	 			if (i->image[i][j] != 0) {
	 				count++;
	 			}
	 		}
	 	}
	 }
	 return count;
}

/*this function takes in a pointer to an image struct and returns a malloced position struct with the location of a dead end, at which
we can start the program -> later, will take this position struct --> enqueue neighbors --> since only one neighbor --> will get set to dead value, 
then you'll start loop of pop next node, enqueue neighbors of that, and keep adjusting the values within this position struct*/

struct position* findStartPt (struct image* i) {
	struct position* p;
	p = malloc(sizeof(struct position));
	assert(p);

	for (int j = 0; j <= i->height; j++) {
		for (int k = 0; k <= i->width; k++) {
			if (countNeighbors(i, j, k) == 1) {
				p->row = j;
				p->col = k;
				return p;
			}
		}
	}
}

/*dead neighbors = -1. visited = 2; enqueued = 3; untouched = 0 or 1*/
int nonDeadNeighbors(const struct image* i, const int row, const int col) {
	int count;
	 for (int i = row - 1; i <= row + 1; i++) {
	 	for (int j = col - 1; j <= col + 1; j++) {
	 		if ((j == col) && (i == row)) {
	 			continue;
	 		} else if (i->image[i][j] > 0) { /*neighbor is not dead and is not a wall*/
	 			count++;
	 		}
	 	}
	 }
	 return count;
}

/*this function takes in a MALLOCED location of a deadend node, finds the single nondead neighbor, and modifies the position struct to contain the neighbor
of the deadend node*/
void findNeighbor(struct image* i, struct position* p) {
	/*first we need to find the single, non-dead neighbor (visited node we used to reach this dead end)*/
	for (int i = p->row - 1; i <= p->row + 1; i++) {
		for (int j = p->col - 1; j <= p->col + 1; j++) {
			if ((j==p->col) && (i==p->row)) {
				continue;
			} else if (i->image[i][j] != -1) { /*found the nondead neighbor*/
				p->row = i; /*set position struct to contain neighbor coordinates*/
				p->col = j;

			}

		}
	}
}

void setEnqueue(struct image* i, int row, int col) {
	for (int j = row - 1; j <= row + 1; j++) {
		for (int k = col - 1; k <= col + 1; k++) {
			if ((j==row) && (k==col)) { /*DO I EVEN NEED THIS???*/
				continue;
			}
			if (i->image[j][k] == 3) { /*found the enqueued element*/
				i->image[j][k] = 2; /*set it to visited, to mark the end of our loop*/
			}
		}
	}
}

/*this does not necessarily preserve input position struct - is that okay???!!!*/
int enqNeighbors (struct image* i, Stack s, struct position* p) {
	int count;
	int queueCount;
	int count = 0;
	int queueCount = 0;
	count = nonDeadNeighbors(i, p->row, p->col);
	if (count == 1) { /*only one non-dead neighbor --> need to mark this current spot as dead and then figure out whether need to backtrack or not*/
		i->image[p->row][p->col] = -1; /*mark this node as dead-end*/
		findNeighbor(i, p);
		if (i->image[p->row][p->col] == 2) { /*single non-dead neighbor has been visited --> need to backtrack*/
			while (nonDeadNeighbors(i, p->row, p->col) == 1) { /*while there is only one non-dead neighbor, set location to dead end and check the non-dead neighbor, keep going till it splits somewhere*/
				i->image[p->row][p->col] = -1;
				findNeighbor(i, p);
			}
		} else { /*otherwise, single non-dead neighbor needs to be enqueued and marked as enqueued, we know neighbor is in position p*/
			i->[p->row][p->col] = 3; /*mark this as enqueued*/
			/*now push location it onto stack*/
			stackPush(&s, p->row, p->col);
		}
	} else { /*more than one non-dead neighbor -> mark current node as visited, and enqueue all non-dead, non-visited, non-enqueued neighbors*/
		i->image[p->row][p->col] = 2;
		for (int j = p->row - 1; j <= p->row + 1; j++) {
			for (int k = p->col - 1; k <= p->col + 1; k++) {
				
				if ((j == p->row) && (k == p->col)) {
					continue;
				}

				if (i->image[j][k] == 1) { /*not dead, not visited, not already enqueued*/
					i->image[j][k] = 3; /*mark it as enqueued and then push it onto stack*/
					stackPush(&s, j, k);
					queueCount++;
				}
			
			}
		}
		if (queueCount == 0) { /*didn't enqueue anything --> in a loop --> need to find the enqueued element that's a neighbor, and mark it as visited with 2, then stop*/
			setEnqueue(i, p->row, p->col); /*IS THIS THE LOCATION WE'RE SUPPOSED TO BE INPUTTING?*/
			return 1; /*return 1 to indicate you've found the loop*/
		}
	}
	return 0; /*if you get to here --> enqNeighbors finished, no loop found, so 0 was returned, keep searching*/
}



int main (int argc, char** argv) {

	struct image* i;
	Stack s;
	struct position *p;

	i = imageCreate();
	s = 0; /*initialize the stack*/
	p = findStartPt(i);

	while (enqNeighbors(i, s, p) == 0) {
		free(p);
		p = stackPop(&s);
	}
	
	printImage(i);
	
	return 0;
}