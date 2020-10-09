#include "game.c"

int main(int argc,char *argv[])
{

   int rseed;
   int bp = 93563;

   struct timeval totalRuntimeStart, totalRuntimeEnd;
   struct timeval totalCommTimeStart, totalCommTimeEnd;
   
   // n x n  is is the size of the grid.  
   n = 80;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   int rows = n / p;

   // p is the number of processes being used. n must always be divisible by p.
   int cols = n;

   work = malloc(rows * sizeof *work);
   for(int i = 0; i < rows; i++)
   {
      work[i] = malloc(cols * sizeof *work[i]);
   }

   int totalRuntime, totalCommTimes = 0;

   GenerateInitialGoL(bp, work);

   int *prev, *post;

   gettimeofday(&totalRuntimeStart,NULL);
   for (int i = 0; i < g; i++) {
      prev = malloc(sizeof(int)*n);
      post = malloc(sizeof(int)*n);

      gettimeofday(&totalCommTimeStart, NULL);
      sendRecvRows(prev, post);
      gettimeofday(&totalCommTimeEnd, NULL);
      totalCommTimes += (totalCommTimeEnd.tv_sec-totalCommTimeStart.tv_sec)*1000000 + (totalCommTimeEnd.tv_usec-totalCommTimeStart.tv_usec);

      DetermineGameState(prev, post);

      free(prev);
      free(post);
   }
   gettimeofday(&totalRuntimeEnd,NULL);
   totalRuntime = (totalRuntimeEnd.tv_sec-totalRuntimeStart.tv_sec)*1000000 + (totalRuntimeEnd.tv_usec-totalRuntimeStart.tv_usec);

   DisplayTimingStats(totalRuntime, totalCommTimes);

   // DisplayGoL();

   MPI_Finalize();
}
