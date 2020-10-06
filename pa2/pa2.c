#include "helper.c"

int main(int argc,char *argv[])
{

   int rank, p, rseed;
   //int bp = 93563;
   int bp = 7;
   int **work;
   
   int n = 7;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   int rows = n;
   int cols = n / p;

   work = malloc(rows * sizeof *work);
   for(int i = 0; i < rows; i++)
   {
      work[i] = malloc(cols * sizeof *work[i]);
   }

   GenerateInitialGoL(p, bp, rank, n, work, rows, cols);

   printShare(work, rows, cols, rank);

   MPI_Finalize();
}
