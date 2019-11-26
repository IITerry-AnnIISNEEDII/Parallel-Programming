/*
Semester: Fall 2018
Name: Terry-Ann Sneed
Lab: 6 Question III 
Goal: "Modify the code so as to not have any errors at the output." 
*/



/*
 *  Solution to Producer Consumer Problem
 *  Using Pthreads, a mutex and condition variables
 *  From Tanenbaum, Modern Operating Systems, 3rd Ed.
 */

/*
    In this version the buffer is a single number.
    The producer is putting numbers into the shared buffer
    (in this case sequentially)
    And the consumer is taking them out.
    If the buffer contains zero, that indicates that the buffer is empty.
    Any other value is valid.
*/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


#define MAX 10000000000			/* Numbers to produce */
pthread_mutex_t the_mutex;

int buffer = 0;

void* producer(void *ptr) {
  int i;

  srand(time(NULL));


  for (i = 1; i <= MAX; i++) {

    while (buffer != 0) {		  /* If there is something */
	;
    }
    buffer++;
    if (buffer>1)
	printf("Error !!!!!!!!!\n");
    printf("Producer buffer = %d\n",buffer);
    sleep(rand()%3);
  }
  pthread_exit(0);
}

void* consumer(void *ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    while (buffer == 0){			/* If there is nothing in */
	;
    }
    buffer--;
    if (buffer<0)
	printf("Error !!!!!!!!!\n");
    printf("Consumer buffer = %d\n",buffer);
    sleep(1);
  }
  pthread_exit(0);
}

int main(int argc, char **argv) {
  pthread_t pro, con, con2;

  // Create the threads
  pthread_create(&con, NULL, consumer, NULL);
  pthread_create(&con2, NULL, consumer, NULL);
  pthread_create(&pro, NULL, producer, NULL);

  // Wait for the threads to finish
  pthread_join(con, NULL);
  pthread_join(con2, NULL);
  pthread_join(pro, NULL);



}
