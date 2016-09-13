#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "fib.h"

static uint64_t fib_seq (int n)
{
	if (n < 2)
		return n;
	else
		return fib_seq(n-1) + fib_seq(n-2);
}

uint64_t fib (int n)
{
	if (n < 37)
		return fib_seq(n);
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
