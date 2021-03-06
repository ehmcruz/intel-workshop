#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum(int *v, int n)
{
	int i, sum, nthreads;
	
	printf("parallel\n");
	
	sum = 0;
	
	#pragma omp parallel shared(sum, nthreads) private(i)
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
				
		for (i=omp_get_thread_num(); i<n; i+=nthreads) {
			sum += v[i];
		}
	}
	
	return sum;
}
