#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/matrix.h"

static myfloat *first;
static myfloat *second;
static myfloat *multiply;

__attribute__((target(mic))) struct timeval xeonphi_time_start, xeonphi_time_end;

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, j, n, is_ok;
	struct timeval time_start, time_end;
	double elapsed, xeonphi_elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	
	n = ((n%8) == 0) ? n : n + (8 - n%8);
	
	printf("number of rows x cols: %i\n", n);
	
	first = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(first != NULL);
	
	second = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(second != NULL);
	
	multiply = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(multiply != NULL);
		
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
			first[i*n+j] = 1;
			second[i*n+j] = 1;
		}
	}
	
	gettimeofday(&time_start, NULL);
	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));
	matrix_mult(first, second, multiply, n, n, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;

/*	for (i=0; i<n; i++) {*/
/*		for (j=0; j<n; j++) {*/
/*			printf("%i ", multiply[i][j]);*/
/*		}*/
/*		printf("\n");*/
/*	}*/

	is_ok = 1;	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			if ((double)multiply[i*n+j] != (double)n) {
/*				printf("i %i j %i multiply[i][j] %i\n", i, j, multiply[i][j]);*/
				is_ok = 0;
				i = n;
				j = n;
			}
		}
	}
	
	printf("multiply values are %s\ntotal time: %.3f seconds\nxeon phi time: %.3f seconds\n", awnser[is_ok], elapsed, xeonphi_elapsed);
	
	return 0;
}
