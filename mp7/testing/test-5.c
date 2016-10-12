#include "test_utility.c"
#include <pthread.h>

int NUM_THREADS = 25;

void* update_thread(void* arg){
	int rev = *((int*)arg);
	while(!STD_UPDATE_SUCCESS(1, 1, rev)){
		continue;
	}
        return NULL;
}

void main(void)
{
    load();
	pthread_t * threads = malloc(sizeof(pthread_t)*NUM_THREADS);
	STD_ADD_SUCCESS(1);
	int i;
        int rev[NUM_THREADS];
	for(i=0; i<NUM_THREADS; i++)
        {
		rev[i] = i+1;
		pthread_create(&threads[i], NULL, update_thread, &rev[i]);
	}

	for(i=0; i<NUM_THREADS; i++)
        {
		pthread_join(threads[i], NULL);
	}
	
	if (!STD_GET_SUCCESS(1, 1, NUM_THREADS+1))
        	fail(5);
    save();
    free(threads);
    success(5);
}
