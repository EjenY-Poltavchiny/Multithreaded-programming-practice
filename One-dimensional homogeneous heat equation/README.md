# One-dimensional homogeneous heat equation

Solving a one-dimensional homogeneous heat equation by using a second-order approximation scheme. Parallel solution of the difference equation. Comparison of the results of parallel and sequential work.

# Files

* [main.cpp](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/main.cpp)

The solution of the problem is presented, the program is launched through the console. For the executive file: mpiexec -np %processes_number% %filename%. Comments out the conclusions of the results of a sequential calculation and the complex output of the results of a parallel calculation (line #179). This is done for more accurate time counting. In this form, the file should be run to get data of speed up the work time.

* [script.sh](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/script.sh)

Bash script for repeatedly running the program for 1-8 processes, at N = 2000, 10000, 50000.

* [Graphs.ipynb](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/Graphs.ipynb)

Python code for plotting acceleration graphs depending on the number of processes for different N.

# Enviroment

C++ (MPI library)
