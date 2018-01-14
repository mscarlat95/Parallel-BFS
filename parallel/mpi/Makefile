CC=mpic++
CE=mpirun
CFLAGS=-g -Wall -Wextra

all: bfs

bfs: bfs.cpp
	$(CC) bfs.cpp $(CFLAGS) -o bfs

run: bfs
	$(CE) -np ${ARGS} bfs 

.PHONY: clean

clean:
	rm -rf bfs
