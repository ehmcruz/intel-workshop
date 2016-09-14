#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/sum.h"

static myfloat *vector;

__attribute__((target(mic))) struct timeval xeonphi_time_start, xeonphi_time_end;

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, n;
	myfloat sumv;
	struct timeval time_start, time_end;
	double elapsed, xeonphi_elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of elements: %i\n", n);
	
	vector = calloc(n, sizeof(myfloat));
	assert(vector != NULL);
	
//	omp_set_num_threads(2);
	
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
		vector[i] = 1.0;
		
	gettimeofday(&time_start, NULL);
	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));
	sumv = sum(vector, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;
	
	printf("sum value is %.3f, which is %s\ntotal time: %.3f seconds\nxeon phi time: %.3f seconds\n", (double)sumv, awnser[sumv == (float)n], elapsed, xeonphi_elapsed);
	
	return 0;
}
