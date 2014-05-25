#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "stdio.h"

struct pthread_args
{
	unsigned long ID;
	unsigned long num_threads;
	unsigned long col;
	unsigned long time_steps;
	unsigned long dim_x;
	unsigned long dim_y;
	unsigned char *grid_in;
	unsigned char *grid_out;
	pthread_barrier_t *barrier;
};

void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned int dim_x, unsigned int dim_y, unsigned int x, unsigned int y)
{
	int count = 0;

	int i, j, x_tmp, y_tmp;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			x_tmp = x + j - 1;
			y_tmp = y + i - 1;

			/* warp boundaries */
			if (x_tmp == -1)
			{
				x_tmp = dim_x - 1;
			} else if (x_tmp == dim_x)
			{
				x_tmp = 0;
			}
			if (y_tmp == -1)
			{
				y_tmp = dim_y - 1;
			} else if (y_tmp == dim_y)
			{
				y_tmp = 0;
			}

			/* Count the living cells*/
			if (grid_in[x_tmp + y_tmp * dim_x] == 1)
			{
				count++;
			}
		}
	}

	grid_out[x + y * dim_x] = grid_in[x + y * dim_x];

	if (grid_in[x + y * dim_x] == 1 && (count == 4 || count == 3))
	{
		grid_out[x + y * dim_x] = 1;
	}
	else if (grid_in[x + y * dim_x] == 0 && count == 3)
	{
		grid_out[x + y * dim_x] = 1;
	}
	else
	{
		grid_out[x + y * dim_x] = 0;
	}
	/*printf("x: %x, y: %x, cell: %x, out: %i , count: %i\n", x, y, grid_in[x + y * dim_x], grid_out[x + y * dim_x], count);*/
}


void swap(unsigned char **a, unsigned char **b)
{
	unsigned char *tmp = *a;
	*a = *b;
	*b = tmp;
}

void *thread_evolve(void *ptr)
{
	struct pthread_args *arg = ptr;
	const size_t size = sizeof(unsigned char) * arg->dim_x * arg->dim_y;
	const unsigned long x_max = arg->col*arg->num_threads;
	unsigned long t, x, y;

	for (t = 0; t < arg->time_steps; t++)
	{
		for(y = 0; y < arg->dim_y; y++)
		{
			for(x = arg->ID; x < x_max; x+= arg->num_threads)
			{
				evolve(arg->grid_in, arg->grid_out, arg->dim_x, arg->dim_y, x, y);
			}
		}

		pthread_barrier_wait(arg->barrier);
		swap(&arg->grid_in, &arg->grid_out);
	}

	if(arg->time_steps % 2 != 0)
	{
		memcpy(arg->grid_out, arg->grid_in, size);
	}
	return 0;
}



unsigned int cells_alive(unsigned char *grid, unsigned int dim_x, unsigned int dim_y)
{
	unsigned char (*c_grid)[dim_x] = (unsigned char (*)[dim_x])grid;

	unsigned int cells = 0;

	for (int y = 0; y < dim_y; ++y)
	{
		for (int x = 0; x < dim_x; ++x)
		{
			cells += c_grid[y][x];
		}
	}

	return cells;
}

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps, unsigned int num_threads)
{
	unsigned char *grid_in, *grid_out, *grid_tmp;
	size_t size = sizeof(unsigned char) * dim_x * dim_y;
	const unsigned long iter = dim_x / num_threads;
	const unsigned long rem = dim_x % num_threads;
	pthread_t *thread;
	struct pthread_args *thread_arg;
	pthread_barrier_t barrier;

	grid_tmp = malloc(size);
	if(grid_tmp == NULL)
		exit(EXIT_FAILURE);

	memset(grid_tmp, 0, size);

	grid_in = grid;
	grid_out = grid_tmp;


	thread = malloc(num_threads*sizeof(*thread));
	thread_arg = malloc(num_threads*sizeof(*thread_arg));

	for(unsigned long i = 0; i < num_threads; i++)
	{
		thread_arg[i].col = iter;
	}

	/* assign remaining terms to threads**/
	if(rem != 0)
	{
		for(unsigned long i = 0; i < rem; i++)
		{
			thread_arg[i].col += 1;
		}
	}


	pthread_barrier_init(&barrier, 0, num_threads);

	/* launch all threads*/
	for(unsigned long i = 0; i < num_threads; i++)
	{
		thread_arg[i].ID = i;
		thread_arg[i].dim_x = dim_x;
		thread_arg[i].dim_y = dim_y;
		thread_arg[i].num_threads = num_threads;
		thread_arg[i].time_steps = time_steps;
		thread_arg[i].grid_in = grid_in;
		thread_arg[i].grid_out = grid_out;
		thread_arg[i].barrier = &barrier;
		pthread_create(thread+i, 0, &thread_evolve, thread_arg+i);
	}

	/* Join threads and collect data*/
	for(unsigned long i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], 0);
	}

	pthread_barrier_destroy(&barrier);
	free(thread);
	free(thread_arg);
	free(grid_tmp);

	return cells_alive(grid, dim_x, dim_y);
}
