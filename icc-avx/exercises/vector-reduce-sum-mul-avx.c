#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>
#include <assert.h>

#include <immintrin.h>

typedef double myfloat;

static myfloat *va;
static myfloat *vb;

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
	
	assert((n%4) == 0);
	assert(sizeof(convert.vint) == 8 && sizeof(convert.vdouble) == 8 && sizeof(convert) == 8);
	assert(sizeof(convert2.vint) == 32 && sizeof(convert2.vdouble) == 32 && sizeof(convert2) == 32);
	
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
	
	return r;
}

static int check (myfloat r, int n)
{	
	return (r == (myfloat)(n*2));
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, n;
	struct timeval time_start, time_end;
	double elapsed; //, xeonphi_elapsed;
	myfloat r;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	
	if ((n%8) != 0)
		n += (8 - (n%8));
	
	printf("number of elements: %i\n", n);
	
	va = _mm_malloc(n * sizeof(myfloat), 64);
	assert(va != NULL);
	
	vb = _mm_malloc(n * sizeof(myfloat), 64);
	assert(vb != NULL);
	
	for (i=0; i<n; i++) {
		va[i] = 1;
		vb[i] = 2;
	}
		
	gettimeofday(&time_start, NULL);
/*	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));*/
/*	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));*/
	r = vsum(va, vb, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
/*	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;*/
	
	printf("output is %f, which is %s\ntotal time: %.3f seconds\n", r, awnser[ check(r, n) ], elapsed);
	
	return 0;
}
