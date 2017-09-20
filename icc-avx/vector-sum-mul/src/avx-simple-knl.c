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
	__m512d ac, va, vb, mul;
	double partial[8];
	
	printf("avx simple knl\n");
	
	ac = _mm512_set_pd(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	
	for (i=0; i<n; i+=8) {
		va = _mm512_load_pd(&a[i]);
		vb = _mm512_load_pd(&b[i]);
		mul = _mm512_mul_pd(va, vb);
		ac = _mm512_add_pd(ac, mul);
	}
	
	_mm512_storeu_pd(partial, ac);
	
	r = partial[0] + partial[1] + partial[2] + partial[3] + partial[4] + partial[5] + partial[6] + partial[7];
	
	return r;
}
