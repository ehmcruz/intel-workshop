#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
	MPI_Scatter(
		void* send_data,
		int send_count,
		MPI_Datatype send_datatype,
		void* recv_data,
		int recv_count,
		MPI_Datatype recv_datatype,
		int root,
		MPI_Comm communicator)
*/

#define el_per_proc 2

int main (int argc, char **argv)
{
	int world_size;
	int world_rank;
	int i;
	int *vector, *subvector;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		vector = (int*)malloc(el_per_proc * world_size * sizeof(int));
		assert(vector != NULL);
		
		for (i=0; i<el_per_proc*world_size; i++)
			vector[i] = i;
	}

	subvector = (int*)malloc(el_per_proc * sizeof(int));
	assert(subvector != NULL);

	MPI_Scatter(vector, el_per_proc, MPI_INT, subvector, el_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (i=0; i<el_per_proc; i++)
		printf("rank %i el %i\n", world_rank, subvector[i]);

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}
