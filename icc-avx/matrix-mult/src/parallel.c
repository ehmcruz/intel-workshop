#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "matrix.h"

void matrix_mult (myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols)
{
	int i, j, k;
	myfloat sum;

	#pragma omp parallel for private(i, j, k, sum)	
	for (i=0; i<first_rows; i++) {
		for (j=0; j<second_cols; j++) {
			sum = 0;
			
			for (k=0; k<first_cols; k++) {
				sum += first[i*first_cols+k] * second[k*second_cols+j];
			}

			multiply[i*second_cols+j] = sum;
		}
	}
}
