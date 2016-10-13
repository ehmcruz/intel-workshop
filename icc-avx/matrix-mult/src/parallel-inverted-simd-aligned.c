#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "matrix.h"

void matrix_mult (myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols)
{
	int i, j, k;

	for (i=0; i<first_rows; i++) {
		for (j=0; j<second_cols; j++)
			multiply[i*second_cols+j] = 0.0;
	}
	
	#pragma omp parallel for private(i, j, k)
	for (i=0; i<first_rows; i++) {
		for (k=0; k<first_cols; k++) {
			#pragma vector aligned
			#pragma simd
			for (j=0; j<second_cols; j++) {
				multiply[i*second_cols+j] += first[i*first_cols+k] * second[k*second_cols+j];
			}
		}
	}
}
