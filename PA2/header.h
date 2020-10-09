#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <mpi.h>
#include <time.h>

int p, n, rank, rows, cols;
bool **work, **temp;
int g = 10;

// Initialization
void GenerateInitialGoL(int bp, bool **section);

// Communications
void sendRecvRows(bool *prev, bool *post);
void getRowsFromNeighbors(bool *prev, bool *post);
void sendFwd();
void sendBack();

// Game logic
bool* generateNeighborList(int x, int y, bool *prev, bool *post);
int DetermineState(int x, int y, bool *prev, bool *post);
void DetermineGameState(bool *prev, bool *post);

// Output
void printShare();
void DisplayGoL();
void DisplayTimingStats(int runtime, int commtime);

#endif