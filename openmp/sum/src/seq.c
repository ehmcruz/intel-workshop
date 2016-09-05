#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum(int *v, int n)
{
	int i, sum;
	
	printf("sequential\n");
	
	sum = 0;
	
	for (i=0; i<n; i++) {
		sum += v[i];
	}
	
	return sum;
}
