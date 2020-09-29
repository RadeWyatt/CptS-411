/*
	

	for(int j = 0; j < numIterations; j++)
	{
		int numInts = power(2, j);
		int msgSize = sizeof(int) * numInts;
		if(rank==1) {
			for (int k = 0; k < 10; k++) {
				x = (int *)calloc(numInts, sizeof(int));
				int dest = 0;
				gettimeofday(&t1,NULL);
				MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
				gettimeofday(&t2,NULL);
				int sendMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				nonBlockSendTime[j] = (nonBlockSendTime[j] + sendMicro) / 2;
			}
		} else if (rank==0) {
			MPI_Request request1;
			for (int k = 0; k < 10; k++) {
				MPI_Status status1;
				y = (int *)calloc(numInts, sizeof(int));
				gettimeofday(&t1,NULL);
				MPI_Irecv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&request1);
				MPI_Wait(&request1, &status1);
				gettimeofday(&t2,NULL);
				int recMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				nonBlockRecTime[j] = (nonBlockRecTime[j] + recMicro) / 2;
			}
		}
	}
	*/

	for(int j = 0; j < numIterations; j++)
	{
		int numInts = power(2, j);
		int msgSize = sizeof(int) * numInts;
		if(rank==1) {
			for (int k = 0; k < 10; k++) {
				x = (int *)calloc(numInts, sizeof(int));
				int dest = 0;
				gettimeofday(&t1,NULL);
				MPI_Send(x,numInts,MPI_INT,dest,0,MPI_COMM_WORLD);
				gettimeofday(&t2,NULL);
				int sendMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				blockSendTime[j] = blockSendTime[j] + sendMicro;
			}
		} else if (rank==0) {
			for (int k = 0; k < 10; k++) {
				MPI_Status status;
				y = (int *)calloc(numInts, sizeof(int));
				gettimeofday(&t1,NULL);
				MPI_Recv(y,numInts,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				gettimeofday(&t2,NULL);
				int recMicro = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
				blockRecTime[j] = blockRecTime[j] + recMicro;
			}
		}
	}