#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>

int main(int argc,char *argv[])
{
    if (argc !=3) {
        printf("Not enough arguments...\n");
        exit(1);
    }

    int P = atoi(argv[2]);

    if (P != 1 && P != 2 && P != 4 && P != 8) {
        printf("invalid value for p, must be 1,2,4,8...\n");
        exit(1);
    }

    long long int i, loops;
    loops = atoll(argv[1]);
    printf("Info: number of iterations = %lld (%ld)\n", loops, sizeof(long long int));

    omp_set_num_threads(P);

    #pragma omp parallel
	{
		assert(P == omp_get_num_threads());
		//printf("Debug: number of threads set = %d\n",omp_get_num_threads());

		int rank = omp_get_thread_num();
		//printf("Info: Rank=%d: my world has %d threads\n",rank,p);
	}  // end of my omp parallel region

    return 0;
}