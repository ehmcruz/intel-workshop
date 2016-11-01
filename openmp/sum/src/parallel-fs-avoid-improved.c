#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define MAX_THREADS 256
#define CACHE_LINE_SIZE (64*2)

struct element_t {
	int sum;
} __attribute__ ((aligned (CACHE_LINE_SIZE)));

struct element_t partial[MAX_THREADS] __attribute__ ((aligned (CACHE_LINE_SIZE)));

int sum(int *v, int n)
{
	int i, sum, nthreads, myid, init, end, slice;
	
	printf("parallel\n");
	
	sum = 0;
	
	for (i=0; i<MAX_THREADS; i++)
		partial[i].sum = 0;
	
	#pragma omp parallel shared(sum, nthreads) private(i,myid,init,end,slice)
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
		
/*		#pragma omp flush(nthreads)*/
		
		myid = omp_get_thread_num();
		slice = n / nthreads;
		init = myid*slice;
		
		if (myid == (nthreads-1))
			end = n;
		else
			end = init+slice;
		
		for (i=init; i<end; i++) {
			partial[myid].sum += v[i];
		}
	}
	
	for (i=0; i<MAX_THREADS; i++)
		sum += partial[i].sum;
	
	return sum;
}
