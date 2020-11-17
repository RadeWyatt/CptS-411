#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

int p, n, *A, x, rank, g0, g1, g2;
int *naiveTimes, *myTimes, *libraryTimes;

MPI_Status status;

void MyAllReduce();
void NaiveAllReduce();
void MPILibraryAllReduce();
void GenerateArray();
void AddLocal();
int determinePartner(int t);
int power(int base, unsigned int exp);

#endif