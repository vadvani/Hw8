CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testOrderedSet

testOrderedSet: testOrderedSet.o orderedSet.o
	$(CC) $(CFLAGS) -o $@ $^

testOrderedSet.o: testOrderedSet.c orderedSet.h

orderedSet.o: orderedSet.c orderedSet.h

clean: $(RM) testOrderedSet *.o
