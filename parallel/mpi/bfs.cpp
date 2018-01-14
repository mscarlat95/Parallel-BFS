#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include "mpi.h"

#define NUM_THREADS 4
#define MAX_LEN	100
#define MAX_QUEUE_SIZE	10

int marked[MAX_LEN], adj_mat[MAX_LEN], proc_mat[MAX_LEN], proc_queue[MAX_QUEUE_SIZE], result[MAX_LEN];

void DIE (bool condition, int line, const char *message) {
	if (condition) {
		fprintf(stderr, "[ERROR] Line %d:\t%s\n", line, message);
		exit (-1);
	}
}

void display_all(int n) {
	for (int i = 0; i < n; ++i) {
		printf("%d: ", i);
		for (int j = 0; j < n; ++j) {
			if (adj_mat[i * n + j] == 1) {
				printf("%d ", j);
			}
		}
		printf("\n");
	}
	printf("\n");
}


void display_proc_neighbors(int q_size, int *proc_queue) {
	for (int i = 0; i < q_size; ++i) {
		printf("%d ", proc_queue[i]);
	}
	printf("\n\n");
}


int main(int argc, char const *argv[])
{
	FILE *fin = NULL;
	int i, j, res, nodesNum, node1, node2, root;
	int rank = -1, nTasks = -1;

	/* Initialize MPI execution environment */
	MPI_Init (&argc, (char ***) &argv);
	/* Determines the rank/ID of the current task */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	/* Gives the number of tasks */
	MPI_Comm_size (MPI_COMM_WORLD, &nTasks);


	/* Assign one task to each thread */

	/* Read input file using Thread 0 */
	if (rank == 0) {
		fin = fopen ("bfs.in", "r");
		DIE (fin == NULL, __LINE__, "Unable to open input file");

		/* Number of nodes */
		fscanf (fin, "%d", &nodesNum);
		DIE (nodesNum <= 0, __LINE__, "Invalid number of nodes");
		while (fscanf (fin, "%d%d", &node1, &node2) != EOF) {
			adj_mat[node1*nodesNum + node2] = 1;
			adj_mat[node2*nodesNum + node1] = 1;			
		}
		res = fclose (fin);
		DIE (res < 0, __LINE__, "Unable to close input file");
	
		/* Display edges */
		printf("Created the following graph:\n");
		display_all (nodesNum);

		/* setup BFS source node */
		root = 0;
	}

	/* Broadcast: number of nodes and source node */
	MPI_Bcast (&nodesNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast (&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* Scatter each row to each process */
	MPI_Scatter (adj_mat, nodesNum , MPI_INT, proc_mat, nodesNum, MPI_INT, 0, MPI_COMM_WORLD);

	/* Initialize process queue */
	for (i = 0; i < MAX_QUEUE_SIZE; ++i) {
		proc_queue[i] = -1; /*unset*/
	}

	/* Update process queue */
	int q_size = 0;

	for (i = 0; i < nodesNum; ++i) {
		if (proc_mat[i] == 1) {
			proc_queue[q_size ++] = i;
		}
	}
	printf("Process %d has the following neighbors: ", rank);
	display_proc_neighbors (q_size, proc_queue);


	/* Synchronize before gathering the neighbors */
	MPI_Barrier(MPI_COMM_WORLD);

	/* Send all info to Process 0 */
	MPI_Gather(proc_queue, MAX_QUEUE_SIZE, MPI_INT, result, MAX_QUEUE_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	/* Initialize frequency array (marked nodes) */
	for (i = 0; i < nodesNum; ++i) {
		marked[i] = 0;
	}

	/* Compute BFS traversal in process 0 */
	if (rank == 0) {
		printf("Perform BFS: %d ", root);
		marked[root] = 1;
	
		for (i = 0; i < MAX_QUEUE_SIZE * nodesNum; ++i) {

			int done = 1;
			for (j = 0; j < nodesNum && done; ++j) {
				if (!marked[j]) {
					done = 0;
				}
			}

			/* All nodes were visited */
			if (done) {
				break;	/* Finish */
			}

			if (result[i] != -1) { /* Neighbors available */
				/* Update marked array */
				if (!marked[result[i]]) {
					printf("%d ", result[i]);
					marked[result[i]] = 1; 
				}
			}
		}
		printf("\n");
	}
	

	/* Terminates MPI execution environment */
	MPI_Finalize();

	return 0;
}