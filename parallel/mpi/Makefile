
CC=g++
CFLAGS=-g -Wall -Wextra
PFLAGS=-fopenmp

all: bfs run

bfs: bfs.cpp
	$(CC) bfs.cpp  $(CFLAGS) $(PFLAGS) -o bfs 

run:
	./bfs

.PHONY: clean

clean:
	rm -rf bfs
