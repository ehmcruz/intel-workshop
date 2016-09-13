#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

#define MAX_THREADS 64
#define CACHE_LINE_SIZE (64*2)

struct element_t {
	int sum;
} __attribute__ ((aligned (CACHE_LINE_SIZE)));

struct element_t partial[MAX_THREADS] __attribute__ ((aligned (CACHE_LINE_SIZE)));

static void seq_step (int *v, int n, int i)
{
	int j, min, myid, tmp;
	
	myid = omp_get_thread_num();
	min = i+myid;

	if (min >= n)
		min = i;

	for (j=i+myid; j<n; j++) {
		if (v[j] < v[min])
			min = j;
	}

	if(min != i) {
		tmp = v[i];
		v[i] = v[min];
		v[min] = tmp;
	}
}

static void parallel_step (int *v, int n, int i, int nthreads)
{
	int the_min, tmp, j;
	
	the_min = i;

/*printf("--------------------\n");*/
/*for (j=0; j<i; j++)*/
/*	printf("%i ", v[j]);*/
/*printf(",");*/
/*for (j=i; j<n; j++)*/
/*	printf("%i ", v[j]);*/
/*printf("\n");*/
	
	#pragma omp parallel
	{
		int j, min, myid;
		
		myid = omp_get_thread_num();
		min = i;

		for (j=i+myid; j<n; j+=nthreads) {
			if (v[j] < v[min])
				min = j;
		}
		
/*		#pragma omp barrier*/
		
		#pragma omp critical
		{
/*printf("%i. min threads %i = %i\n", i, myid, v[min]);*/
			if (v[min] < v[the_min])
				the_min = min;
		}
	}
		
	if (the_min != i) {
/*printf("switch %i <--> %i\n", i, the_min);*/
/*printf("%i. min threads %i = %i\n", i, myid, v[min]);*/
		tmp = v[i];
		v[i] = v[the_min];
		v[the_min] = tmp;
	}
}

/*static void parallel_step2 (int *v, int n, int i, int nthreads)*/
/*{*/
/*	#pragma omp parallel*/
/*	{*/
/*		int j, min, myid, tmp;*/
/*		*/
/*		myid = omp_get_thread_num();*/
/*		min = i;*/

/*		for (j=i+myid; j<n; j+=nthreads) {*/
/*			if (v[j] < v[min])*/
/*				min = j;*/
/*		}*/
/*		*/
/*		#pragma omp barrier*/

/*		#pragma omp critical*/
/*		{*/
/*			if(min != i) {*/
/*				tmp = v[i];*/
/*				v[i] = v[min];*/
/*				v[min] = tmp;*/
/*			}*/
/*		}*/
/*	}*/
/*}*/

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
/*		if ((n-i-1) < 100)*/
/*			seq_step(v, n, i);*/
/*		else*/
			parallel_step(v, n, i, nthreads);
	}
}
