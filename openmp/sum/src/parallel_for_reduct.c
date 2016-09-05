#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum(int *v, int n)
{
	int i, sum;
	
	printf("parallel for reduction\n");
	
	sum = 0;
	
	#pragma omp parallel for private(i) reduction(+:sum)
	for (i=0; i<n; i++) {
		sum += v[i];
	}
	
	return sum;
}
