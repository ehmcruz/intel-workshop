#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

static int *vector;

#define MAX_THREADS 256

int partial[MAX_THREADS];

int sum(int *v, int n)
{
	int i, sum, nthreads, myid;
	
	sum = 0;
	
	for (i=0; i<MAX_THREADS; i++)
		partial[i] = 0;
	
	#pragma omp parallel shared(nthreads) private(i,myid)
	{
		#pragma omp single
		{
			nthreads = omp_get_num_threads();
		}
		
		myid = omp_get_thread_num();
		
		for (i=myid; i<n; i+=nthreads) {
			partial[myid] += v[i];
		}
	}
	
	for (i=0; i<MAX_THREADS; i++)
		sum += partial[i];
	
	return sum;
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, sumv, n;
	struct timeval time_start, time_end;
	double elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of elements: %i\n", n);
	
	vector = calloc(n, sizeof(int));
	assert(vector != NULL);
		
	for (i=0; i<n; i++)
		vector[i] = 1;
	
	gettimeofday(&time_start, NULL);
	sumv = sum(vector, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	printf("sum value is %u, which is %s\ntime: %.3f seconds\n", sumv, awnser[sumv == n], elapsed);
	
	return 0;
}
