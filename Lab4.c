/*
Semester: FALL 2018
Lab: 4
Name: Terry-Ann Sneed

Use MPI to implement a parallel version of the trapezoidal
rule, with process 0 sending user inputs with a single message
to all processes.
*/

#include <stdio.h>
#include <mpi.h>

/* Get the input values */
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
      int* n_p);

/* Calculate local integral  */
double Trap(double left_endpt, double right_endpt, int trap_count,
   double base_len);

/* Build derived datatype */
void Build_mpi_type(double* a_p, double* b_p, int* n_p,
     MPI_Datatype* input_mpi_t_p);

/* Call to function we're integrating */
double f(double x);

int main(void) {

   int my_rank, comm_sz, n, local_n;
   double a, b, h, local_a, local_b;
   double local_int, total_int;
   MPI_Datatype input_mpi;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   /* Call to get input from user function */
   Get_input(my_rank, comm_sz, &a, &b, &n);

   /* Call to build derived dataype function */
   Build_mpi_type(&a, &b, &n, &input_mpi);

   h = (b-a)/n;          /* h is the same for all processes */
   local_n = n/comm_sz;  /* So is the number of trapezoids  */

   /* Length of each process' interval of
    * integration = local_n*h.  */
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   /* Add up the integrals calculated by each process */
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0,
         MPI_COMM_WORLD);

   /* Print the result */
   if (my_rank == 0) {

      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n", a, b, total_int);

   }

   MPI_Finalize();

   return 0;

}

/* Build derived datatype function from user input*/
void Build_mpi_type(double* a_p, double* b_p, int* n_p,
  MPI_Datatype* input_mpi_t_p){

    int array_of_blocklengths[3] = {1, 1, 1};

    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Aint a_addr, b_addr, n_addr;
    MPI_Aint array_of_displacements[3] = {0};

    /* Return address of memory locations for inputs */
    MPI_Get_address(a_p, &a_addr);
    MPI_Get_address(b_p, &b_addr);
    MPI_Get_address(n_p, &n_addr);

    /* Byte displacements from start of (message + 1) */
    array_of_displacements[1] = b_addr - a_addr;
    array_of_displacements[2] = n_addr - a_addr;

    /*Build derived datatype*/
    MPI_Type_create_struct(3, array_of_blocklengths, array_of_displacements,
      array_of_types, input_mpi_t_p);

    /* Commit to optimize internal representation of derived datatype*/
    MPI_Type_commit(input_mpi_t_p);

  }

/* Get user input function */
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
  int* n_p) {

   /* MPI derived datatype */
   MPI_Datatype input_mpi_t;

   /* Call to build derived datatype function */
   Build_mpi_type(a_p, b_p, n_p, &input_mpi_t);

   if (my_rank == 0) {

      printf("Enter a, b, and n\n");
      scanf("%lf %lf %d", a_p, b_p, n_p);

   }

   /* MPI broadcast function of derived datatype */
   MPI_Bcast(a_p, 1, input_mpi_t, 0, MPI_COMM_WORLD);

   /* Free storage used */
   MPI_Type_free(&input_mpi_t);

}

/* Estimating of definite integral using the trapezoidal rule */
double Trap(double left_endpt, double right_endpt, int trap_count,
    double base_len) {


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

   return x*x;

}
