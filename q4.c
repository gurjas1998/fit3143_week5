#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	int i;
	double d;
	char buffer[100];
	int position = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(size < 2)
	{
		printf("Please run with 2 processes.\n");
		fflush(stdout);
		MPI_Finalize();
		return 0;
	}
    do {
	if(rank == 0)
	    {
		    printf("\nEnter a round number (>=0) and a real number: ");
		    fflush(stdout);
		    scanf("%d%lf", &i, &d);
		    MPI_Pack(&i, 1, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
		    MPI_Pack(&d, 1, MPI_DOUBLE, buffer, 100, &position, MPI_COMM_WORLD);
		    //MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
		    
	    }
        MPI_Bcast(buffer, 100, MPI_CHAR, 0, MPI_COMM_WORLD); //BCast is just a wrapper for SEND and RECV
	   
	if(rank > 0)
	    {
		    //MPI_Recv(buffer, 100, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
		    MPI_Unpack(buffer, 100, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
		    MPI_Unpack(buffer, 100, &position, &d, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		    printf("\nRank: %d: Received round number: %d and real number: %lf", rank, i, d);
		    fflush(stdout);
	    }
	    
	}while(i>=0);

	MPI_Finalize();
	return 0;
}
