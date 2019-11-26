/*
Semester: Fall 2018
Lab: 2 Part II
Name: Terry-Ann Sneed
*/

#include <stdio.h>
#include <mpi.h>

/* Calculate value of pi *?
/* This is the same method as Part I, except the function and boudaries of integration differ */
/* Higher values of n yield more accurate results */
void Get_input(int my_rank, int comm_sz, int *n_p);
double Trap_for_pi(double left_endpt, double right_endpt, int trap_count, double base_len);
double f(double x);

int main(void) {

   int my_rank, comm_sz, n, local_n;
   double a, b, h, local_a, local_b;
   double local_int, total_int;

   /* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   /* Get input from user */
   Get_input(my_rank, comm_sz, &n);

   a = 0.0;
   b = 1.0;
   h = 1 / (double) n;          /* h is the same for all processes */
   local_n = n / comm_sz;  /* So is the number of trapezoids  */

   /* Length of each process' interval of integration  */
   local_a = a + my_rank * local_n * h;
   local_b = local_a + local_n * h;
   local_int = Trap_for_pi(local_a, local_b, local_n, h);

   /* Local sums are added to total sum, including processor 0 */
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   /* Print result */
   if (my_rank == 0) {

     printf("With n = %d trapezoids, our estimate\n", n);
     printf("of the integral from %f to %f = %.15e\n", a, b, total_int);

  }

   /* Shut down MPI */
   MPI_Finalize();

   return 0;

}


/* Get input from user function */
void Get_input(int my_rank, int comm_sz, int* n_p) {

  if (my_rank == 0) {

    printf("Enter n: \n");
    scanf("%d", n_p);

  }

  /* Send user input values to all processors*/
	MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

/* Trapezoidal rule estimate of integral function */
double Trap_for_pi( double left_endpt, double right_endpt, int trap_count, double base_len) {


  double estimate, x;
  int i;

  estimate = (f(left_endpt) + f(right_endpt))/2.0;

  for (i = 1; i <= trap_count-1; i++) {

    x = left_endpt + i*base_len;
    estimate += f(x);

  }

  estimate = estimate*base_len;

  return estimate;

}

/* Compute value of function to be integrated */
double f(double x) {

   return 4 / (1 + x * x);

}
