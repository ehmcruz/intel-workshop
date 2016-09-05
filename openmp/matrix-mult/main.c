#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/matrix.h"

static int **first;
static int **second;
static int **multiply;

void* matrix_malloc (unsigned nrows, unsigned ncols, unsigned type_size)
{
	void **p;
	unsigned i;
	
	p = (void**)calloc(nrows, sizeof(void*));
	assert(p != NULL);
	
	p[0] = (void*)calloc(nrows*ncols, type_size);
	assert(p[0] != NULL);
	for (i=1; i<nrows; i++)
		p[i] = p[0] + i*ncols*type_size;
	
	return (void*)p;
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, j, n, is_ok;
	struct timeval time_start, time_end;
	double elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of rows x cols: %i\n", n);
	
	first = matrix_malloc(n, n, sizeof(int));
	second = matrix_malloc(n, n, sizeof(int));
	multiply = matrix_malloc(n, n, sizeof(int));
	
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
	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			first[i][j] = 1;
			second[i][j] = 1;
		}
	}
	
	gettimeofday(&time_start, NULL);
	matrix_mult(first, second, multiply, n, n, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

/*	for (i=0; i<n; i++) {*/
/*		for (j=0; j<n; j++) {*/
/*			printf("%i ", multiply[i][j]);*/
/*		}*/
/*		printf("\n");*/
/*	}*/

	is_ok = 1;	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			if (multiply[i][j] != n) {
/*				printf("i %i j %i multiply[i][j] %i\n", i, j, multiply[i][j]);*/
				is_ok = 0;
				i = n;
				j = n;
			}
		}
	}
	
	printf("multiply values are %s\ntime: %.3f seconds\n", awnser[is_ok], elapsed);
	
	return 0;
}
