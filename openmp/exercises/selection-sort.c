#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

static int *vector;
static int *test;

void selection_sort(int *v, int n)
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

void quick_sort (int *a, int n)
{
	int i, j, p, t;
	
	if (n < 2)
		return;
	
	p = a[n / 2];
	for (i = 0, j = n - 1;; i++, j--) {
		while (a[i] < p)
			i++;
		while (p < a[j])
			j--;
		if (i >= j)
			break;
		t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
	quick_sort(a, i);
	quick_sort(a + i, n - i);
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };
	
	int i, n, is_ok;
	struct timeval time_start, time_end;
	double elapsed;
	
	assert(argc == 2);
	
	n = atoi(argv[1]);
	printf("number of elements: %i\n", n);
	
	vector = calloc(n, sizeof(int));
	assert(vector != NULL);

	test = calloc(n, sizeof(int));
	assert(test != NULL);
	
	srand(time(NULL));
	printf("randoming vector...\n");
	
	for (i=0; i<n; i++) {
		vector[i] = rand();
		test[i] = vector[i];
	}
	printf("done!\n");
	
	printf("sorting...\n");
	gettimeofday(&time_start, NULL);
	selection_sort(vector, n);
	gettimeofday(&time_end, NULL);
	
	printf("checking if output is correct...\n");
	quick_sort(test, n);
	
	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

	is_ok = (memcmp(vector, test, n*sizeof(int)) == 0);
	
	printf("sorted values are %s\ntime: %.3f seconds\n", awnser[is_ok], elapsed);
	
	return 0;
}
