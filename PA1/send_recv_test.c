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


const int numIterations = 20;

// power function that works for type int. 
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}

void printArray(int *arr) {
	int i = 0;
	printf("%10s%15s%15s\n", "# Bytes", "# Microsec", "# Millisec");
	while (i < numIterations) {
		printf("%10d%15d%15d\n", power(2, i) * 4, arr[i], (arr[i]/1000));
		i++;
	}
}

void dataHandler(int *blockSend, int *blockRec, int rank) {
	if (rank == 0) {
		printf("\n\n%29s\n", "Blocking Receive Statistics");
		printArray(blockRec);
	} else if (rank == 1) {
		printf("\n\n%28s\n", "Blocking Send Statistics");
		printArray(blockSend);
	} else if (rank == 2) {
		printf("\n\n%28s\n", "Non-Blocking Send Statistics");
		//printArray(nonBlockSend);
	} else if (rank == 3) {
		printf("\n\n%29s\n", "Non-Blocking Receive Statistics");
		//printArray(nonBlockRec);
	}
}

int main(int argc,char *argv[])
{
	int rank, p;

	struct timeval t1,t2,t3,t4;

	int blockSendTime[numIterations], blockRecTime[numIterations];
	int nonBlockSendTime[numIterations], nonBlockRecTime[numIterations];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	assert(p>=2);

	if (rank == 1) {
		int *x1;
		for (int i = 0; i < numIterations; i++) {
			x1 = (int *)calloc(power(2, i), sizeof(int));
			gettimeofday(&t1, NULL);
			MPI_Send(x1, power(2, i), MPI_INT, 0, 0, MPI_COMM_WORLD);
			gettimeofday(&t2, NULL);
			blockSendTime[i] += (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
			free(x1);
		}
	} else if (rank == 0) {
		int *y1;
		MPI_Status status;
		for (int i = 0; i < numIterations; i++) {
			y1 = (int *)calloc(power(2, i), sizeof(int));
			gettimeofday(&t1,NULL);
			MPI_Recv(y1, power(2, i),MPI_INT, 1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			gettimeofday(&t2,NULL);
			blockRecTime[i] += (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
		}
	} else if (rank == 2) {
		char *x2;
		for (uint i = 0; i < numIterations; i++) {
			x2 = (char *)calloc(power(2, i), sizeof(char));
			gettimeofday(&t3, NULL);
			MPI_Send(x2, power(2, i), MPI_CHAR, 3, 0, MPI_COMM_WORLD);
			gettimeofday(&t4, NULL);
			nonBlockSendTime[i] += (t4.tv_sec-t3.tv_sec)*1000000 + (t4.tv_usec-t3.tv_usec);
			free(x2);
		}
	} else if (rank == 3) {
		char *y2;
		for (uint i = 0; i < numIterations; i++) {
			MPI_Request request;
			MPI_Status status;
			y2 = (char *)calloc(power(2, i), sizeof(char));
			gettimeofday(&t3,NULL);
			MPI_Irecv(y2,power(2, i),MPI_CHAR, 2,MPI_ANY_TAG,MPI_COMM_WORLD,&request);
			MPI_Wait(&request, &status);
			gettimeofday(&t4,NULL);
			nonBlockRecTime[i] += (t4.tv_sec-t3.tv_sec)*1000000 + (t4.tv_usec-t3.tv_usec);
		}
	}

	MPI_Finalize();

	for (int i = 0; i < numIterations; i++) {
		if (rank == 1)
			blockSendTime[i] = blockSendTime[i] / numIterations;
		if (rank == 0)
			blockRecTime[i] = blockRecTime[i] / numIterations;
		if (rank == 2)
		nonBlockSendTime[i] = nonBlockSendTime[i] / numIterations;
		if (rank == 3)
			nonBlockRecTime[i] = nonBlockRecTime[i] / numIterations;
	}


	dataHandler(blockSendTime, blockRecTime, rank);
}
