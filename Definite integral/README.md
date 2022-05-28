# Definite integral

Calculation of a definite integral $$\Large \displaystyle \int\limits_0^1 \frac{4}{1 + x^2}dx,$$ using the trapezoid method. Get parallel and sequential solution and compare the results of work.

# Files

* [main.cpp](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/Definite%20integral/main.cpp)

Here is the code for solving the problem, run through the console. Instructions for the executive file: mpiexec -np %processes_number% %filename%. The program outputs the solution calculated sequentially and by each individual process (in case of parallel operation). Line #89 is necessary for repeated execution of the file when calculating acceleration. In this case, all other conclusions should be commented out.

* [script.sh](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/Definite%20integral/script.sh)

A script for repeatedly running a program with the number of processes 1-8 for N = 1000, 1000000, 100000000. Collects data into a file for_graph.txt.

* [Graphs.ipynb](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/Definite%20integral/Graphs.ipynb)

Python code for plotting acceleration graphs depending on the number of processes for different N.

# Author 

Poltavtev Evgeniy

# Enviroment 

C++ (MPI library)
