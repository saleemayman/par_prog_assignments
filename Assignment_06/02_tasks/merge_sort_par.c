#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "merge_sort.h"
#include "helper.h"

void swap(int64_t **a, int64_t **b)
{
	int64_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

void merge(int64_t *a, int64_t *b, size_t begin, size_t mid, size_t end)
{
	size_t l = begin, r = mid, idx = begin;

	while (l < mid && r < end)
		b[idx++] = a[l] < a[r] ? a[l++] : a[r++];

	while(l < mid)
		b[idx++] = a[l++];

	while (r < end)
		b[idx++] = a[r++];
}

void split(int64_t *a, int64_t *b, size_t begin, size_t end, int threshold)
{
	if(end - begin < 2)
		return;

	swap(&a, &b);

	size_t mid = (begin + end) / 2;

	size_t array_dim = (end - begin) + 1;
	
	/* create tasks but not for every case*/
	#pragma omp task if ( array_dim > threshold)
	{
		split(a, b, begin, mid, threshold);	
		split(a, b, mid, end, threshold);	

		/* wait for 'em to finish	*/
		#pragma omp taskwait
		merge(a, b, begin, mid, end);	
	}
	
}


/*	merge_sort(a, num_elements, num_threads, threshold);	*/
void merge_sort(int64_t *a, size_t num_elements, int num_threads, int threshold)
{
	omp_set_num_threads(num_threads);

	size_t size = num_elements * sizeof(int64_t);
	int64_t *b = malloc(size);

	if(b == NULL)
		exit(EXIT_FAILURE);

	memcpy(b, a, size);

	/*	parallel region ....	*/
	#pragma omp parallel
	{
		#pragma omp single
		{
			split(a, b, 0, num_elements, threshold);	
		}
	}
	

	free(b);
}