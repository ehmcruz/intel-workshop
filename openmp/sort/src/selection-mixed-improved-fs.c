#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

#define MAX_THREADS 256
int partial[MAX_THREADS];

void sort(int *v, int n){
	int i, j, min, tmp;
	int the_min, myid, nthreads;

	for(i = 0; i< n - 1; i++){
		#pragma omp parallel default(shared) private(j, min, myid)
		{
			#pragma omp single
			nthreads = omp_get_num_threads();

			myid = omp_get_thread_num();

			min = i;
			#pragma omp for
			for(j = i + 1; j < n; j++)
				if(v[j] < v[min])
					min = j;
			
			partial[myid] = min;
		}

		the_min = partial[0];
		for(j = 0; j < nthreads; j++)
			if(v[partial[j]] < v[the_min])
				the_min = partial[j];

		tmp = v[i];
		v[i] = v[the_min];
		v[the_min] = tmp;
	}
}