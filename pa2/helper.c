#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void GenerateInitialGoL(int p, int bp, int rank, int n, int **section, int rows, int cols)
{
    MPI_Status status;
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

void printShare(int **arr, int rows, int cols, int rank)
{
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