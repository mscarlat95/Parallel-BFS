
CC=g++
CFLAGS=-g -Wall -Wextra
PFLAGS=-lpthread

all: bfs

bfs: bfs.cpp
	$(CC) bfs.cpp $(CFLAGS) $(PFLAGS) -o bfs 

run:
	./bfs 8

.PHONY: clean

clean:
	rm -rf bfs
