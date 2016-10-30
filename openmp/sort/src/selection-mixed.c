#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

static void seq_step (int *v, int n, int i)
{
	int j, min, tmp;
	
	min = i;

	for (j= i+1; j<n; j++) {
		if (v[j] < v[min])
			min = j;
	}

	tmp = v[i];
	v[i] = v[min];
	v[min] = tmp;
}

static void parallel_step (int *v, int n, int i)
{
	int the_min, tmp;
	
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

void sort(int *v, int n)
{
	int i, nthreads;

	#pragma omp parallel
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
	}

	for (i=0; i<n-1; i++) {
		if ((n-i) < 10000)
			seq_step(v, n, i);
		else
			parallel_step(v, n, i);
	}
}
