#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "fib.h"

uint64_t fib (int n)
{
	if (n < 2)
		return n;
	else {
		uint64_t a, b;
		
		#pragma omp parallel sections
		{
			#pragma omp section
			a = fib(n-1);
			
			#pragma omp section
			b = fib(n-2);
		}
		
		return a + b;
	}
}
