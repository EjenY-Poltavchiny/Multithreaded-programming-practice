#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <cstring>
#include <iostream>
#include <cmath>
#include <utility>

int main(int argc, char* argv[])
{ 

    double start, end;
    double T_cons, T_par;
    int size, rank;
    double u_0 = 1;
    double l = 1;
    double k = 1;
    double h, t;
    //double T = 0.1;
    double T = 0.00001;
    double fict_left = 0, fict_right = 0;
    double exact = 0;
    int  new_steps, tau_steps;
    int  N = atoi(argv[1]) + 1;
    double borders[2] = {0, 0};
    MPI_Status Status;
    MPI_Init(&argc, & argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    h = l/(N - 1);
    t = (h*h)/(2.1*k);
    //t = 0.0002;
    int rem = N%size;
    int steps = N/size;
    tau_steps = T/t;
    if (!rank)
    {
	//// 1 proc calculation
	start = MPI_Wtime();
	double* U_h1 = new double[N];
        double* next_U_h1 = new double[N];
        U_h1[0] = 0;
        U_h1[N-1] = 0;
	for (int i = 1; i < N-1; ++i)
        {
            U_h1[i] = u_0;
        }
	for (int j = 0; j < tau_steps; ++j)
        {
            U_h1[0] = 0;
            U_h1[N-1] = 0;
            for (int i = 1; i < N-1; ++i)
            {
                next_U_h1[i] = U_h1[i] + t*k/(h*h)*(U_h1[i+1] - 2*U_h1[i] + U_h1[i-1]);
            }
            for (int i = 1; i < N-1; ++i)
            {
                U_h1[i] = next_U_h1[i];
            }
        }
        //// Output for ine process
	/*std::cout << "For one process ======================================" << std::endl;
        double helper1 = 0;
        double step_output1 = 0.1/h;
        for (int j = 0; j < N; ++j)
        {
	    ++helper1;
            if (!j) {std::cout << U_h1[j] << " "; --helper1;}
            if (helper1/step_output1 >= 1) {std::cout << U_h1[j] << " " << std::endl; helper1 = 0;} 
        }
        std::cout << std::endl;*/
        delete[] U_h1;
        delete[] next_U_h1;
        end = MPI_Wtime();
        T_cons = end - start;
        //std::cout << "Time cons = " << T_cons << std::endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    if (!rank)
    {
        for (int i = 0; i < size; ++i)
	{
	    bool overage = 0;
	    if (rem > i)
	    {
		overage = 1;
	    }	
	    new_steps = steps + overage;
	    MPI_Send(&new_steps, 1, MPI_INT, i, 1, MPI_COMM_WORLD); 
	}
        //// Exact solution
        /*std::cout <<"Exact ======================================" << std::endl;
        for (int i = 0; i < 11; i++)
        {
            exact = 0;
            for (int m = 0; m < 10000; m++) 
            {
                exact += 4*u_0 / M_PI * exp((-1)*M_PI * M_PI * 0.1 * (2 * m + 1) * (2 * m + 1) / (l * l)) * sin(M_PI* 0.1  * i * (2 * m + 1) / l) / (2 * m + 1);
            }
            printf("U(%.3f, %.1f) = %.6f\n", i*0.1, 0.1, exact);  
        }
        std::cout << std::endl;*/
    }
    MPI_Recv(&new_steps, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);
    double* U_h = new double[new_steps];
    double* next_U_h = new double[new_steps];
    //// initial conditions
    for (int i = 0; i < new_steps; ++i)
    {
        U_h[i] = u_0;
    }
    if (!rank) 
    {
        U_h[0] = 0;
    }    
    if (rank == size - 1)
    { 
        U_h[new_steps - 1] = 0;
    }
    //// Approx scheme realization
    for (int j = 0; j < tau_steps; ++j)
    {
        ////(O(p))
        //if (rank > 0) {MPI_Send(&U_h[0], 1, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD);}
        //if (rank < size - 1) {MPI_Recv(&fict_right, 1, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD, &Status);}
        //if (rank < size - 1) {MPI_Send(&U_h[new_steps - 1], 1, MPI_DOUBLE, rank + 1, 3, MPI_COMM_WORLD);}
        //if (rank > 0) {MPI_Recv(&fict_left, 1, MPI_DOUBLE, rank - 1 , 3, MPI_COMM_WORLD, &Status);}
        ////
        //// (0(1))
        if (rank % 2)
        {	
	    if (rank > 0) {MPI_Send(&U_h[0], 1, MPI_DOUBLE, rank - 1, 4, MPI_COMM_WORLD);}
            if (rank > 0) {MPI_Recv(&fict_left, 1, MPI_DOUBLE, rank - 1 , 5, MPI_COMM_WORLD, &Status);}
            if (rank < size - 1) {MPI_Recv(&fict_right, 1, MPI_DOUBLE, rank + 1, 6, MPI_COMM_WORLD, &Status);}
	    if (rank < size - 1) {MPI_Send(&U_h[new_steps - 1], 1, MPI_DOUBLE, rank + 1, 7, MPI_COMM_WORLD);}
        }
        if (!(rank % 2))
        {
            if (rank < size - 1) {MPI_Recv(&fict_right, 1, MPI_DOUBLE, rank + 1, 4, MPI_COMM_WORLD, &Status);}
	    if (rank < size - 1) {MPI_Send(&U_h[new_steps - 1], 1, MPI_DOUBLE, rank + 1, 5, MPI_COMM_WORLD);}
            if (rank > 0) {MPI_Send(&U_h[0], 1, MPI_DOUBLE, rank - 1, 6, MPI_COMM_WORLD);}
            if (rank > 0) {MPI_Recv(&fict_left, 1, MPI_DOUBLE, rank - 1 , 7, MPI_COMM_WORLD, &Status);}
        }
        ////
        // The process of counting values in nodes
        for (int i = 0; i < new_steps; ++i) 
        {
            if (new_steps == 1)
	    {
		next_U_h[i] = U_h[i] + t*k/(h*h)*(fict_right - 2*U_h[i] + fict_left);
	    }
	    if (!rank && i == 0)
            {
                next_U_h[0] = 0;
                continue;	    
            }
	    if (rank == size - 1 && i == new_steps - 1)
            {
		next_U_h[new_steps - 1] = 0;
                continue;
            }
            if (rank && i == 0) 
            {
		next_U_h[i] = U_h[i] + t*k/(h*h)*(U_h[i+1] - 2*U_h[i] + fict_left);
                continue;
            }
	    if (rank < size - 1 && i == new_steps - 1)
            {
               next_U_h[i] = U_h[i] + t*k/(h*h)*(U_h[i-1] - 2*U_h[i] + fict_right);
               continue;
            }
            next_U_h[i] = U_h[i] + t*k/(h*h)*(U_h[i+1] - 2*U_h[i] + U_h[i-1]);
	}
        for (int k = 0; k < new_steps; ++k)
        { 
	    U_h[k] = next_U_h[k];
        } 
    }
    ///// Output for par process
    // thanckfuly to this value, processes will know how to count steps to i*0,1 point
    /*double helper = 0; 
    double step_output = 0.1/h;
    int sends_number = 0;

    if (!rank) {std::cout << "Results for parallel processes ======================================" << std::endl;} 
    if (rank)
    {
        MPI_Recv(&helper, 1, MPI_DOUBLE, rank-1, 10, MPI_COMM_WORLD, &Status);
    }
    for (int j = 0; j < new_steps; ++j)
    {    
        ++helper;
        if (!j && !rank) {MPI_Send(&U_h[j], 1, MPI_DOUBLE, 0, 15 + sends_number, MPI_COMM_WORLD); --helper; ++sends_number; continue; }
        if (rank == size - 1 && j == new_steps - 1) {MPI_Send(&U_h[j], 1, MPI_DOUBLE, 0, 15 + sends_number, MPI_COMM_WORLD); ++sends_number; continue; }
        if (helper/step_output >= 1) {MPI_Send(&U_h[j], 1, MPI_DOUBLE, 0, 15 + sends_number, MPI_COMM_WORLD); helper = 0; ++sends_number;  continue; }
    }
    // Number of sends by single process
    MPI_Send(&sends_number, 1, MPI_INT, 0, 20, MPI_COMM_WORLD);
    if (rank != size - 1)
    {       
        MPI_Send(&helper, 1, MPI_DOUBLE, rank+1, 10, MPI_COMM_WORLD);
    }
    if (!rank)
    {
	double* output_mass = new double[11];
	int outputs_number = 0;
        // Special iterator for array in process 0
        int new_counter = 0;
        for (int i = 0; i < size; ++i)
	{   
	    MPI_Recv(&outputs_number, 1, MPI_DOUBLE, i, 20, MPI_COMM_WORLD, &Status);
            for (int j = 0; j < outputs_number; ++j)
	    {
	        MPI_Recv(&output_mass[new_counter], 1, MPI_DOUBLE, i, 15 + j, MPI_COMM_WORLD, &Status);
	        std::cout << output_mass[new_counter] << " " << std::endl;
	        ++new_counter;
	    }
        }
        std::cout << std::endl;
        delete[] output_mass;
    }*/
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    T_par = end - start;
    //if (rank == size - 1) {std::cout << "Time for parar = " << T_par << std::endl;} 
    if (!rank) {std::cout << " [" << size << ", " << T_cons/T_par << "], ";}
    delete[] U_h; 
    delete[] next_U_h;
    MPI_Finalize();
    return 0;
}
