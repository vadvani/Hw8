#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LEFT (0)
#define RIGHT(1)

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

struct node {
	char* key;
	struct node* child[2];
	int height;
	size_t size;
};

/*CHECK POINTER STUFF WITH ORDERED SET, NEED DOUBLE OR SINGLE POINTER???*/
struct orderedSet {
	struct *node set; /*pointer to root of set*/
};

/*CHECK POINTERS HERE*/
struct orderedSet *orderedSetCreate(void) {
	struct orderedSet* s;
 	s = malloc(sizeof(struct orderedSet));
	assert(s);
	orderedSet->set = TREE_EMPTY; /*THIS RIGHT???*/
	return s;
}

/*CHECK POINTERS HERE*/
size_t orderedSetSize(const struct orderedSet *s) {
	return s->set->size;
}

static int treeHeight(const struct node *root) {
	if (root==0) {
		return TREE_EMPTY_HEIGHT;
	}
	else {
		return root->height;
	}
}
static size_t treeSize (const struct node *root) {
	if (root == 0){
		return 0;
	} else {
		return root->size;
	}
}

static size_t treeCalcSize (const struct node *root) {
	size_t size;

	if (root == 0) {
		return 0;
	}
	else {
		size = 1 + treeSize(root->child[LEFT]) + treeSize(root->child[RIGHT]);
		return size;
	}
}

static int treeCalcHeight (const struct node *root) {
	int childHeight;
	int maxChild;

	if (root == 0) {
		return TREE_EMPTY_HEIGHT;
	} else {
		if (treeHeight(root->child[LEFT]) > treeHeight(root->child[RIGHT])) {
			return (treeHeight(root->child[LEFT]) + 1);
		} else {
			return (treeHeight(root->child[RIGHT]) + 1);
		}
	}
}

static void treeFix (struct node *root) {
	if (root) {
		root->height = treeCalcHeight(root);
		root-size = treeCalcSize(root);
	}
}

static void treeRotate (struct node **root, int direction) {
	struct node *x;
	struct node *y;
	struct node *b;

	y = *root;
	assert (y);
	x = y->child[direction];
	assert(x);
	b = x->child[!direction];

	*root = x;
	x->child[!direction] = y;
	y->child[direction] = b;

	treeFix(y); 
	treeFix(x); /*HOW COME WE DON'T HAVE TO WORRY ABOUT STUFF OF DIRECTION BRANCH BELOW X, THAT GETS MOVED UP TOO RIGHT??? - in notes = A*/

}

/*DO NOT UNDERSTAND THE IF INEQUALITIES FOR ZIG ZAG - ASK SOMEONE???*/
static void treeRebalance(struct node **root) {
	int tallerKid;

    if(*root) {
        for(tallerKid = 0; tallerKid < 2; tallerKid++) {
            if(treeHeight((*root)->child[tallerKid]) >= treeHeight((*root)->child[!tallerKid]) + 2) {

                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(treeHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= treeHeight((*root)->child[tallerKid] - 1)) {
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

/*This function takes in a pointer to the root of a tree, and a target string
and returns 0 if the string is not contained in the tree and 1 if it is*/
/*this equality assignment okay???*/
static int treeContains (const struct node *root, char* target) {
	int equality;
	while ((root != 0) && ((equality = strcmp(root->key, target)) != 0)) {
		if (equality>0) { /*root > target --> go left*/
			root = root->child[LEFT];
		} else {
			root = root->child[RIGHT];
		}
	}

	return (root != 0);
}

/* free all elements of a tree, replacing it with TREE_EMPTY */
static void treeDestroy(struct node **root) {
	if (*root) {
		treeDestroy(&(*root)->child[LEFT]);
		treeDestroy(&(*root)->child[RIGHT]);
		free((*root)->key); /*free malloced mem for string in each node*/
		free(*root); /*free memory for each node*/
		*root = TREE_EMPTY;
	}
}

void orderedSetDestory (struct orderedSet *s) {
	treeDestroy(&(s->set)); /*free tree in orderedSet struct*/
	free(s); /*freeSet*/
}

/* insert an element into a tree pointed to by root, if the element
doesn't already exist there*/
static void treeInsert(struct node **root, char* newElement) {
	struct node *e;
	int equality;

	if ((*root) == TREE_EMPTY) { /*if nothing in the root, just insert newElement there*/
		e = malloc(size of (struct node));
		assert(e);
		e->key = newElement;
		e->child[LEFT] = 0;
		e->child[RIGHT] = 0;
		*root = e;
	}
	else {
		equality = strcmp((*root)->key, newElement);
		if (equality > 0) { /*this means the root > newElem --> need to insert in left subtree*/
			treeInsert(&(*root)->child[LEFT], newElement);
		}
		if (equality < 0) { /*root < newElem --> right subtree*/
			treeInsert(&(*root)->child[RIGHT], newElement);
		}
	}
		
	/*fix data and rebalance on way out*/
	treeFix(*root); /*MAKE SURE THIS LINE WORKS - otherwise use treeFix*/
	treeRebalance(root);
}

static char* myStrdup (const char* string) {
	char* string2;

	string2 = malloc(sizeof(char) * (strlen(string) + 1));

	assert(string2); /*NEED THIS???*/

	if (string2 != 0) {
		strcpy(string2, string); /*DOES THIS ADD NULL AT END?*/
	}
	return string2; /*returns new copied string or 0 if malloc failed*/
}

/*WILL HE EVER TRY TO PASS IN AN EMPTY STRING HERE???*/
void orderedSetInsert(struct orderedSet *s, const char *newElem) {
	char* insertElem;
	insertElem = myStrdup(newElem);
	treeInsert(&(s->set), insertElem);
}

/*this function deletes the minimum string in a tree and returns its value.
Don't call on empty tree*/
static char* treeDeleteMin(struct node **root) {
	struct node* toFree;
	char* retString;
	
	assert(*root); /*make sure tree isn't empty*/
		
	if ((*root)->child[LEFT]) {
		retString = treeDeleteMin(&(*root)->child[LEFT]);
	} else {
		retString = (*root)->key;
		toFree = *root;
		*root = *root->child[RIGHT]; /*since removing this root, if it has a right child --> have root point to right child*/
		free(toFree->key);
		free(toFree);
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
	struct node *toFree;
	int equality;
	if (*root) {
		if((equality = strcmp((*root)->key, target)) == 0) { /*if the root and target are same*/
			if ((*root)->child[RIGHT]) {
				(*root)->key = treeDeleteMin(&(*root)->child[RIGHT]);
			} else { /*need to delete root*/
				toFree = *root;
				*root = toFree->child[LEFT]; /*set new root to left child*/
				free(toFree->key);
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
	treeDelete(&(s->set), elem);
}

static void operateFilter(struct orderedSet *s, struct node *root, int (*predicate)(void *arg, const char *c), void *arg) {
	if ((predicate(arg, root->key)) != 0) {
		orderedSetInsert(s, root->key);
	}
}

static struct orderedSet *setFilter(struct node *root, int (*predicate)(void *arg, const char *c), void *arg) {
	struct orderedSet *s2;
	s2 = orderedSetCreate;

	if (root != 0) {
		if(root->child[LEFT]) {
			setFilter(root->child[LEFT], predicate(arg, c), arg);
		}
		operateFilter(s2, root, predicate(arg, c), arg);
		if(root->child[RIGHT]) {
			setFilter(root->child[RIGHT], predicate(arg, c), arg);
		}
	}

	return s2;
	
}

struct orderedSet *orderedSetFilter(const struct orderedSet *s, int (*predicate)(void *arg, const char *c), void *arg) {
	struct orderedSet *s2;
	s2 = setFilter(s->set, predicate(arg, c), arg);

	return s2;
}