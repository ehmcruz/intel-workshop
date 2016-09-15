#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "sum.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

struct timeval _Cilk_shared shared_xeonphi_time_start, shared_xeonphi_time_end;

_Cilk_shared myfloat blah;

_Cilk_shared myfloat mic_sum (_Cilk_shared myfloat *v, int n)
{
	int i, nthreads = 1;
	myfloat sum;

#ifndef __MIC__
	printf("we want xeon phi to run this\n");
	exit(1);
#endif

	sum = 0.0;

	#pragma omp parallel
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
	}
	
	// Important:
	// we touch the data to force it to be transfered to the phi
	// why the hell they don't put this in the manual?

	blah = 0.0;
	#pragma omp parallel for private(i) reduction(+:sum)
	for (i=0; i<n; i++) {
		blah += v[i];
	}

	gettimeofday(&shared_xeonphi_time_start, NULL);
	
	#pragma omp parallel for private(i) reduction(+:sum)
	for (i=0; i<n; i++) {
		sum += v[i];
	}
	
	gettimeofday(&shared_xeonphi_time_end, NULL);

	printf("nthreads was %i\n", nthreads);
	
	return sum;
}

myfloat sum(myfloat *v, int n)
{
	myfloat sum;
	myfloat _Cilk_shared *implicit;
	
	printf("parallel implicit\nallocating implicit memory...\n");
	
	implicit = _Offload_shared_aligned_malloc(n*sizeof(myfloat), sizeof(myfloat));
	assert(implicit != NULL);
	
	memcpy(implicit, v, n*sizeof(myfloat));
	
	printf("done\n");
	
	sum = _Cilk_offload mic_sum(implicit, n);
	
	printf("xeon phi finished blah %.3f\n", blah);
	
	memcpy(&xeonphi_time_start, &shared_xeonphi_time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &shared_xeonphi_time_end, sizeof(struct timeval));
	
	_Offload_shared_aligned_free(implicit);
	
	return sum;
}
