#include "helper.c"

int main(int argc,char *argv[])
{

   int rank,p, n, rseed;
   int bp = 93563;
   int **work;
   n = 7;
   
   work = malloc(sizeof(int*) * (n/p));
   for(int i = 0; i < n/p; i++)
   {
      section[i] = malloc(sizeof(int) * n);
   }

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   GenerateInitialGoL(p, bp, rank, n, work);
   printShare(work, n, p);

   MPI_Finalize();

}
