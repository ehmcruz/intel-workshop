#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sum.h"

myfloat vsum(myfloat *a, myfloat *b, int n)
{
	int i;
	myfloat r = 0;
	
	printf("sequential\n");

	__asm__ __volatile__ ("nop\nnop\nnop");
	
	for (i=0; i<n; i++) {
		r += a[i] * b[i];
	}
	
	__asm__ __volatile__ ("nop\nnop\nnop");
	
	return r;
}
