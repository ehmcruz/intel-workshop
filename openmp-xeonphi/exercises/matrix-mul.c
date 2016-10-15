#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

typedef double myfloat;

static myfloat *first;
static myfloat *second;
static myfloat *multiply;

__attribute__((target(mic))) struct timeval xeonphi_time_start, xeonphi_time_end;

void matrix_mult (myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols);

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, j, n, is_ok;
	struct timeval time_start, time_end;
	double elapsed, xeonphi_elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	
	n = ((n%8) == 0) ? n : n + (8 - n%8);
	
	printf("number of rows x cols: %i\n", n);
	
	first = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(first != NULL);
	
	second = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(second != NULL);
	
	multiply = _mm_malloc(n * n * sizeof(myfloat), 64);
	assert(multiply != NULL);
	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			first[i*n+j] = 1;
			second[i*n+j] = 1;
		}
	}
	
	gettimeofday(&time_start, NULL);
	memcpy(&xeonphi_time_start, &time_start, sizeof(struct timeval));
	memcpy(&xeonphi_time_end, &time_start, sizeof(struct timeval));
	#pragma noinline
	matrix_mult(first, second, multiply, n, n, n);
	gettimeofday(&time_end, NULL);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	
	xeonphi_elapsed = xeonphi_time_end.tv_sec - xeonphi_time_start.tv_sec + (xeonphi_time_end.tv_usec - xeonphi_time_start.tv_usec) / 1000000.0;

/*	for (i=0; i<n; i++) {*/
/*		for (j=0; j<n; j++) {*/
/*			printf("%i ", multiply[i][j]);*/
/*		}*/
/*		printf("\n");*/
/*	}*/

	is_ok = 1;	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			if ((double)multiply[i*n+j] != (double)n) {
/*				printf("i %i j %i multiply[i][j] %i\n", i, j, multiply[i][j]);*/
				is_ok = 0;
				i = n;
				j = n;
			}
		}
	}
	
	printf("multiply values are %s\ntotal time: %.3f seconds\nxeon phi time: %.3f seconds\n", awnser[is_ok], elapsed, xeonphi_elapsed);
	
	return 0;
}

void matrix_mult (myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols)
{
	int i, j, k;

	for (i=0; i<first_rows; i++) {
		for (j=0; j<second_cols; j++)
			multiply[i*second_cols+j] = 0.0;
	}
	
	for (i=0; i<first_rows; i++) {
		for (k=0; k<first_cols; k++) {
			for (j=0; j<second_cols; j++) {
				multiply[i*second_cols+j] += first[i*first_cols+k] * second[k*second_cols+j];
			}
		}
	}
}

