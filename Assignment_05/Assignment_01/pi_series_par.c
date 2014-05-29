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