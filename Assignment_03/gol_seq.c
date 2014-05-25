#include <stdlib.h>
#include <string.h>
#include "stdio.h"


void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned int dim_x, unsigned int dim_y, unsigned int x, unsigned int y)
{
	/*unsigned char (*in)[dim_x] = (unsigned char (*)[dim_x])grid_in;*/

	/*printf("x: %x, y: %x, in: %u \n", x, y, grid_in[x+y*dim_x]);*/
	int count = 0;

	int i, j, x_tmp, y_tmp;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			x_tmp = x + j - 1;
			y_tmp = y + i - 1;

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

			/*printf("x: %x, y: %x, xt: %i, yt: %i, grid: %x\n", x, y, (j - 1), (i - 1), grid_in[x_tmp + y_tmp * dim_x]);*/
			if (grid_in[x_tmp + y_tmp * dim_x] == 1)
			{
				count++;
			}
		}
	}


/*	if (grid_in[x + y * dim_x] == 1)
		count--;*/

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
	/*printf("Entered gol() \n");*/
	unsigned char *grid_in, *grid_out, *grid_tmp;
	size_t size = sizeof(unsigned char) * dim_x * dim_y;

	grid_tmp = malloc(size);
	if(grid_tmp == NULL)
		exit(EXIT_FAILURE);

	memset(grid_tmp, 0, size);

	grid_in = grid;
	grid_out = grid_tmp;

	/*printf("Outside main loop in gol() \n");*/
	for (int t = 0; t < time_steps; ++t)
	{
		for (int y = 0; y < dim_y; ++y)
		{
			for (int x = 0; x < dim_x; ++x)
			{
				evolve(grid_in, grid_out, dim_x, dim_y, x, y);
			}
		}
		swap(&grid_in, &grid_out);
	}
	/*printf("Main loop gol() exited\n");*/

	if(grid != grid_in)
		memcpy(grid, grid_in, size);

	free(grid_tmp);

	return cells_alive(grid, dim_x, dim_y);
}
