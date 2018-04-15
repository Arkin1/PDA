#include "mpi.h" 
#include<iostream>
using namespace std;

#define N_MAX 5

int D[N_MAX][N_MAX]={
						0,3,9,8,3,
						5,0,1,4,2,
						6,6,0,4,5,
						2,9,2,0,7,
						7,9,3,2,0
					};

int main(int argc, char **argv)
{

	int  numtasks, rank, len, rc;    
	rc = MPI_Init(&argc, &argv);   
	if (rc != MPI_SUCCESS)
	{
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);   
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (int k = 0; k < N_MAX; ++k)
	{
		for (int j = 0; j < N_MAX; ++j)
		{
			if (D[rank][j] > D[rank][k] + D[k][j])
				D[rank][j] = D[rank][k] + D[k][j];
		}
		MPI_Request req;
		MPI_Allgather(D[rank], N_MAX, MPI_INT, D, N_MAX, MPI_INT, MPI_COMM_WORLD);


	}
	
	if (rank == 0)
	{
		for (int i = 0; i < N_MAX; ++i)
		{
			for (int j = 0; j < N_MAX; ++j)
				
					cout << D[i][j] << " ";
			
			cout << '\n';
		}
	}



	MPI_Finalize();

	return 0;
}
