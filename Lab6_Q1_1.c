/*
Smester: Fall 2018
Name: Terry-Ann Sneed
Lab: 6 Question I Part I
Goal: "Synchronize the two threads with mutexes so that Thread 0 is executed always first." 
*/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
// Declare mutex lock
pthread_mutex_t lock;

void* doSomeThing(void *id)
{
    //lock mutex 
    pthread_mutex_lock(&lock);  
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFF);i++);

    printf("\n Job %d finished\n", counter);
	
	// Unlock mutex
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void)
{
    unsigned long i = 0; 
    int err;	

	if (pthread_mutex_init(&lock, NULL) != 0) {

		printf("\n mutex init failed\n");
		return 1;
	}


    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, (void *)i);
        // Put thread to sleep for 1 second
	    sleep(1);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	pthread_mutex_destroy(&lock);			

    return 0;
}
