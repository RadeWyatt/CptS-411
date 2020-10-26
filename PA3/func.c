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
    int temp = x;
    int recvtemp, partner;
    g = 0;
    for (int t = 0; t < log2(p); t++)
    {
        partner = determinePartner(t);
        MPI_Sendrecv(&temp, 1, MPI_INT, partner, 0, &recvtemp, 1, MPI_INT, partner, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        temp += recvtemp;
    }
    g = temp;
}

int determinePartner(int t)
{
    return power(2, t) ^ rank;
}

// power function that works for type int. 
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}