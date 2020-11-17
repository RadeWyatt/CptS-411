#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

// get distance between point (x,y) and point (0.5,0.5)
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

    double count = 0.0;
	double runtime = omp_get_wtime();
	#pragma omp parallel default(none) reduction(+:count) shared(loops) private(i)
	{
		struct drand48_data buffer;
		int rank = omp_get_thread_num();
		srand48_r((rank+1) * time(0), &buffer);

		#pragma omp for
		for(i = 0; i < loops; i++) 
		{	
			double x, y;
			int rank = omp_get_thread_num();
			drand48_r(&buffer, &x);
			drand48_r(&buffer, &y);
			if (getDistance(x, y) <= 0.5) { 
				count++;
			}
		}		
	}
	runtime = omp_get_wtime() - runtime;

	// end of the second parallel region for FOR LOOP
	printf("\n%f seconds \n", runtime);

    double pi = 4 * (count / (double)loops);

    printf("PI = %.20f\n", pi);
	printf("HITS: %f\nTOTAL SHOTS: %lld\n", count, loops);
	printf("************************************************\n");

	return 0;
}