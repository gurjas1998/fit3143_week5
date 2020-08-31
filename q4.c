#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	int i;
	double d;
	char buffer[100]; //used 100 as stated in lab instructions
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
		    scanf("%d%lf", &i, &d); //round number stored in i and real number stored in d for Processor 0's copy of the 'code'
		    MPI_Pack(&i, 1, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD); //round number packed into buffer starting from position 0 of buffer 
										      //MPI_Pack should determine the next position and store it back into variable position
		    MPI_Pack(&d, 1, MPI_DOUBLE, buffer, 100, &position, MPI_COMM_WORLD); //Uses newly determined position to pack the real number into the buffer 
		    //MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
		    
	    }
        MPI_Bcast(buffer, 100, MPI_CHAR, 0, MPI_COMM_WORLD); //BCast is just a wrapper for SEND and RECV
	    						     //Broadcast is sent from Processor 0, which is the buffer array containing the data we want to send
	   
	if(rank > 0) //For all processors rank > 0, receive the data and unpack it, store it in their own copies of variables i and d
	    {
		    //MPI_Recv(buffer, 100, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
		    MPI_Unpack(buffer, 100, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
		    MPI_Unpack(buffer, 100, &position, &d, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		    printf("\nRank: %d: Received round number: %d and real number: %lf", rank, i, d);
		    fflush(stdout);
	    }
	    
	}while(i>=0); //Keep prompting user for input until they enter a negative round number

	MPI_Finalize();
	return 0;
}
