% Semester: Fall 2018
% Name: Terry-Ann Sneed
% Lab #5 PART I

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_THREADS	4

// Thread Structure accessible to all threads
struct thread_data {

	int thread_id;
	int limit;
};

// Thread array
struct thread_data thread_data_array[NUM_THREADS];

// Function to be executed by threads
void *BusyWork(void *threadarg)
{
   long i, tid, task_id;
   int limit;
   long t;
   double result=0.0;
   // ointer to thread structure
   struct thread_data *my_data;
   sleep(2);
   my_data = (struct thread_data *) threadarg;
   // Retrieving of structure data
   task_id = my_data->thread_id;
   limit = my_data->limit;
   
   
   printf("Thread %ld starting...\n",task_id);
   for (i=0; i<limit; i++) {
     result = result + sin(i) * tan(i);
     }
   printf("Thread %ld done. Result = %e\n",task_id, result);
}

int main(int argc, char *argv[])
{
pthread_t thread[NUM_THREADS];
int rc; 
long t;
int this_limit = 1000000 - 100;

for(t=0;t<NUM_THREADS;t++) {
   printf("Main: creating thread %ld\n", t);
   thread_data_array[t].thread_id = t;
   this_limit = this_limit + 100;
   thread_data_array[t].limit = this_limit;
   rc = pthread_create(&thread[t], NULL, BusyWork, (void *)&thread_data_array[t]); 
  
   if (rc) {
     printf("ERROR; return code from pthread_create() is %d\n", rc);
     exit(-1);
     }
  }

printf("Main: program completed. Exiting.\n");

pthread_exit(NULL);

}

