#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void GenerateInitialGoL(int p, int bp, int rank, int n, int **section)
{
    MPI_Status status;
    int randSeed;
    int bp2 = bp*bp;
    srand(time(NULL));
    if(rank == 0){
        for (int i = 1; i < p; p++)
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
    section = malloc(sizeof(int*) * (n/p));
    for(int i = 0; i < n/p; i++)
    {
        section[i] = malloc(sizeof(int) * n);
        for(int j = 0; j < n; j++)
        {
            section[i][j] = rand() % 2;
        }
    }
}

void printShare(int **arr, int n, int p)
{
    for(int i = 0; i < n/p; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}