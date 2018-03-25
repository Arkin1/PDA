#include "mpi.h" 
#include<iostream>
using namespace std;

const int nr = 3;

int v[10] = { 4,1,2,3,3,3,3,10,11,12 };
int p[10] = { 0,1,2,3,4,5,6,7,8,9 };
int main(int argc, char **argv)
{   
	
	int  numtasks, rank, len, rc;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	
	rc = MPI_Init(&argc,&argv);

	if (rc != MPI_SUCCESS)
	{     
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(hostname, &len);

	int array_size = sizeof(v) / sizeof(int);

	int del[2];

	MPI_Request request;



	MPI_Irecv(&del, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

	if (rank == 0)
	{
		int nr = array_size / numtasks;
		int r = array_size % numtasks;
		int del[2];
		del[0] = del[1] = 0;
		
		for (int i = 0; i < numtasks; ++i)
		{
			del[1] = del[0] + nr + (r>0) - 1;
			MPI_Send(&del, 2, MPI_INT, i, 1, MPI_COMM_WORLD);

			del[0] = del[1] + 1;

			if (r > 0)
				--r;
		}
		
	}
	MPI_Status status;

	MPI_Wait(&request, &status);

	printf("My rank= %d del=(%d %d)\n", rank, del[0], del[1]);

	int nothing = -1;
	
	for (int i = del[0]; i <= del[1]; ++i)
	{
		
		if(v[i] == nr)
			MPI_Isend(&p[i], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
	
	}
	MPI_Isend(&nothing, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

	if (rank == 0)
	{
		int p = numtasks;
		while(p)
		{
			int nr;
			MPI_Recv(&nr, 1, MPI_INT , MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (nr != -1)
				cout << nr << " ";
			else
				p--;

		}
	}


	MPI_Finalize();

	return 0;
}
