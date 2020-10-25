#include "func.c"

int main(int argc,char *argv[])
{

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

      // n = size of array
   if (argc < 2) {
      printf("Not enough arguments to program.\n");
      MPI_Finalize();
      return 0;
   } else if (argc == 2) {
      n = atoi(argv[1]); // Size of array
      if (n % p != 0) {
         printf("Invalid matrix size for %d processes\n", p);
         MPI_Finalize();
         return 0;
      }
   }

   GenerateArray();
   NaiveAllReduce();

   printf("rank %d answer %d", rank, x);

   MPI_Finalize();

}