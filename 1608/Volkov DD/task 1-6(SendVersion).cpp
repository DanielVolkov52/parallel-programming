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
	double time;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int *parts;
	
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
		time = MPI_Wtime();
		int *resultVector= new int[VectorSize];
		int counter=1,tmp=v[0];
		resultVector[0] = v[0];
		for (int i = 1; i < VectorSize; i++)
		{
			if (tmp != v[i])
			{
				resultVector[counter++] = v[i];
			}
			tmp = v[i];
		}
		std::cout << "resultVector:";
		for (int i = 0; i < counter; ++i) {
			v[i] = rand() % 10;
			std::cout << " " << resultVector[i];
		}
		std::cout << std::endl;
		
		int count = VectorSize;
		
		std::cout << "Process number: " << rank << " works ... " << std::endl;
		int size = counter / num_proc + (0 < counter % num_proc);

		for (int i = 0; i < size-2; i++)
		{
			if ((resultVector[i] > resultVector[i + 1]) != (resultVector[i + 1] > resultVector[i + 2]))
			{
				res_proc += 1;
			}
		}
		for (int i = 1; i < num_proc; i++)
		{
			
			size = counter / num_proc + (i < counter % num_proc)+2;
			MPI_Send(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			int pos = 0;
			for (int j = 0; j < i; j++)
				pos += counter / num_proc + (j < counter % num_proc);
			MPI_Send(resultVector + pos - 2, size, MPI_INT, i, 0, MPI_COMM_WORLD);
			
		}
	}
	else {
		int SizeOfParts;
		MPI_Status status;
		MPI_Recv(&SizeOfParts, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		parts = new int[SizeOfParts + 2];
		MPI_Recv(parts, SizeOfParts+1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		std::cout << "Process number: " << rank << " works ... " << std::endl;
		std::cout << "Part:";
		for (int i = 0; i < SizeOfParts+1; i++) 
			std::cout << " " << parts[i];
		std::cout << std::endl;
		for (int i = 0; i < SizeOfParts - 2; i++)
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
		
		printf("Final result: %d\nTime:  %.10lf " ,res, MPI_Wtime()-time);
	}
	MPI_Finalize();
	return 0;
}