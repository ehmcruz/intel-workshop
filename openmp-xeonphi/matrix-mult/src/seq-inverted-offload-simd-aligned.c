#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "matrix.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

void matrix_mult (myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols)
{
	#pragma offload target(mic) in(first:length(first_rows*first_cols)) in(second:length(first_cols*second_cols)) out(multiply:length(first_rows*second_cols))
	{
		int i, j, k;
		
		gettimeofday(&xeonphi_time_start, NULL);
		
		for (i=0; i<first_rows; i++) {
			for (j=0; j<second_cols; j++)
				multiply[i*second_cols+j] = 0.0;
		}
	
		for (i=0; i<first_rows; i++) {
			for (k=0; k<first_cols; k++) {
				#pragma vector aligned
				#pragma simd
				for (j=0; j<second_cols; j++) {
					multiply[i*second_cols+j] += first[i*first_cols+k] * second[k*second_cols+j];
				}
			}
		}
		
		gettimeofday(&xeonphi_time_end, NULL);
	}
}
