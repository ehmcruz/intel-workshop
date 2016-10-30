#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

void sort(int *v, int n)
{
	int i, the_min, tmp;

	for (i=0; i<n-1; i++) {
		the_min = i;
	
		#pragma omp parallel
		{
			int j, min, myid, nthreads;
			
			nthreads = omp_get_num_threads();
			myid = omp_get_thread_num();
			min = i;

			for (j=i+myid; j<n; j+=nthreads) {
				if (v[j] < v[min])
					min = j;
			}
				
			#pragma omp critical
			{
				if (v[min] < v[the_min])
					the_min = min;
			}
		}
		
		tmp = v[i];
		v[i] = v[the_min];
		v[the_min] = tmp;
	}
}
