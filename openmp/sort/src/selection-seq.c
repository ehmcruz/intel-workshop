#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.h"

void sort(int *v, int n)
{
	int i, j, min, tmp;

	for (j = 0; j < n-1; j++) {
		min = j;

		for ( i = j+1; i < n; i++) {
			if (v[i] < v[min])
				min = i;
		}

		if(min != j) {
			tmp = v[j];
			v[j] = v[min];
			v[min] = tmp;
		}
	}
}
