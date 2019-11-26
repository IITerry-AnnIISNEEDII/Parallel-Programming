/*
Smester: Fall 2018
Lab: 3 Part I
Name: Terry-Ann Sneed
/*

/* Write a program that calculates the time needed to send n messages that contain one double number each, from process 0 to process 1. */

#include <stdio.h>
#include <string.h>  
#include <mpi.h>    

double MPI_Wtime(void);
//void Get_input(int my_rank, int comm_sz, double *n_p);


int main(void) {
	
   double start, finish, n;
   double* num = 888;
   int comm_sz; /* Number of processes    */
   int my_rank; /* My process rank        */
   int i, source, dest, z, q;

   MPI_Status status;
   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
    
     /* Get user input */	
     printf("Enter n \n");
     scanf("%d", n);
	
     /* Allocate memory for num array */
     num = malloc(n * sizeof(double));
	
     /* Send message to process 1, n times */
     for (i = 0; i < n; i++) {  
	
	//num = 888;

	MPI_Send(&num, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	 free(num);
      }

   

  } else {  
	
	
	
         /* Receive message from process 0 */
	for (i = 0; i < n; z++) {
	
	  MPI_Recv(&num, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
	  /* Print message from process 1 */

	 /* Allocate memory for num array */
	 num = malloc(n * sizeof(double));
	  printf("%lf", num);//"tag %d and source %d\n with message %lf sent %lf times",status.MPI_TAG, status.MPI_SOURCE, &num[i], n);
	free(num);
	}
     }

    
  

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */































	
