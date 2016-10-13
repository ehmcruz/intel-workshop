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

static uint64_t fib_par (int n)
{
	if (n < 37)
		return fib_seq(n);
	else {
		uint64_t a, b;

		#pragma omp task shared(a)
		a = fib_par(n-1);
		
//		#pragma omp task shared(b)
		b = fib_par(n-2);
		
		#pragma omp taskwait

		return a + b;
	}
}

uint64_t fib (int n)
{
	uint64_t r;
	
	#pragma omp parallel
	{
		#pragma omp single
		{
			r = fib_par(n);
		}
	}
	
	return r;
}

