CC=mpic++
CE=mpirun
CFLAGS=-g -Wall -Wextra

all: bfs

bfs: bfs.cpp
	$(CC) bfs.cpp $(CFLAGS) -o bfs

run: bfs
	$(CE) -np 8 bfs 

.PHONY: clean

clean:
	rm -rf bfs
