#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum(int *v, int n)
{
	int i, sum;
	
	printf("parallel for\n");
	
	sum = 0;
	
	#pragma omp parallel for private(i) shared(sum)
	for (i=0; i<n; i++) {
			#pragma omp atomic
			sum += v[i];
	}
	
	return sum;
}
