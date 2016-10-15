#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "sum.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

myfloat sum(myfloat *v, int n)
{
	int i;
	myfloat sum;
/*	struct timeval xeonphi_time_start;*/
	
	printf("sequential offload\n");
	
	sum = 0.0;
	
	#pragma offload target(mic) in(v:length(n)) inout(xeonphi_time_start,xeonphi_time_end)
	{
		gettimeofday(&xeonphi_time_start, NULL);
		
		#pragma vector aligned
		#pragma simd reduction(+:sum)
		for (i=0; i<n; i++) {
			sum += v[i];
		}
		
		gettimeofday(&xeonphi_time_end, NULL);
	}
	
	return sum;
}
