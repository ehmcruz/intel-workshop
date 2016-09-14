#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sum.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

myfloat sum(myfloat *v, int n)
{
	int i;
	myfloat sum;
	
	printf("sequential offload\n");
	
	sum = 0.0;
	
	#pragma offload target(mic) in(v:length(n))
	{
		gettimeofday(&xeonphi_time_start, NULL);
		sum = __sec_reduce_add(v[0:n]);
		gettimeofday(&xeonphi_time_end, NULL);
	}
	
	return sum;
}
