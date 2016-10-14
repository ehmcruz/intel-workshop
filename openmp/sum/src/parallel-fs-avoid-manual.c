#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define MAX_THREADS 256
#define CACHE_LINE_SIZE 64

struct element_t {
	int sum;
	char lixo[60];
};

struct element_t partial[MAX_THREADS];

int sum(int *v, int n)
{
	int i, sum, nthreads, myid;
	
	printf("parallel manual sizeof(struct element_t) is %i\n", sizeof(struct element_t));
	
	sum = 0;
	
	for (i=0; i<MAX_THREADS; i++)
		partial[i].sum = 0;
	
	#pragma omp parallel shared(sum, nthreads) private(i,myid)
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
		
/*		#pragma omp flush(nthreads)*/
		
		myid = omp_get_thread_num();
		
		for (i=myid; i<n; i+=nthreads) {
			partial[myid].sum += v[i];
		}
	}
	
	for (i=0; i<MAX_THREADS; i++)
		sum += partial[i].sum;
	
	return sum;
}
