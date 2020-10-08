#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

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

#endif