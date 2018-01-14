#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <pthread.h>

#define NUM_THREADS 4
#define MAX_LEN	100
#define MAX_QUEUE_SIZE	10

typedef struct {
	int id;
	int len;
	int *array;
} thread_info;

pthread_mutex_t mutex;
int marked[MAX_LEN], adj_mat[MAX_LEN], result[MAX_LEN], result_length;

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

void *get_neighbors (void *args) {

	int i = 0, q_size = 0;
	int proc_queue[MAX_QUEUE_SIZE];	
	thread_info *th_info = (thread_info *) args;

	/* Initialize process queue */
	for (i = 0; i < MAX_QUEUE_SIZE; ++i) {
		proc_queue[i] = -1; /*unset*/
	}

	/* Update process queue */
	for (i = 0; i < th_info->len; ++i) {
		if (th_info->array[i] == 1) {
			proc_queue[q_size ++] = i;
		}
	}

	printf("Thread %d has the following neighbors: ", th_info->id);
	display_proc_neighbors (q_size, proc_queue);

	/* Update global result */
	pthread_mutex_lock(&mutex);	
	for (i = 0; i < q_size; ++i) {
		result[result_length ++] = proc_queue[i];
	}
	pthread_mutex_unlock(&mutex);

	return NULL;
}


int main(int argc, char const *argv[])
{
	FILE *fin = NULL;
	int i, j, res, nodesNum, node1, node2, root;
	int threads_num = NUM_THREADS;

	switch (argc) {
		case 1: threads_num = NUM_THREADS; break;
		case 2:	threads_num = atoi (argv[1]); break;
		default:	DIE (1, __LINE__, "Invalid number of arguments");
	}

	pthread_t threads[threads_num];
	thread_info th_info[threads_num];

	/**********************************************************************/
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

	/**********************************************************************/

	/* init mutex */
    res = pthread_mutex_init(&mutex, NULL);
    DIE (res != 0, __LINE__, "pthread_mutex_init");

    /* Send one row to each thread*/
	for (i = 0; i < threads_num; ++i) {

		th_info[i].id = i;
		th_info[i].len = nodesNum;
		th_info[i].array = (int *) malloc (th_info[i].len * sizeof(int));

		for (j = 0; j < th_info[i].len; ++j) {
			th_info[i].array[j] = adj_mat[i * nodesNum + j];
		}

		res = pthread_create (&threads[i],	/* thread */ 
							NULL, 			/* attributes */
							&get_neighbors,	/* assignated function */
							& th_info[i]);	/* function arguments */
		DIE (res != 0, __LINE__, "pthread_create");
		
	}

	/* wait for result */
	for (int i = 0; i < NUM_THREADS; ++i) {
		res = pthread_join (threads[i], NULL);
		DIE (res != 0, __LINE__, "pthread_join");
	}
	/**********************************************************************/


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
	

	/* destroy mutex */
    res = pthread_mutex_destroy(&mutex);
    DIE (res != 0, __LINE__, "pthread_mutex_destroy");

	return 0;
}