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

void AddLocal()
{
    x = 0;
    for (int i = 0; i < n/p; i++)
    {
        x += A[i];
    }
}

void NaiveAllReduce()
{
    g = 0;
    int temprecv, temp = x;
    for(int t = 0; t < p-1; t++)
    {
        if(rank == t)
        {
            MPI_Send(&temp, 1, MPI_INT, t + 1, 0, MPI_COMM_WORLD);
        }
        else if(rank == t+1)
        {
            MPI_Recv(&temprecv, 1, MPI_INT, t, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            temp += temprecv;
        }
    }
    g = temp;
    for(int t = p-1; t > 0; t--)
    {
        if(rank == t && rank != 0)
        {
            MPI_Send(&g, 1, MPI_INT, t - 1, 0, MPI_COMM_WORLD);
        }
        else if(rank == t-1)
        {
            MPI_Recv(&g, 1, MPI_INT, t, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }
}

void MPILibraryAllReduce()
{
    MPI_Allreduce(&x, &g, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
}

void MyAllReduce()
{
    
}