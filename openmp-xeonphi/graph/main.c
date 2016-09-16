#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#include "src/graph.h"

__attribute__((target(mic))) struct timeval xeonphi_time_start, xeonphi_time_end;

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, arity, nlevels;
	uint32_t n;
	struct timeval time_start, time_end;
	double elapsed, xeonphi_elapsed;
	
	assert(argc == 3);
	
	arity = atoi(argv[1]);
	nlevels = atoi(argv[2]);
	
	assert(arity >= 2);
	assert(nlevels >= 1);
	
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
	
	create_tree(arity, nlevels);
		
	gettimeofday(&time_start, NULL);
	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));
	n = touch_all_nodes();
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;
	
	printf("%u nodes touched\ntotal time: %.3f seconds\nxeon phi time: %.3f seconds\n", n, elapsed, xeonphi_elapsed);
	
	return 0;
}
