#include "game.c"

int main(int argc,char *argv[])
{
   int rseed;
   int bp = 93563;

   struct timeval totalRuntimeStart, totalRuntimeEnd;
   struct timeval totalCommTimeStart, totalCommTimeEnd;
   
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   // Validate arguments, g = # of generations && n = size of matrix
   if (argc < 2) {
      printf("Not enough arguments to program.\n");
      MPI_Finalize();
      return 0;
   } else if (argc == 2) {
      n = atoi(argv[1]); // Size of matrix.
      if (n % p != 0) {
         printf("Invalid matrix size for %d processes\n", p);
         MPI_Finalize();
         return 0;
      }
   }

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
      MPI_Barrier(MPI_COMM_WORLD);
      gettimeofday(&totalCommTimeEnd, NULL);
      totalCommTimes += (totalCommTimeEnd.tv_sec-totalCommTimeStart.tv_sec)*1000000 + (totalCommTimeEnd.tv_usec-totalCommTimeStart.tv_usec);

      DetermineGameState(prev, post);

      free(prev);
      free(post);
   }
   gettimeofday(&totalRuntimeEnd,NULL);
   totalRuntime = (totalRuntimeEnd.tv_sec-totalRuntimeStart.tv_sec)*1000000 + (totalRuntimeEnd.tv_usec-totalRuntimeStart.tv_usec);

   free(work);

   DisplayTimingStats(totalRuntime, totalCommTimes);

   MPI_Finalize();
}
