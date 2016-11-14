#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

int sum(int *v, int n){
	int i, sum = 0;
	int sum_local, myid, nthreads;
	
	#pragma omp parallel default(shared) private(i, sum_local, myid)
	{
		sum_local = 0;
		myid = omp_get_thread_num();

		#pragma omp single
		nthreads = omp_get_num_threads();

		for(i = myid; i < n; i += nthreads)
			sum_local += v[i];
	
		#pragma omp atomic
		sum += sum_local;
	}
	
	return sum;
}