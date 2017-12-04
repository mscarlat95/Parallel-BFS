#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <omp.h>

#define NUM_THREADS 4

void DIE (bool condition, int line, const char *message) {
	if (condition) {
		fprintf(stderr, "[ERROR] Line %d:\t%s\n", line, message);
		exit (-1);
	}
}

void display (std::vector <std::vector <int> > &graph) {
	unsigned int i, j;

	for (i = 0; i < graph.size(); ++i) {
		if (graph[i].size() != 0) {
			printf ("%d : ", i);
			for (j = 0; j < graph[i].size(); ++j) {
				printf("%d ", graph[i][j]);
			}
			printf("\n");
		}
	}
}

int main(int argc, char const *argv[])
{
	int *marked = NULL;
	int rank = -1, nTasks = -1;
	int res, nodesNum, node1, node2, node, root;
	unsigned int i, j, threads_num;

	std::vector <std::vector <int> > graph;
	std::queue <int> queue;


	/* Initialize MPI execution environment */
	MPI_Init (&argc, (char ***) &argv);

	/* Determines the rank/ID of the current task */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	/* Gives the number of tasks */
	MPI_Comm_size (MPI_COMM_WORLD, &nTasks);

	/* Setup number of threads */
	omp_set_num_threads (threads_num);

	/* Setup omp lock */
	omp_lock_t lck;
	omp_init_lock (&lck);

	/* Create graph using the given input file */
	FILE *fin = fopen ("../../bfs.in", "r");
	DIE (fin == NULL, __LINE__, "Unable to open input file");

	fscanf (fin, "%d", &nodesNum);
	DIE (nodesNum <= 0, __LINE__, "Invalid number of nodes");
	graph.resize (nodesNum + 1);
	marked = (int *) calloc (nodesNum + 1,  sizeof (int));
	
	while (fscanf (fin, "%d%d", &node1, &node2) != EOF) {
		res = node1 < 0 || node2 < 0 || node1 > nodesNum || node2 > nodesNum;		
		DIE (res == 1, __LINE__, "Invalid nodes");

		/* Add adjacence <node1, node2> */
		graph[node1].push_back (node2);
		graph[node2].push_back (node1);
	}

	res = fclose (fin);
	DIE (res < 0, __LINE__, "Unable to close input file");

	/* Display adjacencies */
	printf("Created the following graph:\n");
	display (graph);

	/* Perform Breadth First Search */
	printf("\nPerform BFS: ");
	root = 1;
	queue.push (root);
	while (! queue.empty()) {	
		node = queue.front();
		queue.pop();

		marked[node] = 2;
		printf("%d ", node);

		for (i = 0; i < graph[node].size(); ++i) {
			if ( marked[graph[node][i]] == 0 ) {
				queue.push (graph[node][i]);
				marked[graph[node][i]] = 1;
			}
		}
	}
	printf("\n");

	/* Cleanup */
	free (marked);

	return 0;
}