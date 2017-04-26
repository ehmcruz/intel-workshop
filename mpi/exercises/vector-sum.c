#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define el_per_proc 10

int main (int argc, char **argv)
{
	int world_size;
	int world_rank;
	int i;
	int *vector;
	int sum_total;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		vector = (int*)malloc(el_per_proc * world_size * sizeof(int));
		assert(vector != NULL);
		
		for (i=0; i<el_per_proc*world_size; i++) {
			vector[i] = i;
		}
	}

	if (world_rank == 0) {
		sum_total = 0;
		for (i=0; i<el_per_proc*world_size; i++)
			sum_total += vector[i];
		printf("sum_total=%i\n", sum_total);
	}

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}
