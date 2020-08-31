#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
static long N = 100000000;
int main(int argc, char* argv[])
{

int my_rank;
int p;
MPI_Init(&argc, &argv); // Initializes the communicator
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // MPI_COMM_WORLD is your default communicator
MPI_Comm_size(MPI_COMM_WORLD, &p);



double sum = 0.0;
double globalSum = 0.0;
double piVal;
struct timespec start, end;
double time_taken;
 // Get current clock time.

long i, j;
clock_gettime(CLOCK_MONOTONIC, &start);	
long rpt = N / p;                      // rpt = rows per thread (in this e.g, row1 = 20M, rpt = 20M/6 ~ 3.33M
long rptr = N % p;                              // rpt = rows per thread remainder
	
long sp = my_rank * rpt;                        // e.g., my_rank = 1, sp = 3.3M,
long ep = sp + rpt;                           // ep = 6.6M
if(my_rank == p-1)
	ep += rptr;
for(i = sp; i< ep; i++)
{
    sum += 4.0 / (1 + pow((2.0 * i + 1.0)/(2.0 * N), 2));
}
MPI_Reduce(&sum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
piVal = globalSum / (double)N;
// Get the clock current time again

// Subtract end from start to get the CPU time used.
clock_gettime(CLOCK_MONOTONIC, &end);
time_taken = (end.tv_sec - start.tv_sec) * 1e9;
 time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) *
1e-9;
printf("Calculated Pi value (Serial-AlgoI) = %12.9f\n",piVal);
printf("Overall time (s): %lf\n", time_taken); // ts
return 0;
}
