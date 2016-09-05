#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/sum.h"

static int *vector;

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, sumv, n;
	struct timeval time_start, time_end;
	double elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of elements: %i\n", n);
	
	vector = calloc(n, sizeof(int));
	assert(vector != NULL);
	
	omp_set_num_threads(2);
	
	#pragma omp parallel
	{
		#pragma omp single
		{
			printf("there are %u threads %u\n", omp_get_num_threads(), omp_get_thread_num());
		}

		#pragma omp single
		{
			printf("ola %u\n", omp_get_thread_num());
		}

		
//		#pragma omp barrier
		
		printf("hello from thread %u\n", omp_get_thread_num());
		
	//	#pragma omp barrier
		
		#pragma omp master
		{
			printf("the master thread is %u\n", omp_get_thread_num());
		}
	}
	
	for (i=0; i<n; i++)
		vector[i] = 1;
	
	gettimeofday(&time_start, NULL);
	sumv = sum(vector, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	printf("sum value is %u, which is %s\ntime: %.3f seconds\n", sumv, awnser[sumv == n], elapsed);
	
	return 0;
}
