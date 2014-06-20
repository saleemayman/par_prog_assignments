#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double pi_series(long num_terms, long num_threads)
{
	double sum = 0.0;
	omp_set_num_threads(num_threads);
	
	#pragma omp parallel
	{
		#pragma omp for reduction(+:sum)
		for (unsigned long n = 0; n < num_terms; n++)
		{
			sum += powf((-1.0f),n) / ((double)(2 * n + 1));
		}
	}
return (sum * 4);
}

void split(int64_t *a, int64_t *b, size_t begin, size_t end, int threshold)
{
if(end - begin < 2)
return;

swap(&a, &b);

size_t mid = (begin + end) / 2;

size_t size_array = end - begin;


#pragma omp task if(size_array +1 > threshold)
split(a, b, begin, mid, threshold);
split(a, b, mid, end, threshold);


#pragma omp taskwait
merge(a, b, begin, mid, end);
}

void merge_sort(int64_t *a, size_t num_elements, int num_threads, int threshold)
{
size_t size = num_elements * sizeof(int64_t);
int64_t *b = malloc(size);

if(b == NULL)
exit(EXIT_FAILURE);

memcpy(b, a, size);

#pragma omp parallel
{
#pragma omp single
{

split(a, b, 0, num_elements, threshold);
}
}

free(b);
}