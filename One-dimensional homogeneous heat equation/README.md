# One-dimensional homogeneous heat equation

* Solving a one-dimensional homogeneous heat equation $$\Large \displaystyle \frac{\partial u}{\partial t} = k\frac{\partial^2 u}{\partial x^2},$$ using a second-order approximation scheme $$\Large \displaystyle \frac {u_i^{n+1} - u_i^n}{\tau} = k\frac{u_{i+1}^n - 2u_i^n + u_{i-1}^n}{h^2}.$$ 

* Getting **parallel** and sequential **solution** of the difference equation. 

* **Comparing** results of parallel and sequential work.

# Files

* [main.cpp](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/main.cpp) - solution of the problem is presented, the program is launched through the console. 

#### To compile use: 
```sh
~$ mpiCC main.cpp -o exec
``` 
#### For the **executive file**: 
```sh
~$ mpiexec -np "processes_number" ./exec "N_value"
``` 
When removing comments, the program outputs the exact solution, the solution in a sequential way and the solution by parallelization. Comments out the conclusions of the results of a sequential calculation and the complex output of the results of a parallel calculation (line #179). This is done for more accurate time counting. In this form, the file should be run to get data of speed up the work time, using script.sh. 


* [script.sh](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/script.sh) - **bash** script for repeatedly running the program for 1-8 processes, for N = 2000, 10000, 50000. Collects data into a file for_graph.txt.
#### Command to get acceleration data: 
```sh
~$ bash script.sh
``` 

* [Graphs.ipynb](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/One-dimensional%20homogeneous%20heat%20equation/Graphs.ipynb)

Python code for plotting acceleration graphs depending on the number of processes for different N.

# Author

Poltavtsev Evgeniy

> I will be glad to have a star if it was useful :wink:
