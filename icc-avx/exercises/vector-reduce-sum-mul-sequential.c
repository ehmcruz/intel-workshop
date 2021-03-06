#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

typedef double myfloat;

static myfloat *va;
static myfloat *vb;

myfloat vsum(myfloat *a, myfloat *b, int n)
{
	int i;
	myfloat r = 0;
		
	for (i=0; i<n; i++) {
		r += a[i] * b[i];
	}
	
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
	double elapsed;
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
	r = vsum(va, vb, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	printf("output is %f, which is %s\ntotal time: %.3f seconds\n", r, awnser[ check(r, n) ], elapsed);
	
	return 0;
}
