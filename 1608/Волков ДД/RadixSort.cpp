#include<iostream>
#include<queue>
#include <mpi.h>
#include<time.h>
using namespace std;
void swap(int* a, int * b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
void RadixSort(int* arr, int size)
{
	int maxRadix = 0;
	int tmp, count, max=abs(arr[0]);
	queue<int> buf[20];
	for (size_t i = 0; i < size; i++)
	{
		/*tmp = arr[i];
		count = (tmp < 0) ? 0 : 9;
		while (tmp != 0)
		{
			tmp /= 10;
			count++;
		}
		if (count > maxRadix) maxRadix = count;*/
		if (max < abs(arr[i])) max = abs(arr[i]);
	}
	while (max != 0)
	{
		max /= 10;
		maxRadix++;
	}


	for (size_t i = 0; i < maxRadix; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			tmp = arr[j];
			count = 0;
			while (count < i)
			{
				tmp /= 10;
				count++;
			}
			buf[tmp % 10 + 10].push(arr[j]);
		}
		count = 0;
		for (size_t num = 0; num < 20; num++)
		{
			while (!buf[num].empty())
			{
				arr[count++] = buf[num].front();
				buf[num].pop();
			}
		}


	}

}
int main(int argc, char** argv)
{
	srand(time(NULL));
	const int info = 0, sizeData = 1, data = 2;
	int rank, size, num_proc;
	int parent = -1, brother = -1, children1 = -1, children2 = -1;
	int VectorSize;
	double time;
	int * part, *part2;
	int tmp[4];
	int SizeOfParts, SizeOfParts2;
	MPI_Comm comm_cart;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		cout << num_proc << endl;
		cout << "Please, enter vector size: ";
		cin >> VectorSize;
		part = new int[VectorSize];
		part2 = new int[VectorSize];
		//std::cout << "Vector:";
		for (int i = 0; i < VectorSize; ++i) {
			part[i] = part2[i] = rand() % 1000 - 500;
			
			//cout << part[i] << " ";
		}
		time = clock();
		RadixSort(part2, VectorSize);
		/*std::cout << "\nVector sort:";
		for (int a = 0; a < VectorSize; a++)
		{
			cout << part2[a] << " ";
		}
		cout << endl;*/
		cout << "Serial time: " << (clock() - time)/CLOCKS_PER_SEC << endl;
		cout << endl;
		time = MPI_Wtime();
		children1 = 1;
		tmp[0] = 0;
		tmp[1] = -1;
		tmp[2] = 2;
		tmp[3] = 3;
		MPI_Send(tmp, 4, MPI_INT, 1, info, MPI_COMM_WORLD);
			tmp[0] = 1;
			tmp[1] = 3;
			tmp[2] = 4;
			tmp[3] = 5;
			MPI_Send(tmp, 4, MPI_INT, 2, info, MPI_COMM_WORLD);
			tmp[0] = 1;
			tmp[1] = 2;
			tmp[2] = 6;
			tmp[3] = 7;
			MPI_Send(tmp, 4, MPI_INT, 3, info, MPI_COMM_WORLD);
			tmp[0] = 2;
			tmp[1] = 5;
			tmp[2] = -1;
			tmp[3] = -1;
			MPI_Send(tmp, 4, MPI_INT, 4, info, MPI_COMM_WORLD);
			tmp[0] = 2;
			tmp[1] = 4;
			tmp[2] = -1;
			tmp[3] = -1;
			MPI_Send(tmp, 4, MPI_INT, 5, info, MPI_COMM_WORLD);
			tmp[0] = 3;
			tmp[1] = 7;
			tmp[2] = -1;
			tmp[3] = -1;
			MPI_Send(tmp, 4, MPI_INT, 6, info, MPI_COMM_WORLD);
			tmp[0] = 3;
			tmp[1] = 6;
			tmp[2] = -1;
			tmp[3] = -1;
			MPI_Send(tmp, 4, MPI_INT, 7, info, MPI_COMM_WORLD);
		MPI_Send(&VectorSize, 1, MPI_INT, children1, sizeData, MPI_COMM_WORLD);
		MPI_Send(part, VectorSize, MPI_INT, children1, data, MPI_COMM_WORLD);
		delete[] part;
		part = new int[VectorSize];
		MPI_Recv(part, VectorSize, MPI_INT, children1, data, MPI_COMM_WORLD, &status);
		time = MPI_Wtime() - time;
		cout << "Parallel sort: " << time << endl;
		bool check = true;
		for (size_t i = 0; i < VectorSize; i++)
		{
			if (part[i] != part2[i]) check = false;
		}
		if (check)cout << "\nOK\n" << endl;
		else
			cout << "\nnotOK\n" << endl;
	/*	cout << "\nSORT (" << rank << "): ";
		for (int a = 0; a < VectorSize; a++)
		{
			cout << part[a] << " ";
		}
		cout << endl;*/

		
	}
	else {
		//if(parent==-1)MPI_Finalize();
		MPI_Recv(tmp, 4, MPI_INT, 0, info, MPI_COMM_WORLD, &status);
		parent = tmp[0];
		brother = tmp[1];
		children1 = tmp[2];
		children2 = tmp[3];
		/*cout << "Process number: " << rank << " info :\nParent: " << parent << "\nBrother: " << brother << "\nClidren: " << children1 <<", " <<children2<< std::endl;*/
		MPI_Recv(&SizeOfParts, 1, MPI_INT, parent, sizeData, MPI_COMM_WORLD, &status);
		part = new int[SizeOfParts];
		MPI_Recv(part, SizeOfParts, MPI_INT, parent, data, MPI_COMM_WORLD, &status);
		/*cout << "Process number: " << rank << " works ... " << std::endl;
		cout << "\nPart (" << rank << "): ";
		for (int a = 0; a < SizeOfParts; a++)
		{
			cout << part[a] << " ";
		}
		cout << endl;*/
		if (children1 != -1 && children2 != -1)
		{
			size = SizeOfParts / 2;
			MPI_Send(&size, 1, MPI_INT, children1, sizeData, MPI_COMM_WORLD);
			MPI_Send(part, size, MPI_INT, children1, data, MPI_COMM_WORLD);
			size = SizeOfParts - size;
			MPI_Send(&size, 1, MPI_INT, children2, sizeData, MPI_COMM_WORLD);
			MPI_Send(part + (SizeOfParts - size), size, MPI_INT, children2, data, MPI_COMM_WORLD);
			delete[] part;
			part = new int[SizeOfParts];
			MPI_Recv(part, SizeOfParts, MPI_INT, children1, data, MPI_COMM_WORLD, &status);
		}
		else
		{
			RadixSort(part, SizeOfParts);
			/*cout << "\nPart after sort(" << rank << "): ";
			for (int a = 0; a < SizeOfParts; a++)
			{
				cout << part[a] << " ";
			}
			cout << endl;*/
		}
		if (brother != -1)
		{
			if (brother > rank)
			{
				MPI_Send(&SizeOfParts, 1, MPI_INT, brother, sizeData, MPI_COMM_WORLD);
				MPI_Send(part, SizeOfParts, MPI_INT, brother, data, MPI_COMM_WORLD);
				MPI_Recv(&SizeOfParts2, 1, MPI_INT, brother, sizeData, MPI_COMM_WORLD, &status);
				part2 = new int[SizeOfParts2];
				MPI_Recv(part2, SizeOfParts2, MPI_INT, brother, data, MPI_COMM_WORLD, &status);
				int *res = new int[SizeOfParts + SizeOfParts2];
				int i1, i2, ires;
				i1 = i2 = ires = 0;
				while (i1 < SizeOfParts && i2 < SizeOfParts2)
				{
					if (part[i1] < part2[i2])
					{
						res[ires] = part[i1];
						i1 += 2;
					}
					else
					{
						res[ires] = part2[i2];
						i2 += 2;
					}
					ires += 2;
				}
				if (i1 >= SizeOfParts) {
					while (i2 < SizeOfParts2)
					{
						res[ires] = part2[i2];
						ires += 2;
						i2 += 2;
					}
				}
				if (i2 >= SizeOfParts2)
				{
					while (i1 < SizeOfParts)
					{
						res[ires] = part[i1];
						ires += 2;
						i1 += 2;
					}
				}
				int SizeOfRes = SizeOfParts + SizeOfParts2;
				int *res2 = new int[SizeOfRes];
				delete[] part;
				delete[] part2;
				MPI_Recv(res2, SizeOfRes, MPI_INT, brother, 5, MPI_COMM_WORLD, &status);
				part = new int[SizeOfRes];
				SizeOfParts = SizeOfRes;
				for (size_t x = 0; x < SizeOfRes; x += 2)
				{
					part[x] = res[x];
					part[x + 1] = res2[x + 1];
				}
				/*cout << "\nPart after  merge(" << rank << "): ";
				for (int a = 0; a < SizeOfParts; a++)
				{
					cout << part[a] << " ";
				}
				cout << endl;*/
				delete[] res;
				delete[] res2;
				MPI_Send(part, SizeOfParts, MPI_INT, parent, data, MPI_COMM_WORLD);
				//MPI_Finalize();

			}
			else
			{
				MPI_Recv(&SizeOfParts2, 1, MPI_INT, brother, sizeData, MPI_COMM_WORLD, &status);
				part2 = new int[SizeOfParts2];
				MPI_Recv(part2, SizeOfParts2, MPI_INT, brother, data, MPI_COMM_WORLD, &status);
				MPI_Send(&SizeOfParts, 1, MPI_INT, brother, sizeData, MPI_COMM_WORLD);
				MPI_Send(part, SizeOfParts, MPI_INT, brother, data, MPI_COMM_WORLD);
				int *res = new int[SizeOfParts + SizeOfParts2];
				int i1, i2, ires;
				i1 = i2 = ires = 1;
				while (i1 < SizeOfParts && i2 < SizeOfParts2)
				{
					if (part[i1] < part2[i2])
					{
						res[ires] = part[i1];
						i1 += 2;
					}
					else
					{
						res[ires] = part2[i2];
						i2 += 2;
					}
					ires += 2;
				}
				if (i1 >= SizeOfParts) {
					while (i2 < SizeOfParts2)
					{
						res[ires] = part2[i2];
						ires += 2;
						i2 += 2;
					}
				}
				if (i2 >= SizeOfParts2)
				{
					while (i1 < SizeOfParts)
					{
						res[ires] = part[i1];
						ires += 2;
						i1 += 2;
					}
				}
				int SizeOfRes = SizeOfParts + SizeOfParts2;
				MPI_Send(res, SizeOfRes, MPI_INT, brother, 5, MPI_COMM_WORLD);
				delete[] part;
				delete[] part2;
				delete[] res;
				//MPI_Finalize();


			}
		}
		else {
		int i = 1, j = 2, ires=0;
		int *result = new int[SizeOfParts];
		result[ires++] = part[0];
		result[SizeOfParts - 1] = part[SizeOfParts - 1];
		while (i <= SizeOfParts - 2 && j <= SizeOfParts - 1)
		{
			if (part[i] < part[j]) {
				result[ires++] = part[i];
				i += 2;
			}
			else
			{
				result[ires++] = part[j];
				j += 2;
			}
		}
		if (i >= SizeOfParts - 2)
		{
			while (j < SizeOfParts - 1)
			{
				result[ires++] = part[j];
				j += 2;
			}
		}
		if (j >= SizeOfParts - 1)
		{
			while (i < SizeOfParts - 1)
			{
				result[ires++] = part[i];
				i += 2;
			}
		}

			MPI_Send(result, SizeOfParts, MPI_INT, parent, data, MPI_COMM_WORLD);
			//MPI_Finalize();
		}




	}
	
		

	MPI_Finalize();
}
