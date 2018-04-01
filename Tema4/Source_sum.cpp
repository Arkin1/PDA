#include "mpi.h" 
#include<iostream>
using namespace std;

const int nr = 3;

int array_size=10;
int main(int argc, char **argv)
{

	int  numtasks, rank, len, rc;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	rc = MPI_Init(&argc, &argv);

	if (rc != MPI_SUCCESS)
	{
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);

	

	if (rank == 0)
	{
		int nr = array_size / (numtasks-1);
		int r = array_size % (numtasks-1);
		int x[100], ind = 0;
		for (int i = 1; i < numtasks; ++i)
		{
			for (int j = 1; j <= nr + (r > 0); ++j)
			{
				x[ind] = rand();

				cout << x[ind]<<" ";

				ind++;
			}

			MPI_Send(&ind, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

			MPI_Send(&x, ind, MPI_INT, i, 1, MPI_COMM_WORLD);

			if (r > 0)
				--r;


			ind = 0;

		}

	}

	if (rank != 0)
	{
		int sz;
		int x[100];

		MPI_Recv(&sz, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&x, sz, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		int sum = 0;

		for (int i = 0; i < sz; ++i)
		{

			sum += x[i];

		}

		MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	


	if (rank == 0)
	{
		int sum = 0;
		for (int i = 1; i < numtasks; ++i)
		{
			int nr;
			MPI_Recv(&nr, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			sum += nr;
		}

		cout <<'\n'<< sum;
	}


	MPI_Finalize();

	return 0;
}
