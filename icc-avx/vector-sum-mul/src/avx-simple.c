#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include <immintrin.h>
#include <assert.h>

#include "sum.h"


myfloat vsum(myfloat *a, myfloat *b, int n)
{
	int i;
	myfloat r;
	__m256d ac, va, vb, mul;
	double partial[4];
	
	printf("avx simples\n");
	
	__asm__ __volatile__ ("nop\nnop\nnop");
	
	ac = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
	
	for (i=0; i<n; i+=4) {
		va = _mm256_load_pd(&a[i]);
		vb = _mm256_load_pd(&b[i]);
		mul = _mm256_mul_pd(va, vb);
		ac = _mm256_add_pd(ac, mul);
	}
	
	_mm256_storeu_pd(partial, ac);
	
	r = partial[0] + partial[1] + partial[2] + partial[3];
	
	__asm__ __volatile__ ("nop\nnop\nnop");

	return r;
}
