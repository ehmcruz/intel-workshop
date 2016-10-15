#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

typedef double myfloat;

static myfloat *vector;

_Cilk_shared struct timeval xeonphi_time_start, xeonphi_time_end;

_Cilk_shared myfloat sum(myfloat *v, int n)
{
	int i;
	myfloat sum;

#ifndef __MIC__
	printf("we want xeon phi to run this\n");
	exit(1);
#endif

	printf("sequential offload implicit\n");
	
	sum = 0.0;

	gettimeofday(&xeonphi_time_start, NULL);
	
	for (i=0; i<n; i++) {
		sum += v[i];
	}
	
	gettimeofday(&xeonphi_time_end, NULL);
	
	return sum;
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, n;
	myfloat sumv;
	struct timeval time_start, time_end;
	double elapsed, xeonphi_elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of elements: %i\n", n);
	
	vector = _Offload_shared_aligned_malloc(n*sizeof(myfloat), 64);
	assert(vector != NULL);
	
	for (i=0; i<n; i++)
		vector[i] = 1.0;
		
	gettimeofday(&time_start, NULL);
	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));
	sumv = _Cilk_offload sum(vector, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;
	
	printf("sum value is %.3f, which is %s\ntotal time: %.3f seconds\nxeon phi time: %.3f seconds\n", (double)sumv, awnser[sumv == (float)n], elapsed, xeonphi_elapsed);
	
	return 0;
}
