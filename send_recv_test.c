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
#include <assert.h>
#include <sys/time.h>
#include <mpi.h>
#include <string.h>


const int numIterations = 25;

// power function that works for type int. 
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}

void printArray(int *send, int *rec, int rank, int blocking) {
	int i = 0;
	int *arr = NULL;

	if (rank == 1) {
		arr = send;
		if (blocking)
			printf("\n\n%28s\n", "Blocking Send Statistics");
		else 
			printf("\n\n%28s\n", "Non-Blocking Send Statistics");
	} else {
		arr = rec;
		if (blocking)
			printf("\n\n%29s\n", "Blocking Receive Statistics");
		else 
			printf("\n\n%29s\n", "Non-Blocking Receive Statistics");
	}

	printf("%10s%15s%15s\n", "# Bytes", "# Microsec", "# Millisec");
	while (i < numIterations) {
		int msgSize = sizeof(int) * power(2, i);
		printf("%10d%15d%15d\n", msgSize, arr[i], (arr[i]/1000));
		i++;
	}
}

int main(int argc,char *argv[])
{
	int rank,p, msgSize;
	struct timeval t1,t2;
	int *y, *x;

	int blockSendTime[numIterations], blockRecTime[numIterations];
	int nonBlockSendTime[numIterations], nonBlockRecTime[numIterations];

	memset(blockSendTime, 0, numIterations);
	memset(blockRecTime, 0, numIterations);
	memset(nonBlockRecTime, 0, numIterations);
	memset(nonBlockSendTime, 0, numIterations);

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	assert(p>=2);

	for(int j = 0; j < numIterations; j++)
	{
		int numInts = power(2, j);
		int msgSize = sizeof(int) * numInts;
		if(rank==1) {
			for (int k = 0; k < 10; k++) {
				x = (int *)calloc(numInts, sizeof(int));
				int dest = 0;
				gettimeofday(&t1,NULL);
				MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
				gettimeofday(&t2,NULL);
				int sendMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				blockSendTime[j] = (blockSendTime[j] + sendMicro) / 2;
			}
		} else if (rank==0) {
			for (int k = 0; k < 10; k++) {
				MPI_Status status;
				y = (int *)calloc(numInts, sizeof(int));
				gettimeofday(&t1,NULL);
				MPI_Recv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				gettimeofday(&t2,NULL);
				int recMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				blockRecTime[j] = (blockRecTime[j] + recMicro) / 2;
			}
		}
	}

	for(int j = 0; j < numIterations; j++)
	{
		int numInts = power(2, j);
		int msgSize = sizeof(int) * numInts;
		if(rank==1) {
			for (int k = 0; k < 10; k++) {
				x = (int *)calloc(numInts, sizeof(int));
				int dest = 0;
				gettimeofday(&t1,NULL);
				MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
				gettimeofday(&t2,NULL);
				int sendMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				nonBlockSendTime[j] = sendMicro;
			}
		} else if (rank==0) {
			MPI_Request request1;
			for (int k = 0; k < 10; k++) {
				MPI_Status status1;
				y = (int *)calloc(numInts, sizeof(int));
				gettimeofday(&t1,NULL);
				MPI_Irecv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&request1);
				MPI_Wait(&request1, &status1);
				gettimeofday(&t2,NULL);
				int recMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				nonBlockRecTime[j] = recMicro;
			}
		}
	}
	MPI_Finalize();

	printArray(blockSendTime, blockRecTime, rank, 1);
	printArray(nonBlockSendTime, nonBlockRecTime, rank, 0);
}
