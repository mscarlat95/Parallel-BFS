CC=mpic++
CE=mpirun
CFLAGS=-g -Wall -Wextra
PFLAGS=-fopenmp

all: bfs

bfs: bfs.cpp
	$(CC) bfs.cpp $(CFLAGS) $(PFLAGS) -o bfs

run: bfs
	$(CE) -np ${ARGS} bfs

.PHONY: clean

clean:
	rm -rf bfs
