/*
Lab 7
Fall 2018
Name: Terry-Ann SNEED
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define OPENMP 1

int random_f();

int main(int argc, char* argv[]) {

//int i
int input, sum = 0;
int thread_count;

srand((unsigned) time(NULL));

// User input 
if (argv[1] == NULL)
{
	printf("Please enter a number: ");
	scanf("%d", &input);
	printf("\n");
// Input validation
	while (input < 1) {
		printf("Please enter a positive number, greater than 0: ");
		scanf("%d", &input);
		printf("\n");
	}
}

#if OPENMP
	int my_rank = __builtin_omp_get_thread_num();
	if (argv[1] == NULL)
		thread_count = input;
	else
		thread_count = strtol(argv[1], NULL, 10);
	//int thread_count = input; 
#else     
    int my_rank = 0;
	int thread_count = 1;
#endif

printf("Thread count = %d\n\n------------------------------\n" ,thread_count);

// Sum of all generated numbers
#pragma omp parallel num_threads(thread_count) reduction(+: sum)
	//for (i = 0; i < input ; i++) {
//#pragma omp critical
 		sum += random_f();
	//}

printf("Sum is %d, collected by Thread %d\n", sum, my_rank);

return 0;

}

int random_f() {
	
	// Generate random number between 0 and 100
	
	int x = rand() % 100;

	int my_rank = omp_get_thread_num();

	printf("Thread %d generated number %d\n\n", my_rank, x);

//	printf("\n");

	return x;
	
}
