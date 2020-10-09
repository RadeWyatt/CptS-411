#include "header.h"

void GenerateInitialGoL(int bp, int **section)
{
    MPI_Status status;
    rows = n/p;
    cols = n;
    int randSeed;
    int bp2 = bp*bp;
    srand(time(NULL));
    if(rank == 0){
        for (int i = 0; i < p; i++)
        {
            randSeed = rand() % bp2 + 1;
            MPI_Send(&randSeed, sizeof(int), MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } 
    MPI_Recv(&randSeed, sizeof(int),MPI_INT, 0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

    srand(randSeed);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            section[i][j] = rand() % 2;
        }
    }
}

/*
    Send and recieve rows necessary for state computation. 
*/
void sendRecvRows(int *prev, int *post)
{
    for(int t = 0; t < p; t++)
    {   
        if (t == rank && rank == 0 && p == 1) {
            sendBack();
            sendFwd();
            getRowsFromNeighbors(prev, post);
        } 
        if (rank == t-1 || (t == 0 && rank == p-1)) {
            sendFwd();
        } 
        if (rank == t+1 || (t==p-1 && rank == 0)) {
            sendBack();
        } 
        if (t == rank) {
            getRowsFromNeighbors(prev, post);
        }
    }
}

/*
    Recieve both rows of matrix necessary for state computation for a single process.
*/
void getRowsFromNeighbors(int *prev, int *post)
{
    int back, front;
    back = rank - 1;
    front = rank + 1;
    MPI_Status status;

    if (rank == 0 && p == 1) {
        front = back = 0;
    }
    else if (rank == 0)
    {
        back = p - 1;
    }
    else if (rank == p-1)
    {
        front = 0;
    }

    MPI_Recv(prev, sizeof(int)*n, MPI_INT, back, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(post, sizeof(int)*n, MPI_INT, front, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}

/*
    Send the last row of the matrix to the next process.
*/
void sendFwd()
{
    int *info = work[n/p - 1];
    if (rank == p-1)
    {
		MPI_Send(info, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
		MPI_Send(info, n, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    }
}


/*
    Send the first row of the matrix to previous process.
*/
void sendBack()
{
    int *info = work[0];
    if (rank == 0)
    {
		MPI_Send(info, n, MPI_INT, p-1, 0, MPI_COMM_WORLD);
    }
    else
    {
		MPI_Send(info, n, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
    }
}

/*
    generates 1D array of neighbors alive/dead statuses, indexes are like so:
    0 1 2
    3 x 4
    5 6 7
    x = row, y = col
*/
int *generateNeighborList(int x, int y, int *prev, int *post) {
    int *neighbors = malloc(sizeof(int)*8);
    // Check boundaries.
    if(x == 0 == rows - 1)
    {
        if (y > 0 && y < cols-1)
        { 
            neighbors[0] = prev[y-1];
            neighbors[1] = prev[y];
            neighbors[2] = prev[y+1];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = post[y-1];
            neighbors[6] = post[y];
            neighbors[7] = post[y+1];
        }
        else if(y == 0)
        {
            neighbors[0] = prev[cols-1];
            neighbors[1] = prev[y];
            neighbors[2] = prev[y+1];
            neighbors[3] = work[x][cols-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = post[cols-1];
            neighbors[6] = post[y];
            neighbors[7] = post[y+1];
        }
        else{
            neighbors[0] = prev[y-1];
            neighbors[1] = prev[y];
            neighbors[2] = prev[0];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][0];
            neighbors[5] = post[y-1];
            neighbors[6] = post[y];
            neighbors[7] = post[0];
        }
    }
    else if (x > 0 && x < rows-1) {
        neighbors[1] = work[x-1][y];
        neighbors[6] = work[x+1][y];
        if (y > 0 && y < cols-1) {
            // Normal neighbor list. All belong to this process.
            neighbors[0] = work[x-1][y-1];
            neighbors[2] = work[x-1][y+1];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = work[x+1][y-1];
            neighbors[7] = work[x+1][y+1];
        } else if (y == 0) {
            neighbors[0] = work[x-1][cols-1];
            neighbors[2] = work[x-1][y+1];
            neighbors[3] = work[x][cols-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = work[x+1][cols-1];
            neighbors[7] = work[x+1][y+1];
        } else {
            neighbors[0] = work[x-1][y-1];
            neighbors[2] = work[x-1][0];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][0];
            neighbors[5] = work[x+1][y-1];
            neighbors[7] = work[x+1][0];
        }
    } else if (x == 0) {
        neighbors[1] = prev[y];
        neighbors[6] = work[x+1][y];
        if (y > 0 && y < cols-1) {
            neighbors[0] = prev[y-1];
            neighbors[2] = prev[y+1];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = work[x+1][y-1];
            neighbors[7] = work[x+1][y+1];
        } else if (y == 0) {
            neighbors[0] = prev[cols-1];
            neighbors[2] = prev[y+1];
            neighbors[3] = work[x][cols-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = work[x+1][cols-1];
            neighbors[7] = work[x+1][y+1];
        } else {
            neighbors[0] = prev[y-1];
            neighbors[2] = prev[0];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][0];
            neighbors[5] = work[x+1][y-1];
            neighbors[7] = work[x+1][0];
        }
    } else {
        neighbors[1] = work[x-1][y];
        neighbors[6] = post[y];
        if (y > 0 && y < cols-1) {
            neighbors[0] = work[x-1][y-1];
            neighbors[2] = work[x-1][y+1];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = post[y-1];
            neighbors[7] = post[y+1];
        } else if (y == 0) {
            neighbors[0] = work[x-1][cols-1];
            neighbors[2] = work[x-1][y+1];
            neighbors[3] = work[x][cols-1];
            neighbors[4] = work[x][y+1];
            neighbors[5] = post[cols-1];
            neighbors[7] = post[y+1];
        } else {
            neighbors[0] = work[x-1][y-1];
            neighbors[2] = work[x-1][0];
            neighbors[3] = work[x][y-1];
            neighbors[4] = work[x][0];
            neighbors[5] = post[y-1];
            neighbors[7] = post[0];
        }
    }
    return neighbors;
}

int DetermineState(int x, int y, int *prev, int *post) {
    int *neighbors = generateNeighborList(x,y,prev,post);
    int count = 0;
    for (int i = 0; i < 8; i++) {
        count += neighbors[i];
    }
    if (count < 3 || count > 5) {
        return 0;
    } else {
        return 1;
    }
}

void DetermineGameState(int *prev, int *post) {
   temp = malloc(rows * sizeof *temp);
   for(int i = 0; i < rows; i++)
   {
      temp[i] = malloc(cols * sizeof *temp[i]);
   }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            temp[i][j] = DetermineState(i, j, prev, post);
        }
    }
    work = temp;
}

/*
    Prints the work matrix. 
    "work" is the subsection of the game board that a single
    process is responsible for. 
*/
void printShare()
{
    printf("RANK: %d\n", rank);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d ", work[i][j]);
        }
        printf("\n");
    }
}

void DisplayGoL() {
    // Initialize whole matrix
    if(rank == 0){
        MPI_Status status;
        int **GoL = malloc(n * sizeof *GoL);
        for(int i = 0; i < n; i++)
        {
            GoL[i] = malloc(n * sizeof *GoL[i]);
        }

        for (int i = 0; i < rows; i++) 
            for (int j = 0; j < cols; j++)
                GoL[i][j] = work[i][j];

        for (int t = 1; t < p; t++)
        {
            MPI_Recv(&(GoL[rows*t][0]), n*n/p, MPI_INT, t, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", GoL[i][j]);
            }
            printf("\n");
        }
    }
    else{
        for(int t = 1; t < p; t++)
        {
            if (rank == t)
            {
		        MPI_Send(&(work[0][0]), n*n/p, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
}

void DisplayTimingStats(int runtime, int commtime) {
    int avgRuntime = runtime / g;
    int totalCompTime = runtime - commtime;
    printf("RANK= %d | TOTAL RUNTIME: %d \xC2\xB5s | AVG RUNTIME: %d \xC2\xB5s | TOTAL COMM TIME: %d \xC2\xB5s | TOTAL COMPUTATION TIME: %d \xC2\xB5s\n", rank, runtime, avgRuntime, commtime, totalCompTime);
}