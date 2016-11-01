#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

void do_parallel (int n)
{
	int i;
	for (i=0; i<5; i++) {
		printf("hello %i\n", n);
		usleep(500000);
	}
}

int main ()
{
	#pragma omp task
	do_parallel(5);

	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task
			do_parallel(0);

			#pragma omp task
			do_parallel(1);
			
			#pragma omp taskwait

			do_parallel(2);
		}
	}
	
	do_parallel(10);
	
	printf("good bye\n");
	return 0;
}
