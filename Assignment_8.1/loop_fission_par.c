#include <omp.h>

void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N)
{
	/* nested loops interchanged to increase data locality */
	for (int i=1; i<N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			c[i][j] = 3*d[i][j];
			d[i][j]   = 2*c[i+1][j];
		}
	}

	/* nested loops interchanged to increase data locality and OMP *used for the outer most loop */
	#pragma omp parallel for
	for (int i=1; i<N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			a[i][j]   = a[i][j]*b[i][j];
			b[i][j+1] = 2*a[i][j]*c[i-1][j];
		}
	}
}
