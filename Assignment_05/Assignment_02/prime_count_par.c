#include <stdlib.h>
#include <math.h>
#include "stdio.h"
#include <omp.h>

int prime_check(unsigned long number)
{
    unsigned long i, sqrRoot;
    int check;

    check = 1;

    /* 	ignore zero and one, but count two*/
    if(number == 0 || number == 1)
    {
        check = 0;
    }
    else if(number == 2)
    {
        check = 1;
    }
    else
    {
    	sqrRoot = sqrt(number) + 1;
        for(i = 2; i <= sqrRoot; i++)
        {
            if((number % i) == 0)
            {
                check = 0;
                break;
            }
        }
    }
    return check;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	/*printf("prime_count_seq started ....\n");*/
	long N;
	unsigned long sum = 0;

	omp_set_num_threads(num_threads);	/* Launch threaaaaaaaaaadsssssssss!!	*/

#pragma omp parallel
{
	#pragma omp for schedule(dynamic, chunk_size) reduction(+:sum) 
	for (N = a; N <= b; N++)
	{
		sum += prime_check(N);
	}
}
	return sum;
}
