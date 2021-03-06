#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct pthread_args
{
	double start, end, out;
};

void * pi_sum(void *ptr)	{
	struct pthread_args *arg = ptr;
	arg->out = 0;
	while (arg->start < arg->end) {
		arg->out += powf((-1.0f),arg->start) / ((double)(2 * arg->start + 1));
		arg->start++;
	}

	return NULL;
}

double pi_series(long num_terms, long num_threads)
{
	long j, i;
	/*long j_max = 0;*/
	double pi_val = 0.0;

	long iter = floor(num_terms/num_threads);
	long rem = num_terms % num_threads;

	pthread_t *thread;
	thread = malloc(num_threads * sizeof (*thread));
	struct pthread_args *thread_arg;
	thread_arg = malloc(num_threads * sizeof (*thread_arg));

	for (j = 0; j < num_threads; j++)
	{
		if (rem == 0)	/* If no remainder then distribute equal work to all threads	*/
		{
		thread_arg[j].start = j * iter;
		thread_arg[j].end = (j + 1) * iter;
		}
		else	/* If remainder exist then 1st thread takes the extra load of the remainder	*/
		{
			if (j == 0)
			{
				thread_arg[j].start = j * iter;
				thread_arg[j].end = (j + 1) * iter + rem;
			}
			else	/* Other threads take equal work load	*/
			{
				thread_arg[j].start = j * iter + rem;
				thread_arg[j].end = (j + 1) * iter + rem;
			}
		}
	}


	/* Create threads for the computation*/
	for (i = 0; i < num_threads; i++)
	{
		pthread_create(&thread[i], 0, &pi_sum, &thread_arg[i]);
	}

	/* Collect data from all the threads	*/
	for (i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], NULL);
		pi_val += thread_arg[i].out;
	}

	free(thread);
    free(thread_arg);

    return 4 * pi_val;
}



