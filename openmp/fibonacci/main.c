#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

#include "src/fib.h"

// 0 1 2 3 4 5 6
// 0 1 1 2 3 5 8 ...

uint64_t fib_check (int n)
{
	int i;
	uint64_t prev, fib, tmp;

	if (n < 2)
		return n;

	prev = 0;
	fib = 1;

	for (i=1; i<n; i++) {
		tmp = fib;
		fib += prev;
		prev = tmp;
	}
	
	return fib;
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };

	int i, n, is_ok;
	uint64_t r;
	struct timeval time_start, time_end;
	double elapsed;

	assert(argc == 2);

	n = atoi(argv[1]);
	printf("fib seq: %i\n", n);

/*	omp_set_num_threads(2);*/

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
	
/*	printf("fib_check:");*/
/*	for (i=0; i<n; i++)*/
/*		printf(" %i", fib_check(i));*/
/*	printf("\n");*/
/*	printf("fib      :");*/
/*	for (i=0; i<n; i++)*/
/*		printf(" %i", fib(i));*/
/*	printf("\n");*/

	gettimeofday(&time_start, NULL);
	r = fib(n);
	gettimeofday(&time_end, NULL);

	is_ok = (r == fib_check(n));

	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

	printf("fibonacci is %llu, which is %s\ntime: %.3f seconds\n", r, awnser[is_ok], elapsed);

	return 0;
}
