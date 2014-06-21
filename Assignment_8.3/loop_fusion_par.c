#include <omp.h>

void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N) {


	#pragma omp parallel for
	for (int i = 1; i < N; i++)
	{
		a[i][1] = 2 * b[i][1];
		d[i][1] = a[i][1] * c[i][1];
		a[i][2] = 2 * b[i][2];
		d[i][2] = a[i][2] * c[i][2];
		c[i][0] = a[i][0] - a[i][2];

		#pragma omp parallel for
		for (int j = 3; j < N; j++)
		{
			a[i][j] = 2 * b[i][j];
			d[i][j] = a[i][j] * c[i][j];
			c[i][j-2] = a[i][j-2] - a[i][j];
		}

		c[i][N-2] = a[i][N-2] - a[i][N];
	}
}


/*********************** Serial Code ***********************/
/*
void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N) {

	for (int i = 1; i < N; i++) {
		for (int j = 1; j < N; j++) {
			a[i][j] = 2 * b[i][j];
			d[i][j] = a[i][j] * c[i][j];
		}
	}

	for (int j = 1; j < N; j++) {
		for (int i = 1; i < N; i++) {
			c[i][j-1] = a[i][j-1] - a[i][j+1];
		}
	}
}
*/