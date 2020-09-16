/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
 *	Example code
 *	Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
 * */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <assert.h>
#include <sys/time.h>

//const int numInts = 1000000;
//const int msgSize = sizeof(int) * numInts;

int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
 }

 char* generateString(int numBytes) {
	 return (char *)malloc(numBytes);
 }

int main(int argc,char *argv[])
{
	int rank,p, msgSize;
	struct timeval t1,t2;
	int *y, *x;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	printf("my rank=%d\n",rank);
	printf("Rank=%d: number of processes =%d\n",rank,p);

	assert(p>=2);

	for(int numInts = 10; numInts < 100000; i*=10)
	{
		if(rank==1) {
			msgSize = sizeof(int) * numInts;
			*x = (int *)calloc(msgSize, sizeof(int));
			int dest = 0;
			gettimeofday(&t1,NULL);
			MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
			gettimeofday(&t2,NULL);
			int tSend = (t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000;
			printf("Rank=%d: sent message to rank %d; size: %d bytes; Send time %d millisec\n", rank,dest,msgSize,tSend);
		} else if (rank==0) {
			MPI_Status status;
			*y = (int *)malloc(sizeof(int) * numInts);
			gettimeofday(&t1,NULL);
			MPI_Recv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			gettimeofday(&t2,NULL);
			int tRecv = (t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000;
			printf("Rank=%d: received message %d from rank %d; Recv time %d millisec\n",rank, y[0],status.MPI_SOURCE,tRecv);
		}
	}

	MPI_Finalize();
}
