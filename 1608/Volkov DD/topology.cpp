
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size, l, data1, dims[1];
	int periods[1], source, dest;
	int reorder = 0;
	MPI_Comm comm_cart;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (int i = 0; i < 1; i++) { dims[i] = 0; periods[i] = 1; }
	MPI_Dims_create(size, 1, dims);
	MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, reorder,&comm_cart);
	MPI_Cart_shift(comm_cart, 0, 1, &source, &dest);
	if (rank == 0)
	{
		data1 = 3;
		printf("Process# %d send data=%d\n", rank, data1);
		MPI_Send(&data1, 1, MPI_INT, dest, 12, comm_cart);
		MPI_Recv(&data1, 1, MPI_INT, source, 12, comm_cart, &status);
		printf("Process# %d received data=%d\n", rank,  data1);
	}
else
{		
MPI_Recv(&data1, 1, MPI_INT, source, 12, comm_cart, &status);
printf("Process# %d received data=%d\n", rank,  data1);
data1 += rank;
printf("Process# %d send data=%d\n", rank, data1);
MPI_Send(&data1, 1, MPI_INT, dest, 12, comm_cart);
 }
 MPI_Comm_free(&comm_cart);
 MPI_Finalize();
 return 0; 
}
