
CC=g++
CFLAGS=-g -Wall -Wextra
#PFLAGS=

all: bfs run

bfs: bfs.cpp
	$(CC) bfs.cpp  $(CFLAGS) -o bfs 

run:
	./bfs

.PHONY: clean

clean:
	rm -rf bfs
