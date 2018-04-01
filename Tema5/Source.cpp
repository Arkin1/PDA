#include<mpi.h>
#include<iostream>
#include<time.h>
#include<Windows.h>
using namespace std;


int main(int argc, char** argv)
{
	int numberProc, procId, rc, toSend[2], toReceive[2], leaderId;
	MPI_Status status;
	MPI_Request request;

	rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS)
	{
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numberProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &procId);

	leaderId = procId;

	srand(procId*time(NULL));

	int maxPriority = rand();

	for (int i = 0; i < numberProc; i++)
	{
		toSend[0] = leaderId;
		toSend[1] = maxPriority;
		MPI_Isend(&toSend, 2, MPI_INT, (procId + 1) % numberProc, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(&toReceive, 2, MPI_INT, (numberProc + procId - 1) % numberProc, 0, MPI_COMM_WORLD, &status);
		cout << procId <<"(" << toReceive[0] << " " << toReceive[1] << ") on step " << i << '\n';
		if (toReceive[1] > maxPriority)
		{
			leaderId = toReceive[0];
			maxPriority = toReceive[1];

		}
		else if(toReceive[1] == maxPriority && toReceive[0] > procId)
		{
			
			leaderId = toReceive[0];
			maxPriority = toReceive[1];
		
		}
	}

	Sleep(3000);
	cout << procId << "(" << leaderId << " " << maxPriority << ")";

	MPI_Finalize();
	return 0;
}
