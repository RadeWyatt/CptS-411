#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>

void MyAllReduce();
void NaiveAllReduce();
void MPILibraryAllReduce();

#endif