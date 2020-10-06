#include <stdio.h>
#include <mpi.h>
#include <time.h>

void GenerateInitialGoL(int p, int bp, int rank, int n, int ***section)
{
    int randSeed;
    int bp2 = bp*bp;
    int cols = n/p;
    int div[cols][n];
    srand(time(NULL))
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
    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < n; j++)
        {
            div[i][j] = rand() % 2;
        }
    }
    section = div;
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