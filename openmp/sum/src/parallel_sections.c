#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum(int *v, int n)
{
	int i, sum;
	
	printf("parallel sections\n");
	
	sum = 0;
	
	#pragma omp parallel sections private(i) shared(sum)
	{
		#pragma omp section
		{
			for (i=0; i<n/2; i++) {
				#pragma omp atomic
				sum += v[i];
			}
		}
		
		#pragma omp section
		{
			for (i=n/2; i<n; i++) {
				#pragma omp atomic
				sum += v[i];
			}
		}
	}
	
	return sum;
}
