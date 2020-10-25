#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>

int p, n, *A, x, rank, g;
MPI_Status status;

void MyAllReduce();
void NaiveAllReduce();
void MPILibraryAllReduce();
void GenerateArray();
void AddLocal();

#endif