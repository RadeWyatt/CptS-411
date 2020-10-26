#include "func.c"

int main(int argc,char *argv[])
{

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   struct timeval startNaive, startLibrary, startMine;
   struct timeval endNaive, endLibrary, endMine;
   int timeNaive, timeLibrary, timeMine;

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
   AddLocal();

   //time for naive reduce
   gettimeofday(&startNaive,NULL);
   NaiveAllReduce();
   gettimeofday(&endNaive,NULL);
   timeNaive = (endNaive.tv_sec-startNaive.tv_sec)*1000000 + (endNaive.tv_usec-startNaive.tv_usec);
   printf("rank %d answer %d time %d naive\n", rank, g, timeNaive);

   //time for library all reduce
   gettimeofday(&startLibrary,NULL);
   MPILibraryAllReduce();
   gettimeofday(&endLibrary, NULL);
   timeLibrary = (endLibrary.tv_sec-startLibrary.tv_sec)*1000000 + (endLibrary.tv_usec-startLibrary.tv_usec);
   printf("rank %d answer %d time %d library\n", rank, g, timeLibrary);

   //time for my all reduce
   gettimeofday(&startMine,NULL);
   MyAllReduce();
   gettimeofday(&endMine,NULL);
   timeMine = (endMine.tv_sec-startMine.tv_sec)*1000000 + (endMine.tv_usec-startMine.tv_usec);
   printf("rank %d answer %d time %d mine\n", rank, g, timeMine);

   MPI_Finalize();

}