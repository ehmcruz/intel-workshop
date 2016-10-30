#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

#define MAX_THREADS 256
#define CACHE_LINE_SIZE (64*2)

struct element_t {
	int min;
} __attribute__ ((aligned (CACHE_LINE_SIZE)));

struct element_t partial[MAX_THREADS] __attribute__ ((aligned (CACHE_LINE_SIZE)));

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
	int the_min, tmp, nt_global, j;
	
	#pragma omp parallel private(j)
	{
		int min, myid, nthreads;
		
		nthreads = omp_get_num_threads();
		
		#pragma omp master
		nt_global = nthreads;
		
		myid = omp_get_thread_num();
		min = i;

		#pragma omp for
		for (j=i; j<n; j++) {
			if (v[j] < v[min])
				min = j;
		}
		
		partial[myid].min = min;
	}
	
	the_min = partial[0].min;

	for (j=0; j<nt_global; j++) {
		if (v[ partial[j].min ] < v[the_min])
			the_min = partial[j].min;
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
