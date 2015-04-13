
typedef struct stackElem *Stack;

void stackPush (Stack *s, struct position *p);

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
