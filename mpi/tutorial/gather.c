#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
	MPI_Gather(
		void* send_data,
		int send_count,
		MPI_Datatype send_datatype,
		void* recv_data,
		int recv_count,
		MPI_Datatype recv_datatype,
		int root,
		MPI_Comm communicator)
*/

int main (int argc, char **argv)
{
	int world_size;
	int world_rank;
	int i, my_data;
	int *results;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	my_data = world_rank;

	if (world_rank == 0) {
		results = (int*)malloc(world_size * sizeof(int));
		assert(results != NULL);
	}
	
	MPI_Gather(&my_data, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (world_rank == 0) {
		for (i=0; i<world_size; i++)
			printf("data %i received\n", results[i]);
	}

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}
