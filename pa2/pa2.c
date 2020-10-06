/*  Cpt S 411, Introduction to Parallel Computing
 *  School of Electrical Engineering and Computer Science
 *    
 *  Example code
 *  	Hello world MPI
 *          
 */


#include <stdio.h>
#include <mpi.h> 
#include "helper.c"

int main(int argc,char *argv[])
{

   int rank,p, n, rseed;
   int bp = 93563;
   int **work;
   n = 7;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   GenerateInitialGoL(p, bp, rank, n, &work);
   printShare(work, n, p);

   MPI_Finalize();

}
