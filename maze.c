#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maze.h"
/*to keep track of the top of the stack*/

/*Veena Advani
CPSC 223 - Hw9
Due: April 15th, 2015
Citation: Stack code modified from Stack section of Aspnes, Lecture Notes on Data Structures
 and Programming Techniques, available at http://www.cs.yale.edu/homes/aspnes/classes/223/notes.html, 
 downloaded April 10th 2015. 
*/

/*THINGS TO DO:
-check enqFunction - for general coding problems AND logic problems
-make sure not doing anything funny with find neighbors by reusing p
-ask Sam about p pointer in stackPush function and is it okay that using getchar? -> assuming char and int values the same???
-in findStartPt function - end: label --> will it still return p in case that p is 0?
-question about getchar values in createImage
ALGORITHM ANALYSIS:
-WILL THIS ALGORITHM ALWAYS WORK IF THERE IS NO CYCLE??? IF THERE IS A CYCLE???
-do I need to fix backtrack? - I don't think so...
-will I ever have bug Stan was talking about - with not starting at deadend?
-finishing stack --> guaranteed to remove tail???
-have some sort of flag that indicates when you've found cycle --> if no cycle ever found
instead of printing regularly -> call diff print function that takes every non zero number to 1?
-will I ever mark enqueued as dead??? if so --> is it a problem? - I don't think I will...*/


/*this function takes a pointer to the top of the stack, a row and column number, and pushes a new
element onto the stack with a position struct that contains those two coordinates*/
void stackPush (Stack *s, int row, int col) {
	struct stackElem *e; /*new stack element*/
	struct position* p; /*new position struct*/
	e = malloc(sizeof(struct stackElem)); /*malloc memory for the stack element*/
	assert(e); /*make sure malloc worked*/
	p = malloc(sizeof(struct position)); /*malloc memory for the position struct*/
	assert(p); /*make sure malloc worked*/

	/*initialize the row and column values in the position structs to the input values*/
	p->row = row; 
	p->col = col;

	e->p = p; /*set the position pointer in the stack element to this newly malloced position struct - THIS OKAY??? IS THERE A PROBLEM LEAVING P THERE PTING TO IT???*/

	e->next = *s; /*set the new element next pointer to the old top of the stack*/
	*s = e; /*update the top of the stack to the newly added element*/
}

/*this function takes in a pointer to the top of the stack and returns 1 if the stack is empty, 0 if it's not*/
int stackEmpty (const Stack *s) {
	return (*s == 0);
}

/*this function takes in a pointer to the top of a stack and returns a pointer to the position struct of that top element, and removes the top
element from the stack, should not be called on an empty stack*/
struct position* stackPop (Stack *s) {
	struct position *retP; /*pointer for return value/struct*/
	struct stackElem *e; /*pointer to remove the top element and free it*/

	assert(!stackEmpty(s));  /*make sure stack isn't empty --> can't pop from it*/
	retP=(*s)->p; /*set the return value to the position struct at the top of the stack*/

	e = *s; /*set e to the top of the stack - element we want to remove*/
	*s = e->next; /*cut e out of the stack (top of the stack is now the second elem)*/

	free(e); /*free the stack elem we're popping*/

	return retP; /*return the position struct - this is still malloced, it will need to be freed later*/
}

/*this function reads the input from stdin and returns a pointer to a malloced image struct that contains
the height and width of the image, as well as a 2D array that contains the values of the image/graph*/
struct image* imageCreate(void) {
	struct image* i; /*pter for the image*/
	int c; /*int c to read in chars from stdin*/

	i = malloc(sizeof(struct image)); /*malloc memory for the image struct*/
	assert(i); /*make sure malloc worked*/

	scanf ("P5 %d %d 255\n", &(i->width), &(i->height)); /*scan in values for width and height of image from top of input file*/
	
	i->image = malloc(sizeof(int*) * i->height); /* malloc memory for rows of 2D array- RIGHT SIZEOF HERE???*/
	assert(i->image); /*make sure malloc worked*/
	for (int j = 0; j < i->height; j++) { /*now malloc memory for each row and assert that it worked.*/
		i->image[j] = malloc(sizeof(int) * i->width);
		assert(i->image[j]);
	}

/*step through the whole image and read in the characters into the 2D array*/
	for (int j=0; j < i->height; j++) {
		for (int k=0; k < i->width; k++) {
			c = getchar();
			i->image[j][k] = c;
		}
	}
/*return the image struct pointer*/
	return i;
}


/*this function takes in a pointer to an image struct and returns a malloced position struct with the location of the first one (path block), at which
we can start the program and depth first search*/
struct position* findStartPt (const struct image* i) {
	struct position* p; /*pter for position struct*/
	p = 0; /*initialize p to 0, if it's still 0 at the end --> never found a start pt*/
	/*iterate through the image*/
	for (int j = 0; j < i->height; j++) {
		for (int k = 0; k < i->width; k++) {
			if (i->image[j][k] == 1) { /*found a dead end to start our search at --> set position struct values to this location*/
				p = malloc(sizeof(struct position)); /*malloc memory for it*/
				assert(p); /*make sure malloc worked*/
				p->row = j;
				p->col = k;
				goto end;
			}
		}
	}
	end: return p; /*return position struct of start location*/
}

/*In the image, deadend neighbors = -1. visited = 2; enqueued = 3; untouched = 0 or 1
This function takes in a pointer to an image struct, and a row and column location and counts all the
non-deadend neighbors of the input location*/
int nonDeadNeighbors(const struct image* i, const int row, const int col) {
	int count; /*integer to keep track of the count*/
	int initialk; /*initial for loop values set to make sure nothing is negative*/
	int initialj;
	count = 0;
	if ((row - 1) >= 0) {
		initialk = row - 1;
	}else {
		initialk = 0;
	}

	if ((col - 1) >= 0) {
		initialj = col - 1;
	} else {
		initialj = 0;
	}
/*iterate through the 8 blocks around the input location*/
	 for (int k = initialk; (k <= row + 1) && (k < i->height); k++) {
	 	for (int j = initialj; (j <= col + 1) && (j < i->width); j++) {
	 		if ((j == col) && (k == row)) { /*you're at the input location, can't be a neighbor --> skip it with continue*/
	 			continue;
	 		} else if (i->image[k][j] > 0) { /*neighbor is not dead (-1) and is not a wall (0) --> increment counter*/
	 			count++;
	 		}
	 	}
	 }
	 return count;
}

/*this function takes in a MALLOCED location of a deadend node, finds the single nondead neighbor, and modifies the position struct to contain the neighbor
coordinates of the deadend node*/
/*We need to find the single, non-dead neighbor (visited node we used to reach this dead end)*/
void findNeighbor(const struct image* i, struct position* p) {
	/*initial for loop counter value variables to make sure things aren't negative, and initial col and row values to preserve for the for loop
	as well, since p->col and p->row will be modified*/
	int initialk;
	int initialj;
	int initialCol;
	int initialRow;

/*set initial row and col variables*/
	initialCol = p->col;
	initialRow = p->row;
/*set initialk and j for loop counters*/
	if ((p->row - 1) >= 0) {
		initialk = p->row - 1;
	}else {
		initialk = 0;
	}

	if ((p->col - 1) >= 0) {
		initialj = p->col - 1;
	} else {
		initialj = 0;
	}	
/*iterate through 8 blocks around input location*/
	for (int k = initialk; (k <= initialRow + 1) && (k < i->height); k++) {
		for (int j = initialj; (j <= initialCol + 1) && (j < i->width); j++) {
			if ((j==initialCol) && (k==initialRow)) { /*we're at input location, skip it -> can't be neighbor*/
				continue;
			} else if ((i->image[k][j] != 0) && (i->image[k][j] != -1)) { /*found the nondead neighbor*/
				p->row = k; /*set position struct to contain neighbor coordinates*/
				p->col = j;

			}

		}
	}
}


/*this function takes in a pointer to an image struct, a pointer to the top of a stack, and a position struct p, and sets the value of the image
in location p to the appropriate value (2 for visited, -1 for visited but dead end), and enques all the unvisited, unenqueued neighbors
of that location onto the stack, and returns a number, 0 if everything went fine, 1 if the search is finsihed (the stack is empty and nothing was added this round)*/
int enqNeighbors (struct image* i, Stack *s, struct position* p) {
	int count; /*keep track of the count of nondead neighbors of position p*/
	int queueCount; /*keep track of whether anything has been added to the stack*/
	count = 0; /*initialize count variables*/
	queueCount = 0;
	count = nonDeadNeighbors(i, p->row, p->col); /*count the nondead neighbors of that location*/
	if (count == 0) {
		i->image[p->row][p->col] = -1; /*in edge case where graph is a straight line --> at end will have no non-dead neighbors --> hit end of chain --> dead end, should mark as -1*/
	}
	else if (count == 1) { /*only one non-dead neighbor --> need to mark this current spot as dead and then figure out whether need to backtrack or not*/
		i->image[p->row][p->col] = -1; /*mark this node as dead-end*/
		findNeighbor(i, p); /*set p to the location of the single neighbor*/
		if (i->image[p->row][p->col] == 2) { /*single non-dead neighbor has been visited --> need to backtrack*/
			/*DO I NEED TO FIX THIS??? --> WHAT IF 0 nondead neighbors while backtracking - will this happen? what if make enqueued and after - 1? --> stop search early...*/
			while ((nonDeadNeighbors(i, p->row, p->col) == 1) && (i->image[p->row][p->col] == 2)) { /*while there is only one non-dead neighbor, and we've visited it --> set location to dead end and check the next non-dead neighbor, keep going till it splits somewhere or enqueued --> done backtracking*/
				i->image[p->row][p->col] = -1; /*if there is only one nondead number --> set location to dead end and check the next single neighbor*/
				findNeighbor(i, p); /*find next single neighbor to check if that's a deadend*/
			}
		} else { /*otherwise, single non-dead neighbor needs to be enqueued and marked as enqueued, we know neighbor is in position p*/
			i->image[p->row][p->col] = 3; /*mark this as enqueued - EVER GOING TO BE A TIME WHERE SINGLE NEIGHBOR --> NOT 2 AND NOT 1 --> ALREADY ENQUEUED --> DON'T WANT TO ENQUEUE TWICE...??? (Don't think so - this is something you just popped off the stack --> can't have enqueued neighbors, in enqueuing process now...*/
			/*now push location it onto stack and increment queueCount*/
			stackPush(s, p->row, p->col);
			queueCount++; 
		}
	} else { /*more than one non-dead neighbor -> mark current node as visited, and enqueue all non-dead, non-visited, non-enqueued neighbors*/
		i->image[p->row][p->col] = 2;
		/*set initial for loop counters to make sure nothing is negative*/
		int initialk;
		int initialj;

		if ((p->row - 1) >= 0) {
			initialj = p->row - 1;
		}else {
			initialj = 0;
		}

		if ((p->col - 1) >= 0) {
			initialk = p->col - 1;
		} else {
			initialk = 0;
		}
		/*iterate through all 8 blocks around location p*/
		for (int j = initialj; (j <= p->row + 1) && (j < i->height); j++) {
			for (int k = initialk; (k <= p->col + 1) && (k < i->width); k++) {
				
				if ((j == p->row) && (k == p->col)) { /*we're at p, not a neighbor, itself --> skip*/
					continue;
				}

				if (i->image[j][k] == 1) { /*not dead, not visited, not already enqueued, and not a wall*/
					i->image[j][k] = 3; /*mark it as enqueued and then push it onto stack*/
					stackPush(s, j, k);
					queueCount++;
				}
			
			}
		}
	}

	if (/*(queueCount == 0) &&*/(stackEmpty(s))) { /*need the queueCount check? -> if stackEmpty --> haven't enqueued anything...*/
		return 1; /*if stack is empty --> search is finished --> return 1*/
	}
	return 0; /*if you get to here --> enqNeighbors finished, no loop found, so 0 was returned, keep searching*/
}

/*this function takes in a pointer to an image struct and outputs the image header and content using printf and putchar*/
void printImage(const struct image* i) {
	printf("P5 %d %d 255\n", i->width, i->height); /*print header*/
	for (int j=0; j < i->height; j++) { /*iterate through image 2D array*/
		for (int k=0; k < i->width; k++) {
			if ((i->image[j][k] == -1) || (i->image[j][k] == 3)) { /*if the value is -1 --> deadend node, not part of loop --> needs to get printed as a 1*/
				putchar(1);
			} else {
				putchar(i->image[j][k]); /*otherwise print whatever is there, 2 in the loop, 1 if path or 0 if it's a wall*/
			}
		}
	}
}

/*this function takes a pointer to an image struct and frees all the memory the image struct is using*/ 
void freeImage(struct image* i) {
	for (int j=0; j < i->height; j++) { /*free each row in image*/
		free(i->image[j]);
	}
	free(i->image); /*free array of rows of image*/
	free(i); /*free image struct*/
}

int main (int argc, char** argv) {

	struct image* i; /*pointer for the image*/
	Stack s; /*pointer to top of the stack*/
	struct position *p; /*pointer for starting position in the image*/

	i = imageCreate(); /*read image into image struct*/
	s = 0; /*initialize the stack to a 0 pointer*/
	p = findStartPt(i); /*find starting position in image - location with 1, if no 1's found --> p = 0*/
	
	if (p!=0) {
		while (enqNeighbors(i, &s, p) == 0) { /*while enqNeighbors == 1 --> it's enqueued something or stack isn't empty --> keep going, enqueing necessary adjacent blocks to stack and modifying value of image in location p*/
			free(p); /*free the position struct we just finished enqueueing and adjusting values for*/
			p = stackPop(&s); /*now set p to the next item on the stack*/
		}
		free(p); /*free last p used, no longer looping through, search finished*/
	}
		
	printImage(i); /*output image, loop will now have 2's in it*/
	freeImage(i); /*free the memory the image struct was using*/

	return 0;
}