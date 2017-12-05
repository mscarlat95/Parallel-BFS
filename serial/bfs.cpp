#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

std::vector<std::vector<int> > graph;
std::queue<int> queue;
int *marked;

void DIE (bool condition, int line, const char *message) {
	if (condition) {
		fprintf(stderr, "[ERROR] Line %d:\t%s\n", line, message);
		exit (-1);
	}
}

void display () {
	for (unsigned int i = 0; i < graph.size(); ++i) {
		if (graph[i].size() != 0) {
			printf ("%d : ", i);
			for (unsigned int j = 0; j < graph[i].size(); ++j) {
				printf("%d ", graph[i][j]);
			}
			printf("\n");
		}
	}
}

void init () {
	int res, nodesNum, node1, node2;

	FILE *fin = fopen ("bfs.in", "r");
	DIE (fin == NULL, __LINE__, "Unable to open input file");

	fscanf (fin, "%d", &nodesNum);
	DIE (nodesNum <= 0, __LINE__, "Invalid number of nodes");
	graph.resize (nodesNum + 1);
	marked = new int [nodesNum + 1]();
	
	while (fscanf (fin, "%d%d", &node1, &node2) != EOF) {
		bool checkNode = node1 < 0 || node2 < 0 || node1 > nodesNum || node2 > nodesNum;
		
		DIE (checkNode, __LINE__, "Invalid nodes");		
		graph[node1].push_back (node2);
		graph[node2].push_back (node1);
	}

	res = fclose (fin);
	DIE (res < 0, __LINE__, "Unable to close input file");
}

void bfs (int startNode) {
	queue.push (startNode);

	while (! queue.empty()) {
		int node = queue.front();
		queue.pop();

		marked[node] = 2;
		printf("%d ", node);

		for (unsigned int i = 0; i < graph[node].size(); ++i) {
			if ( marked[graph[node][i]] == 0 ) {
				queue.push (graph[node][i]);
				marked[graph[node][i]] = 1;
			}
		}
	}
}


int main(void)
{

	init();

	printf("Created the following graph:\n");
	display();

	printf("\nPerformed BFS: ");
	bfs (1);
	printf("\n");

	return 0;
}