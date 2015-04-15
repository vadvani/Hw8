/*to keep track of the top of the stack*/
typedef struct stackElem *Stack;

/*struct to keep track of the coordinate location in an array*/
struct position {
	int row; 
	int col;
};

/*structs used to make up the stack in depth first search.  each stack element contains a pointer to a
position struct and a pointer to the next element in the stack*/
struct stackElem {
	struct position *p;
	struct stackElem* next;
};

/*struct to keep track of the width, height and 2D array for the image/graph*/
struct image{
	int width;
	int height;
	int** image;
};

/*this function takes a pointer to the top of the stack, a row and column number, and pushes a new
element onto the stack with a position struct that contains those two coordinates*/
void stackPush (Stack *s, int row, int col);

/*this function takes in a pointer to the top of the stack and returns 1 if the stack is empty, 0 if it's not*/
int stackEmpty (const Stack *s);

/*this function takes in a pointer to the top of a stack and returns a pointer to the position struct of that top element, and removes the top
element from the stack, should not be called on an empty stack*/
struct position* stackPop (Stack *s);

/*this function reads the input from stdin and returns a pointer to a malloced image struct that contains
the height and width of the image, as well as a 2D array that contains the values of the image/graph*/
struct image* imageCreate(void);

/*this function takes in a pointer to an image struct and returns a malloced position struct with the location of the first one (path block), at which
we can start the program and depth first search*/
struct position* findStartPt (const struct image* i);

/*In the image, deadend neighbors = -1. visited = 2; enqueued = 3; untouched = 0 or 1
This function takes in a pointer to an image struct, and a row and column location and counts all the
non-deadend neighbors of the input location*/
int nonDeadNeighbors(const struct image* i, const int row, const int col);

/*this function takes in a MALLOCED location of a deadend node, finds the single nondead neighbor, and modifies the position struct to contain the neighbor
coordinates of the deadend node*/
void findNeighbor(const struct image* i, struct position* p);

/*this function takes in a pointer to an image struct, a pointer to the top of a stack, and a position struct p, and sets the value of the image
in location p to the appropriate value (2 for visited, -1 for visited but dead end), and enques all the unvisited, unenqueued neighbors
of that location onto the stack, and returns a number, 0 if everything went fine, 1 if the search is finsihed (the stack is empty and nothing was added this round)*/
int enqNeighbors (struct image* i, Stack *s, struct position* p);

/*this function takes in a pointer to an image struct and outputs the image header and content using printf and putchar*/
void printImage(const struct image* i);

/*this function takes a pointer to an image struct and frees all the memory the image struct is using*/
void freeImage(struct image* i);
