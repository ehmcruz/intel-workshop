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
	int *vector, *subvector, *results;
	int sum_local, sum_total, verify;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		vector = (int*)malloc(el_per_proc * world_size * sizeof(int));
		assert(vector != NULL);
		
		verify = 0;
		for (i=0; i<el_per_proc*world_size; i++) {
			vector[i] = i;
			verify += i;
		}
	}

	subvector = (int*)malloc(el_per_proc * sizeof(int));
	assert(subvector != NULL);

	MPI_Scatter(vector, el_per_proc, MPI_INT, subvector, el_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	
	sum_local = 0;
	for (i=0; i<el_per_proc; i++)
		sum_local += subvector[i];

	printf("Sum of rank %i is %i\n", world_rank, sum_local);

	if (world_rank == 0) {
		results = (int*)malloc(world_size * sizeof(int));
		assert(results != NULL);
	}
	
	MPI_Gather(&sum_local, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (world_rank == 0) {
		sum_total = 0;
		for (i=0; i<world_size; i++)
			sum_total += results[i];
		printf("sum_total=%i verify=%i\n", sum_total, verify);
	}

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}
