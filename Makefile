CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: maze

maze: maze.o
	$(CC) $(CFLAGS) -o $@ $^

maze.o: maze.c maze.h

clean: $(RM) maze *.o
