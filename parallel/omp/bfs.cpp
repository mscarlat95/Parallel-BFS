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
			fflush (stdout);

			#pragma omp parallel for schedule (dynamic)
			for (j = 0; j < graph[i].size(); ++j) {
				printf("%d ", graph[i][j]);
				fflush (stdout);
			}
			printf("\n");
		}
	}
}

int main(int argc, char const *argv[])
{
	int *marked = NULL;
	int res, nodesNum, node1, node2, node, root;
	unsigned int i, threads_num;

	std::vector <std::vector <int> > graph;
	std::queue <int> queue;

	switch (argc) {
		case 1: threads_num = NUM_THREADS; break;
		case 2:	threads_num = atoi (argv[1]); break;
		default:	DIE (1, __LINE__, "Invalid number of arguments");
	}

	/* Setup number of threads */
	omp_set_num_threads (threads_num);

	/* Setup omp lock */
	omp_lock_t lck;
	omp_init_lock (&lck);

	/* Create graph using the given input file */
	FILE *fin = fopen ("bfs.in", "r");
	DIE (fin == NULL, __LINE__, "Unable to open input file");

	fscanf (fin, "%d", &nodesNum);
	DIE (nodesNum <= 0, __LINE__, "Invalid number of nodes");
	graph.resize (nodesNum + 1);
	marked = (int *) calloc (nodesNum + 1,  sizeof (int));
	
	while (fscanf (fin, "%d%d", &node1, &node2) != EOF) {
		res = node1 < 0 || node2 < 0 || node1 > nodesNum || node2 > nodesNum;		
		DIE (res == 1, __LINE__, "Invalid nodes");

		/* Add adjacence <node1, node2> */
		#pragma omp parallel sections 
		{
			#pragma omp section
			{
				graph[node1].push_back (node2);
			}

			#pragma omp section
			{
				graph[node2].push_back (node1);
			}
		}
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
	
		#pragma omp parallel
		{
			#pragma omp single
			{
				node = queue.front();
				queue.pop();

				marked[node] = 2;
				printf("%d ", node);
			}

			#pragma omp barrier

			#pragma omp parallel for shared (lck) schedule (dynamic)
			for (i = 0; i < graph[node].size(); ++i) {
				omp_set_lock (&lck);
				if ( marked[graph[node][i]] == 0 ) {
					queue.push (graph[node][i]);
					marked[graph[node][i]] = 1;
				}
				omp_unset_lock(&lck);
			}
		}
	}
	printf("\n");

	// /* Cleanup */
	omp_destroy_lock (&lck);
	free (marked);

	return 0;
}
