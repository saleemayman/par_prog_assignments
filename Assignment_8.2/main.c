#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "helper.h"
#include "loop_alignment.h"

unsigned long **get_int64_twodim_array(size_t num)
{
	unsigned long **array = (unsigned long**)malloc(num * sizeof(*array));
	if(array == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	for(int i = 0; i < num; i++) {
		array[i] = (unsigned long*)malloc(num * sizeof(unsigned long));
		if(array[i] == NULL) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}
	for (size_t i = 0; i < num; i++)
		for(size_t j = 0; j < num; j++)
			array[i][j] = num - j;

        return array;
}

void matrix_print(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%lu \t", a[i][j]);
		}
		printf("\t");
		for (int j = 0; j < N; j++)
		{
			printf("%lu \t", b[i][j]);
		}
		printf("\t");
		for (int j = 0; j < N; j++)
		{
			printf("%lu \t", c[i][j]);
		}
		printf("\t");
		for (int j = 0; j < N; j++)
		{
			printf("%lu \t", d[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {

    int N = 4;
    
	unsigned long **a = get_int64_twodim_array(N+1);
	unsigned long **b = get_int64_twodim_array(N+1);
	unsigned long **c = get_int64_twodim_array(N+1);
	unsigned long **d = get_int64_twodim_array(N+1);

	struct timespec begin, end;

	clock_gettime(CLOCK_REALTIME, &begin);
	compute(a, b, c, d, N);
	clock_gettime(CLOCK_REALTIME, &end);

	printf("\nProcessing Time: %.3lf seconds", ts_to_double(ts_diff(begin, end)));
	
	printf("\na: \t\t\t\t\tb: \t\t\t\t\tc: \t\t\t\t\td: \t\t\t\t\t\n");
	matrix_print(a, b, c, d, N);

	free(a);
	free(b);
	free(c);
	free(d);
	return 0;
}

