#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sum.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

/*int aaa[1024*1024*100];*/

myfloat sum(myfloat *v, int n)
{
	int i, nthreads;
	myfloat sum;
	
	printf("parallel offload\n");
	
	sum = 0.0;
	
	#pragma offload target(mic) in(v:length(n)) inout(xeonphi_time_start,xeonphi_time_end)
	{
/*	aaa[0] = 0;*/
		#pragma omp parallel
		{
			#pragma omp single
			{
				nthreads = omp_get_num_threads();
			}
		}
		
		gettimeofday(&xeonphi_time_start, NULL);
		
		#pragma omp parallel for private(i) reduction(+:sum)
		for (i=0; i<n; i++) {
			sum += v[i];
		}
		
		gettimeofday(&xeonphi_time_end, NULL);
	}
	
	printf("nthreads was %i\n", nthreads);
	
	return sum;
}
