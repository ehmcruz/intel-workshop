#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

int sum(int *v, int n){
	int i, sum = 0;
	int sum_local, myid, nthreads;
	int init, end, slice;
	
	#pragma omp parallel default(shared) private(i, sum_local, myid, init, end)
	{
		sum_local = 0;
		myid = omp_get_thread_num();

		#pragma omp single
		{
			nthreads = omp_get_num_threads();
			slice = n / nthreads;
		}

		init = myid * slice;
		if(myid == nthreads - 1)
			end = n;
		else
			end = init + slice;

		for(i = init; i < end; i++)
			sum_local += v[i];
	
		#pragma omp atomic
		sum += sum_local;
	}
	
	return sum;
}