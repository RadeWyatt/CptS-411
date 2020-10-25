#include "header.h"

void GenerateArray()
{
    A = malloc(sizeof(int) * (n/p));
    srand(time(NULL));
    for (int i = 0; i < n/p; i++)
    {
        A[i] = rand() % 10;
    }
}

void NaiveAllReduce()
{
    x = 0;
    int temp;
    for (int i = 0; i < n/p; i++)
    {
        x += A[i];
    }
    for(int t = 0; t < p; t++)
    {
        if(rank == t && rank != p-1)
        {
            MPI_Send(&x, sizeof(int), MPI_INT, t + 1, 0, MPI_COMM_WORLD);
        }
        else if(rank == t+1)
        {
            MPI_Recv(&temp, sizeof(int), MPI_INT, t-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            x += temp;
        }
    }
    for(int t = p-1; t > 0; t++)
    {
        if(rank == t && rank != 0)
        {
            MPI_Send(&x, sizeof(int), MPI_INT, t - 1, 0, MPI_COMM_WORLD);
        }
        else if(rank == t-1)
        {
            MPI_Recv(&x, sizeof(int), MPI_INT, t+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }
}