#include <time.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * increment( void * i_void_ptr )
{
	int * i = ( int *) i_void_ptr;
	int j;

	for (j=0; j < NUM; j++)
	{
		pthread_mutex_lock(&mutex);
		(* i )++;
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main()
{
	int i = 0;
	int j, k;
	pthread_t *thr;
	thr = malloc(2 * sizeof (*thr));

	for (k = 1; k < 2; k++)
	{
		pthread_create(&thr[k] , NULL, &increment , &i ) ;
	}

	for (j=0; j < NUM; j++)
	{
		pthread_mutex_lock(&mutex) ;
		i++;
		pthread_mutex_unlock(&mutex) ;
	}
	for (k = 1; k < 2; k++)
	{
		pthread_join (thr[k] , NULL) ;
	}

	printf ( "Value of i = %d\n" , i ) ;

	return 0;
}
