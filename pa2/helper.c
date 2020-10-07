#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int p, n, rank, **work;

void GenerateInitialGoL(int bp, int **section)
{
    MPI_Status status;
    int rows = n/p;
    int cols = n;
    int randSeed;
    int bp2 = bp*bp;
    srand(time(NULL));
    if(rank == 0){
        for (int i = 1; i < p; i++)
        {
            randSeed = rand() % bp2 + 1;
            MPI_Send(&randSeed, sizeof(int), MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        randSeed = rand() % bp2 + 1;
    }
    else
    {
        MPI_Recv(&randSeed, sizeof(int),MPI_INT, 0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    }

    srand(randSeed);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            section[i][j] = rand() % 2;
        }
    }
}

void sendRecvRows()
{
    int *prev = malloc(sizeof(int)*n);
    int *post = malloc(sizeof(int)*n);
    for(int t - 0; t < p; t++)
    {
        if(t == rank)
        {
            getRowsFromNeighbors(prev, post);
        }
        else if(rank == t-1 || (t == 0 && rank == p-1))
        {
            sendFwd();
        }
        else if(rank == t+1 || (t==p-1 && rank == 0))
        {
            sendBack();
        }
    }
}

void generateNeighborList(int x, int y, int **list) {

}

void DetermineState(int x, int y) {

}

void getRowsFromNeighbors(int *prev, int *post)
{
    int back, front;
    back = rank - 1;
    front = rank + 1;
    MPI_Status status;
    if(rank == 0)
    {
        back = p - 1;
    }
    else if (rank == p-1)
    {
        front = 0;
    }
    MPI_Recv(prev, sizeof(int)*n, MPI_INT, back, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(post, sizeof(int)*n, MPI_INT, front, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}

void sendFwd()
{
    int *info = work[n/p];
    if (rank == p-1)
    {
		MPI_Send(info, sizeof(int)*n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
		MPI_Send(info, sizeof(int)*n, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    }
}

void sendBack()
{
    int *info = work[0]
    if (rank == 0)
    {
		MPI_Send(info, sizeof(int)*n, MPI_INT, p-1, 0, MPI_COMM_WORLD);
    }
    else
    {
		MPI_Send(info, sizeof(int)*n, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
    }
}

void printShare(int **arr)
{
    int cols = n;
    int rows = n/p;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("RANK: %d\n", rank);
}