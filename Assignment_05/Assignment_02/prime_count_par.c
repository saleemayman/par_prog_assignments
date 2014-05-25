#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned long tmp_start;
unsigned long tmp_end;

struct pthread_args
{
	unsigned long start, end, out, chunk_size, a, b;
};

void * check_is_prime(void *ptr)	{
	struct pthread_args *arg = ptr;
	long N, j;
	unsigned long sqrRoot, endVal;
	int check;
	endVal = arg->b;

	arg->out = 0;

	/*pthread_mutex_lock(&mutex);
	tmp_start = tmp_end + arg->a;
	tmp_end = tmp_start + arg->chunk_size - 1;

	arg->start = tmp_start;
	arg->end = tmp_end;
	pthread_mutex_unlock(&mutex);*/
	if ((arg->b - arg->a) < arg->chunk_size)
	{
		arg->chunk_size = (arg->b - arg->a);
	}

	/*printf("Before while. start: %lu, end: %lu, a: %lu, b: %lu \n", tmp_start, tmp_end, arg->a, arg->b);*/

	/* ----- ----- -----, ----- ----- -----, ----- ----- -----, ---- */
	while (tmp_end <= endVal)
	{
		pthread_mutex_lock(&mutex);
		tmp_start = tmp_end + 1;
		tmp_end = tmp_start + arg->chunk_size - 1;

		arg->start = tmp_start;
		arg->end = tmp_end;
		pthread_mutex_unlock(&mutex);

		/*printf("N1: %lu, endVal: %lu \n",N, endVal);*/
		for (N = arg->start; N <= arg->end; N++)
		{
			/*printf("start: %lu, end: %lu, N: %lu, endVal: %lu \n",arg->start, arg->end,  N, endVal);*/
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
				arg->out++;
			}
		}
			/*pthread_mutex_lock(&mutex);
			tmp_start = tmp_end + 1;
			tmp_end = tmp_start + arg->chunk_size - 1;

			arg->start = tmp_start;
			arg->end = tmp_end;
			pthread_mutex_unlock(&mutex);*/



	}
	/*printf("End___ while. start: %lu, end: %lu, a: %lu, b: %lu \n", tmp_start, tmp_end, arg->a, arg->b);*/
	return NULL;
}


unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	long j;
	unsigned long count = 0;

	tmp_start = 0;
	tmp_end = a;

	pthread_t *thread;
	thread = malloc(num_threads * sizeof (*thread));
	struct pthread_args *thread_arg;
	thread_arg = malloc(num_threads * sizeof (*thread_arg));

	for (j = 0; j < num_threads; j++)
	{
		thread_arg[j].a = a;
		thread_arg[j].b = b;
		thread_arg[j].chunk_size = chunk_size;
	}

	/*printf("prime_count_par start: %lu, end: %lu\n", tmp_start, tmp_end);*/
	/* Create threads for the computation*/
	for (j = 0; j < num_threads; j++)
	{
		pthread_create(&thread[j], 0, &check_is_prime, &thread_arg[j]);
	}

	/* Collect data from all the threads	*/
	for (j = 0; j < num_threads; j++)
	{
		pthread_join(thread[j], NULL);
		count += thread_arg[j].out;
	}

	free(thread);
    free(thread_arg);


	return count;
}
