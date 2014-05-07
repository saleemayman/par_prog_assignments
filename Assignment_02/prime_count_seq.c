#include <stdlib.h>
#include <math.h>
#include "stdio.h"

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	/*printf("prime_count_seq started ....\n");*/
	long N, j;
	unsigned long sqrRoot, sum = 0;
	int check;

	for (N = a; N <= b; N++)
	{
		check = 1;
		sqrRoot = sqrt(N) + 1;
		for (j = 2; j <= sqrRoot; j++)
		{
			if ((N % j) == 0)
			{
				check = 0;
				break;
			}
		}
		if (check == 1)
		{
			sum++;
		}
	}
	return sum;
}
