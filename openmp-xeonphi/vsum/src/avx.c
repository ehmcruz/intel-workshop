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
	union {
		__int64 vint;
		double vdouble;
	} convert;
	union {
		__m256i vint;
		__m256d vdouble;
	} convert2;
	
	printf("avx\n");
	
	assert((n%4) == 0);
	assert(sizeof(convert.vint) == 8 && sizeof(convert.vdouble) == 8 && sizeof(convert) == 8);
	assert(sizeof(convert2.vint) == 32 && sizeof(convert2.vdouble) == 32 && sizeof(convert2) == 32);

	__asm__ __volatile__ ("nop\nnop\nnop");
	
	ac = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
	
	for (i=0; i<n; i+=4) {
		va = _mm256_load_pd(&a[i]);
		vb = _mm256_load_pd(&b[i]);
		mul = _mm256_mul_pd(va, vb);
		ac = _mm256_add_pd(ac, mul);
	}
	
	convert2.vdouble = ac;

	r = 0;
	convert.vint = _mm256_extract_epi64(convert2.vint, 0);
	r += convert.vdouble;
	convert.vint = _mm256_extract_epi64(convert2.vint, 1);
	r += convert.vdouble;
	convert.vint = _mm256_extract_epi64(convert2.vint, 2);
	r += convert.vdouble;
	convert.vint = _mm256_extract_epi64(convert2.vint, 3);
	r += convert.vdouble;
	
	__asm__ __volatile__ ("nop\nnop\nnop");

	return r;
}
