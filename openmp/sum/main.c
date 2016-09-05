#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "src/sum.h"

#define VSIZE 1000000

static int vector[VSIZE];

int main(int argc, char **argv)
{
	int i, sumv;
	static char *awnser[] = { "bad", "ok" };
	
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
	
	for (i=0; i<VSIZE; i++)
		vector[i] = 1;
	
	sumv = sum(vector, VSIZE);
	
	printf("sum value is %u, w\"H\"ich is %s\n", sumv, awnser[sumv == VSIZE]);
	
	return 0;
}
