#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define MAX_THREADS 64

int partial[MAX_THREADS];

int sum(int *v, int n)
{
	int i, sum, nthreads, myid;
	
	printf("parallel\n");
	
	sum = 0;
	
	for (i=0; i<MAX_THREADS; i++)
		partial[i] = 0;
	
	#pragma omp parallel shared(sum, nthreads) private(i,myid)
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
		
//		#pragma omp flush(nthreads)
		
		myid = omp_get_thread_num();
		
		for (i=myid; i<n; i+=nthreads) {
			partial[myid] += v[i];
		}
	}
	
	for (i=0; i<MAX_THREADS; i++)
		sum += partial[i];
	
	return sum;
}
