#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "matrix.h"

void matrix_mult (int **first, int **second, int **multiply, int first_rows, int first_cols, int second_cols)
{
	int sum, i, j, k;
	
	for (i=0; i<first_rows; i++) {
		for (j=0; j<second_cols; j++) {
			sum = 0;
			
			for (k=0; k<first_cols; k++) {
				sum += first[i][k]*second[k][j];
			}

			multiply[i][j] = sum;
		}
	}
}
