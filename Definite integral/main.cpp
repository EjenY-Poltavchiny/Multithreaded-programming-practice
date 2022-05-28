#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <utility>
#define f(x) 4/(1 + (x)*(x))

int main(int argc, char* argv[])
{
    int size, rank; 
    int  N = atoi(argv[1]);
    double a = 0, b = 1;
    double h = (b - a)/N;
    double border[2];
    double I = 0, I_temp;
    double x = a;
    double T_paralol, T_cons;
    MPI_Status Status;
    MPI_Init(&argc, &argv);
   
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double newsize = (b - a)/size;
    double start_time;
    double end_time;
    int N_rem = N - size*(N/size); 
    if (!rank)
    {
        start_time = MPI_Wtime();
        for (int i = 0; i<N; ++i)
	{
	    I += h/2*(f(x) + f(x+h));
	    x += h;	
	}
	end_time = MPI_Wtime();
	T_cons = end_time - start_time;
        std::cout << "I_cons = " << I << std::endl << "Time for 1 process " << T_cons << std::endl;
    }	
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    if (!rank)
    {
        for (int i = 0; i < size; ++i)
	{
	    border[0] = a + i*h*(N/size);
	    border[1] = a + (i + 1)*h*(N/size);
	    MPI_Send(border, 2, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
	}
        if (N_rem)
	{
	    for (int i = 0; i < N_rem; ++i)
       	    {
		border[0] = a + h*size*(N/size) + i*h;
		border[1] = a + h*size*(N/size) + (i+1)*h;
		MPI_Send(border, 2, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
	    }
	}
	
    }
    MPI_Recv(border, 2, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &Status);
    x = border[0];
    I = 0;
    for (int i = 0; i < N/size; ++i)
    {
	I += h/2*(f(x) + f(x+h));
	x += h;
    }
    if(rank < N_rem)
    {
	MPI_Recv(border, 2, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &Status);
	x = border[0];
	I += h/2*(f(x) + f(x+h));
    }
    std::cout << "I" << rank << " = " << I << std::endl;
    MPI_Send(&I, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    if (!rank)
    {
        I = 0;
	for (int i = 0; i < size; ++i)
	{
            MPI_Recv(&I_temp, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &Status);
	    I += I_temp;
        }
        std::cout << "I_parprog = " << I << std::endl;
        end_time = MPI_Wtime();
	T_paralol = end_time - start_time;
        std::cout << "Time for par process = " << T_paralol << std::endl;
        //std::cout  << " [" << size << ", " << T_cons/T_paralol << "], ";
    }
    MPI_Finalize();
    return 0;
}
