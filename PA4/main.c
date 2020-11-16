#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

double getDistance(double x, double y) {
    double x1 = x - 0.5;
    double y1 = y - 0.5;
    return sqrt(x1*x1 + y1*y1);
}

int main(int argc,char *argv[])
{
	long long int i, loops;

	if(argc<2) {
		printf("Usage: loop {number of iterations} [number of threads]\n");
		exit(1);
	}
	
	loops = atoll(argv[1]);

	int p=1;
	if(argc==3) {
		p = atoi(argv[2]);
		assert(p>=1);
        printf("Debug: number of requested threads = %d\n",p);
	}

	omp_set_num_threads(p);

	#pragma omp parallel
	{
		assert(p==omp_get_num_threads());
		int rank = omp_get_thread_num();
	} 

	// printf("Debug: sizeof pointer= %ld bytes \n",sizeof(int *));

    struct drand48_data buffer;
    srand48_r ((unsigned) time(NULL), &buffer);

    double x, y;
    double count = 0.0;
	double runtime = omp_get_wtime();
	#pragma omp parallel for schedule(static) reduction(+:count)	//creates N threads to run the next enclosed block 
	for(i = 0; i < loops; i++)  // or line in parallel
	{	
		drand48_r(&buffer, &x);
        drand48_r(&buffer, &y);
        int rank = omp_get_thread_num();
        //printf("POINT %lld: (%f, %f)     Rank = %d\n", i, x, y, rank);
        if (getDistance(x, y) <= 0.5) { // in the circle
            count++;
        }
	}
	// end of the second parallel region for FOR LOOP
	
	runtime = omp_get_wtime() - runtime;
	printf("\n %f seconds \n", runtime);

    double pi = 4 * (count / (double)loops);

    printf("PI = %f\n", pi);

	return 0;
}