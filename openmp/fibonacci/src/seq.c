#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "fib.h"

uint64_t fib (int n)
{
	if (n < 2)
		return n;
	else
		return fib(n-1) + fib(n-2);
}
