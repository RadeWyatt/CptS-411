#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>

int p, n, rank, rows, cols, **work, **temp;
int g = 10;

// Initialization
void GenerateInitialGoL(int bp, int **section);

// Communications
void sendRecvRows(int *prev, int *post);
void getRowsFromNeighbors(int *prev, int *post);
void sendFwd();
void sendBack();

// Game logic
int* generateNeighborList(int x, int y, int *prev, int *post);
int DetermineState(int x, int y, int *prev, int *post);
void DetermineGameState(int *prev, int *post);

// Output
void printShare();
void DisplayGoL();
void DisplayTimingStats(int runtime, int commtime);

#endif