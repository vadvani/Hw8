
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

void stackPush (Stack *s, int row, int col);

int stackEmpty (const Stack *s);

struct position* stackPop (Stack *s);

struct image* imageCreate(void);

int countNeighbors(const struct image* i, const int row, const int col);

struct position* findStartPt (struct image* i);

int nonDeadNeighbors(const struct image* i, const int row, const int col);

int nonDeadNeighbors(const struct image* i, const int row, const int col);

void findNeighbor(struct image* i, struct position* p);

int enqNeighbors (struct image* i, Stack *s, struct position* p);

void printImage(const struct image* i);

void freeImage(struct image* i);
