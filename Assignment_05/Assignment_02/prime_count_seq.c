#include <stdlib.h>
#include <math.h>
#include "stdio.h"
#include <stdbool.h>

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	/*printf("prime_count_seq started ....\n");*/
	long N, j;
	unsigned long sqrRoot, sum = 0;
	bool check;
/*
	if (a == 1)
	{
		a += 2;
	}
*/
	for (N = a; N <= b; N++)
	{
		check = true;
		sqrRoot = sqrt(N) + 1;

		for (j = 2; j <= sqrRoot; j++)
		{
			if ((N % j) == 0)
			{
				check = false;
				break;
			}
			printf("N: %ld \n", N);
		}


		if (check == true && N != 1)
		{
			sum++;
		}
	}

	return sum;
}
