
#include <iostream>
#include <mpi.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdio>

int main(int argc, char **argv)
{
	int rank, num_proc;
	int VectorSize;
	int* v = NULL;
	srand(time(0));

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		std::cout <<"Please, enter vector size: ";
		std::cin >> VectorSize;
	}
	MPI_Bcast(&VectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	v = new int[VectorSize];
	if (rank == 0) {
		std::cout << "Vector:";
		for (int i = 0; i < VectorSize; ++i) {
			v[i] = rand() % 10;
			std::cout << " " << v[i];
		}
		std::cout << std::endl;
	}
	MPI_Bcast(v, VectorSize, MPI_INT, 0, MPI_COMM_WORLD);

	int SizeOfParts = (VectorSize - 1) / num_proc + ((rank < (VectorSize - 1) % num_proc) ? 1 : 0);
	int begin = ((VectorSize - 1) / num_proc) * rank + std::min((VectorSize - 1) % num_proc, rank)+1;
	int res_proc = 0;
	while (SizeOfParts + begin + 1 > VectorSize)SizeOfParts--;
	std::cout << "Process number: " << rank << " works ... " << std::endl;
	for (int i = 0; i < SizeOfParts; ++i)
	{
		if ((v[begin + i - 1] > v[begin + i]) != (v[begin + i] > v[begin + i + 1]))
		{
			res_proc += 1;
		}
	}
	 std::cout<< " result: " << res_proc << "SOP: "<< SizeOfParts<< " beg: "<< begin <<std::endl;

	int res;
	MPI_Reduce(&res_proc, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		std::cout << "Final result:  " << res << std::endl;
	}
	MPI_Finalize();
	return 0;
}