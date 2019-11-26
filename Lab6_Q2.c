/*
Semester: Fall 2018
Name: Terry-Ann Sneed
Lab: 6 Question II 
Goal: "Modify the code so as to not have any errors at the output." 
*/


#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 6
#define NEXT(x) ((x + 1) % N)

pthread_t tid[3];
int buffer[N], in=0, out=0, count=0;
sem_t mutex, items, space;

void* producer(void *ptr)
{
	srand(time(NULL));
	int item;

	sem_wait(&space);
	sem_wait(&mutex);
	
  	while (1) {

		while (count == N)
			; // wait
		item = rand()%10;
		printf("Insert: %d\n",item);
		buffer[in] = item;
		sleep(rand()%5);
		in = NEXT(in);
		count++;
		
		sem_post(&mutex);
		sem_post(&items);	
		
		if (count>N)
			printf("Error!!!!!!!!!!!\n");
	}

}

void* consumer(void *ptr)
{
	srand(time(NULL));
	unsigned long item;

	sem_wait(&items);	
	sem_wait(&mutex);

    while (1) {

		while (count == 0)
			; // wait
		sleep(rand()%2);
		item = buffer[out];
		printf("Remove: %ld\n",item);
		out = NEXT(out);
		count--;
		sem_post(&mutex);
		sem_post(&space);
		
		if (count<0)
			printf("Error!!!!!!!!!!!\n");

		return (void*)item;
	}
}


int main(void)
{


    sem_init(&mutex, 0, 1);
    sem_init(&items, 0, 0);
    sem_init(&space, 0, N);



    pthread_create(&tid[0], NULL, consumer, NULL);
    pthread_create(&tid[1], NULL, consumer, NULL);
    pthread_create(&tid[2], NULL, producer, NULL);


    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    
    return 0;
}

