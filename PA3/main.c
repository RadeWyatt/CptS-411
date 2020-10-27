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

   if (rank == 0) {
      naiveTimes = malloc(sizeof(int) * p);
      libraryTimes = malloc(sizeof(int) * p);
      myTimes = malloc(sizeof(int) * p);
   }

   GenerateArray();

   //time for naive reduce
   gettimeofday(&startNaive,NULL);
   AddLocal();
   NaiveAllReduce();
   gettimeofday(&endNaive,NULL);
   timeNaive = (endNaive.tv_sec-startNaive.tv_sec)*1000000 + (endNaive.tv_usec-startNaive.tv_usec);

   //time for library all reduce
   gettimeofday(&startLibrary,NULL);
   AddLocal();
   MPILibraryAllReduce();
   gettimeofday(&endLibrary, NULL);
   timeLibrary = (endLibrary.tv_sec-startLibrary.tv_sec)*1000000 + (endLibrary.tv_usec-startLibrary.tv_usec);

   //time for my all reduce
   gettimeofday(&startMine,NULL);
   AddLocal();
   MyAllReduce();
   gettimeofday(&endMine,NULL);
   timeMine = (endMine.tv_sec-startMine.tv_sec)*1000000 + (endMine.tv_usec-startMine.tv_usec);

   MPI_Barrier(MPI_COMM_WORLD);

   int times[3] = {timeNaive, timeLibrary, timeMine};

   // aggregate times in proc 0
   if (rank != 0) {
      MPI_Send(times, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
   } else {
      naiveTimes[0] = times[0];
      libraryTimes[0] = times[1];
      myTimes[0] = times[2];
      for (int i = 1; i < p; i++) {
         int *temp = malloc(sizeof(int) * 3);
         MPI_Recv(temp, 3, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         naiveTimes[i] = temp[0];
         libraryTimes[i] = temp[1];
         myTimes[i] = temp[2];
         free(temp);
      }
   }

   // print max times values for each implementation
   if (rank == 0) {
      printf("MAX NAIVE TIME: %d\n", getMax(naiveTimes));
      printf("MAX LIBRARY TIME: %d\n", getMax(libraryTimes));
      printf("MAX MY TIME: %d\n", getMax(myTimes));
   }
   
   MPI_Finalize();
}