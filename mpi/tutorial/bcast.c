#include <mpi.h>
#include <stdio.h>

/*
	MPI_Bcast(
		void* data,
		int count,
		MPI_Datatype datatype,
		int root,
		MPI_Comm communicator)
*/

int main (int argc, char **argv)
{
	int world_size;
	int world_rank;
	int number;
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0)
		number = -1;
	
	MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Process %d received number %d from process 0\n", world_rank, number);

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}
