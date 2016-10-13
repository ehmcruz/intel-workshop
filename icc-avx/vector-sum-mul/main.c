#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/sum.h"

static myfloat *va;
static myfloat *vb;

/*__attribute__((target(mic))) struct timeval xeonphi_time_start, xeonphi_time_end;*/

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
	
//	omp_set_num_threads(2);
	
	#pragma omp parallel
	{
		#pragma omp single
		{
			printf("there are %u threads %u\n", omp_get_num_threads(), omp_get_thread_num());
		}

		#pragma omp single
		{
			printf("ola %u\n", omp_get_thread_num());
		}

		
//		#pragma omp barrier
		
		printf("hello from thread %u\n", omp_get_thread_num());
		
	//	#pragma omp barrier
		
		#pragma omp master
		{
			printf("the master thread is %u\n", omp_get_thread_num());
		}
	}
	
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
