#include <iostream>
#include <mpi.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdio>

int main(int argc, char **argv)
{
	int rank, num_proc;
	int  res_proc = 0;
	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		int VectorSize;
		int* v = NULL;
		std::cout << "Please, enter vector size: ";
		std::cin >> VectorSize;
		v = new int[VectorSize];
		std::cout << "Vector:";
		for (int i = 0; i < VectorSize; ++i) {
			v[i] = rand() % 10;
			std::cout << " " << v[i];
		}
		std::cout << std::endl;
		int * proc = new int[num_proc];
		if (VectorSize%num_proc == 0)
			for (int i = 0; i < num_proc; i++)
				proc[i] = VectorSize / num_proc;
		else
		{
			for (int i = 0; i < num_proc; i++)
				proc[i] = VectorSize / num_proc;
			for (int i = 0; i < VectorSize%num_proc; i++)
				proc[i] += 1;
		}
		std::cout << "Process number: " << rank << " works ... " << std::endl;
		for (int i = 0; i < proc[0] - 2; i++)
		{
			if ((v[i] > v[i + 1]) != (v[i + 1] > v[i + 2]))
			{
				res_proc += 1;
			}
		}
		for (int i = 1; i < num_proc; i++)
		{
			MPI_Send(proc + i, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			int pos = 0;
			for (int j = 0; j < i; j++)
				pos += proc[j];
			MPI_Send(v + pos - 1, proc[i]+1, MPI_INT, i, 0, MPI_COMM_WORLD);
			
		}
	}
	else {
		int SizeOfParts;
		MPI_Status status;
		MPI_Recv(&SizeOfParts, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		int parts[100];
		MPI_Recv(&parts, SizeOfParts+1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		std::cout << "Process number: " << rank << " works ... " << std::endl;
		std::cout << "Part:";
		for (int i = 0; i < SizeOfParts+1; i++) 
			std::cout << " " << parts[i];
		std::cout << std::endl;
		for (int i = 0; i < SizeOfParts - 1; i++)
		{
			if ((parts[i] > parts[i + 1]) != (parts[i + 1] > parts[i + 2]))
			{
				res_proc += 1;
			}
		}
	}
		std::cout << " result: " << res_proc << std::endl;
		int res;
		MPI_Reduce(&res_proc, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout << "Final result:  " << res << std::endl;
	}
	MPI_Finalize();
	return 0;
}