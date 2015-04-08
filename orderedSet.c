#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "orderedSet.h"

#define LEFT (0)
#define RIGHT (1)

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

/*Veena Advani
CPSC 223 - Hwk 8
Due: April 8 2015
Citation: AVL tree code modified from AVL tree section of Aspnes, Lecture Notes on Data Structures
 and Programming Techniques, available at http://www.cs.yale.edu/homes/aspnes/classes/223/notes.html, 
 downloaded April 3rd 2015. */

/*struct for nodes of AVL tree used to represent the ordered set*/
struct node {
	char* key; /*string in each tree node*/
	struct node* child[2]; /*pointers to the two children*/
	int height; /*height of that node*/
	size_t size; /*size of this subtree*/
};

/*struct to represent ordered set, which consists of a pointer to the root of the tree*/
struct orderedSet {
	struct node *set; /*pointer to root of set*/
};

/*This function creates an ordered set struct, mallocs the memory for it, and sets the root node to 0
to represent an empty tree.  Then returns the pointer to the new ordered set*/
struct orderedSet *orderedSetCreate(void) {
	struct orderedSet *s;
 	s = malloc(sizeof(struct orderedSet));
	assert(s); /*makes sure malloc worked*/
	s->set = TREE_EMPTY; 
	return s;
}

/*This function takes in a pointer to an ordered set and returns the size of the set*/
size_t orderedSetSize(const struct orderedSet *s) {
	if (s->set) { /*if the set consists of a non-empty tree --> return the size associated with the root node struct*/
		return s->set->size;
	}
	else { /*otherwise, the set has an empty tree --> just return a size of 0*/
		return 0;
	}
}

/*This function takes a pointer to a node and returns the height associated to that node*/
static int treeHeight(const struct node *root) {
	if (root==0) { /*if node a null pointer --> empty tree --> return empty tree height (-1)*/
		return TREE_EMPTY_HEIGHT;
	}
	else { /*otherwise, node exists --> return height in the root struct*/
		return root->height;
	}
}

/*This function takes in a pointer to a root node and returns the size of the tree*/
static size_t treeSize (const struct node *root) {
	if (root == 0){ /*root is a null pointer --> size is  0*/
		return 0;
	} else { /*otherwise, root exists --> return size stored in the struct*/
		return root->size;
	}
}

/*This function takes in a pointer to a root node and calculates and returns the size of the tree based on the size of its child subtrees*/
static size_t treeCalcSize (const struct node *root) {
	size_t size;

	if (root == 0) { /* root --> size is 0*/
		return 0;
	}
	else { /* root exists --> size = 1 (current node) + size of left and right subtrees*/
		size = 1 + treeSize(root->child[LEFT]) + treeSize(root->child[RIGHT]);
		return size;
	}
}

/*This function takes in a pointer to a root node and calculates and returns the height of the node based on the height of its children*/
static int treeCalcHeight (const struct node *root) {
	
	if (root == 0) { /*root doesn't exist --> empty tree height of -1*/
		return TREE_EMPTY_HEIGHT;
	} else { /*otherwise, figure out which subtree has a larger height and set height of current node to larger subree height + 1*/
		if (treeHeight(root->child[LEFT]) > treeHeight(root->child[RIGHT])) {
			return (treeHeight(root->child[LEFT]) + 1);
		} else {
			return (treeHeight(root->child[RIGHT]) + 1);
		}
	}
}

/*this function takes a pointer to a node and if the node is not empty, updates its height and size values based on the values of it's
children*/
static void treeFix (struct node *root) {
	if (root) {
		root->height = treeCalcHeight(root);
		root->size = treeCalcSize(root);
	}
}

/*this function takes in a pointer to a root pointer and a direction, and completes one tree rotation*/
static void treeRotate (struct node **root, int direction) {
	/*nodes used in rotation*/
	struct node *x;
	struct node *y;
	struct node *b;

	y = *root;
	assert (y);
	x = y->child[direction];
	assert(x);
	b = x->child[!direction];

	/*adjust the root to x*/
	*root = x;
	/*set x child in opposite direction to y, to start the rotation (y used to be the root)*/
	x->child[!direction] = y;
	/*set child[direction] of y to complete the rotation*/
	y->child[direction] = b;

/*Now that the rotation has been finished, update the height and size information for the y node first, and then the x  node, because y is a child
of x */
	treeFix(y); 
	treeFix(x);

}

/*This function takes in a pointer to a root pointer and rebalances the tree*/
static void treeRebalance(struct node **root) {
	int tallerKid; /*int to store taller child index*/

    if(*root) {
        for(tallerKid = 0; tallerKid < 2; tallerKid++) { /*iterate through both children*/
            if(treeHeight((*root)->child[tallerKid]) >= treeHeight((*root)->child[!tallerKid]) + 2) { /*if differences in height are 2 or larger --> rebalance*/

                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(treeHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= treeHeight((*root)->child[tallerKid]) - 1) {
                    /* zig zag case */
                    treeRotate(&(*root)->child[tallerKid], !tallerKid);
                }

                /* fall through to zig zig case */
                treeRotate(root, tallerKid);

                /* don't bother with other kid */
                break;
            }
        }
    }
}


/* free all the memory being used by the tree, and set the root to TREE_EMPTY */
static void treeDestroy(struct node **root) {
	if (*root) { /*if the root exists*/
		treeDestroy(&(*root)->child[LEFT]); /*recursively call on left*/
		treeDestroy(&(*root)->child[RIGHT]); /*recursively call on right*/
		free((*root)->key); /*free malloced mem for string in each node*/
		free(*root); /*free memory for each node*/
		*root = TREE_EMPTY; /*set root to empty*/
	}
}

/*this function takes a pointer to an ordered set and frees all the memory of the tree that the ordered set has a pointer to
and the memory the orderedSet struct uses itself*/
void orderedSetDestroy (struct orderedSet *s) {
	treeDestroy(&(s->set)); /*free tree in orderedSet struct*/
	free(s); /*freeSet*/
}

/* insert an element into a tree pointed to by the root pointer, if the element
doesn't already exist there*/
static void treeInsert(struct node **root, char* newElement) {
	struct node *e; /*node we're inserting*/
	int equality; /*integer to store the results of strcmp*/

	if ((*root) == TREE_EMPTY) { /*if nothing in the root, just insert newElement there*/
		e = malloc(sizeof (struct node)); /*malloc memory for new node*/
		assert(e); /*make sure malloc worked*/
		e->key = newElement; /*set key of node to string*/
		/*set children to null pointers*/
		e->child[LEFT] = 0;
		e->child[RIGHT] = 0;
		/*update root to new element we just added*/
		*root = e;
	}
	else { /*if tree wasn't empty --> need to find where in tree e needs to be inserted*/
		equality = strcmp((*root)->key, newElement); /*store results of strcmp in equality*/
		if (equality > 0) { /*this means the root > newElem --> need to insert in left subtree*/
			treeInsert(&(*root)->child[LEFT], newElement);
		}
		if (equality < 0) { /*root < newElem --> right subtree*/
			treeInsert(&(*root)->child[RIGHT], newElement);
		}
		if (equality == 0) { /*newElem already exists in tree --> need to free memory we're using for this string, don't insert it in tree*/
			free(newElement);
		}
	}
		
	/*fix data and rebalance on way out*/
	treeFix(*root);
	treeRebalance(root);
}

/*function that takes a string and returns a pointer to a newly malloced copy of that string*/
static char* myStrdup (const char* string) {
	char* string2; /*new string*/

	string2 = malloc(sizeof(char) * (strlen(string) + 1)); /*malloc appropriate amount of space for new string*/

	assert(string2); /*make sure malloc worked*/

	if (string2 != 0) {
		strcpy(string2, string); /*copy input string to new string*/
	}
	return string2; /*returns new copied string or 0 if malloc failed*/
}

/*This function takes a pointer to an ordered set and a new string, and inserts the string into the tree pointed to in the ordered set*/
void orderedSetInsert(struct orderedSet *s, const char *newElem) {
	char* insertElem; /*variable to store input string*/
	insertElem = myStrdup(newElem); /*malloc space for input string*/
	treeInsert(&(s->set), insertElem); /*insert new string into tree pointed to in orderedSet s*/
}

/*this function deletes the minimum string in a tree and returns its value.
Don't call on empty tree*/
static char* treeDeleteMin(struct node **root) {
	struct node* toFree; /*pointer to node we need to delete/free*/
	char* retString; /*string to return*/
	
	assert(*root); /*make sure tree isn't empty*/
		
	if ((*root)->child[LEFT]) { /*if there's a left child --> recurse again, not at minimum yet*/
		retString = treeDeleteMin(&(*root)->child[LEFT]);
	} else {/*if no left child --> reached minimum --> time to delete it*/
		retString = (*root)->key; /*store return string value*/
		toFree = *root; /*set toFree pointer to root node (node we want to delete)*/
		*root = (*root)->child[RIGHT]; /*since removing this root, if it has a right child --> have root point to right child, otherwise, if no right child --> this node will just be set to 0*/
		free(toFree); /*free memory for the node we're deleting*/
	}
	/*now that we've removed something --> need to fix data
	and rebalance*/
	treeFix(*root);
	treeRebalance(root);

	return retString;

}

/*This function takes in a pointer to a pointer to the root of a tree
and a target string and deletes the target string from the tree
if it exists there*/
static void treeDelete (struct node **root, char* target) {
	struct node *toFree; /*pointer to the node we're deleting/freeing*/
	int equality; /*integer to store results of strcmp*/
	if (*root) { /*if root exists*/
		if((equality = strcmp((*root)->key, target)) == 0) { /*if the root and target are same*/
			if ((*root)->child[RIGHT]) {
				free((*root)->key); /*free key currently there, so we can set key to min of right child*/
				(*root)->key = treeDeleteMin(&(*root)->child[RIGHT]);
			} else { /*if no right child --> just make left child the new root*/
				toFree = *root; /*set pointer to free node*/
				*root = toFree->child[LEFT]; /*set new root to left child*/
				free(toFree->key); /*free string in node we're deleting*/
				free(toFree); /*free old root node*/
			}
		} else if (equality > 0) { /*root key > target --> look left*/
			treeDelete(&(*root)->child[LEFT], target);
		} else { /*root key < target --> look right*/
			treeDelete(&(*root)->child[RIGHT], target);
		}

		/*fix height and size data in tree, then rebalance it after the deletion*/
		treeFix(*root);
		treeRebalance(root);
	}
}

/*this function takes a pointer to an ordered set and string and 
deletes it from the set if it exists in the set, otherwise it does
nothing*/
void orderedSetDelete(struct orderedSet *s, const char *elem) {
	treeDelete(&(s->set), elem); /*deletes element from tree that orderedset struct has a pointer to*/
}

/*this function takes a pointer to an ordered set, a pointer to a root node, a function pointer and an argument pointer, and inserts
the key of the root node into the ordered set if the predicate called on arg and root->key is not zero*/
static void operateFilter(struct orderedSet *s, struct node *root, int (*predicate)(void *arg, const char *c), void *arg) {
	if ((predicate(arg, root->key)) != 0) { /*make sure results of predicate function are not zero*/
		orderedSetInsert(s, root->key); /*insert string into orderedSet*/
	}
}

/*this function takes a pointer to an orderedSet, a pointer to a root, a function pointer and an argument pointer, and calls
operateFilter on every element in the tree that root points to, in increasing order*/
static void setFilter(struct orderedSet *s2, struct node *root, int (*predicate)(void *arg, const char* c), void *arg) {

	if (root != 0) { /*make sure root exists*/
		if(root->child[LEFT]) { /*if left child exists --> recursively call on it*/
			setFilter(s2, root->child[LEFT], predicate, arg);
		}
		operateFilter(s2, root, predicate, arg); /*call operate filter on current node*/
		if(root->child[RIGHT]) { /*if right child exists --> recursively call on it*/
			setFilter(s2, root->child[RIGHT], predicate, arg);
		}
	}
	
}

/*this function takes a pointer to an ordered set, a function pointer and an argument pointer, 
and returns a pointer to a new orderedSet, which contains all the strings in the initial orderedSet, 
that when predicate was called on them, !=0*/
struct orderedSet *orderedSetFilter(const struct orderedSet *s, int (*predicate)(void *arg, const char* c), void *arg) {
	struct orderedSet *s2; /*pointer to new ordered set*/
	s2 = orderedSetCreate(); /*create the struct/malloc memory for it*/
	setFilter(s2, s->set, predicate, arg); /*call predicate on every element in s in increasing order, and insert the key into the new ordered set if results !=0*/

	return s2; /*return pointer to new ordered set*/
}