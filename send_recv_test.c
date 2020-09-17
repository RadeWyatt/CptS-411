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


const int numIterations = 25;

// power function that works for type int. 
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}

void printArray(int *send, int *rec, int rank) {
	int i = 0;
	int *arr = NULL;

	if (rank == 1) {
		arr = send;
		printf("\n\n%28s\n", "Send Statistics");
	} else {
		arr = rec;
		printf("\n\n%29s\n", "Receive Statistics");
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

	int sendTime[numIterations], recTime[numIterations];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	assert(p>=2);

	for(int j = 0; j < numIterations; j++)
	{
		int numInts = power(2, j);
		int msgSize = sizeof(int) * numInts;
		if(rank==1) {
			x = (int *)calloc(numInts, sizeof(int));
			int dest = 0;
			gettimeofday(&t1,NULL);
			MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
			gettimeofday(&t2,NULL);
			int sendMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
			sendTime[j] = sendMicro;
		} else if (rank==0) {
			MPI_Status status;
			y = (int *)malloc(msgSize);
			gettimeofday(&t1,NULL);
			MPI_Recv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			gettimeofday(&t2,NULL);
			int recMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
			recTime[j] = recMicro;
		}
	}
	MPI_Finalize();
	printArray(sendTime, recTime, rank);
}
