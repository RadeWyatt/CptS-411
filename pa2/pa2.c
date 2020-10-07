#include "helper.c"

int main(int argc,char *argv[])
{

   int rank, p, rseed;
   int bp = 93563;
   int **work;
   
   // n x n  is is the size of the grid.  
   int n = 70;
   // number of generations
   int g = 7;

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

   GenerateInitialGoL(p, bp, rank, work, rows, cols);

   //printShare(work, rows, cols, rank);

   MPI_Finalize();
}
