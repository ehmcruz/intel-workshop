#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sum.h"

myfloat sum(myfloat *v, int n)
{
	int i;
	myfloat sum;
	
	printf("parallel for reduction\n");
	
	sum = 0.0;
	
	#pragma omp parallel for private(i) reduction(+:sum)
	for (i=0; i<n; i++) {
		sum += v[i];
	}
	
	return sum;
}
