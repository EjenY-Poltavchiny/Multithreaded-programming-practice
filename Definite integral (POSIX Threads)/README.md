# Definite integral (POSIX Threads)

Calculation of a definite integral $$\Large \displaystyle \iint\limits_\Omega xy\ dxdy,$$

$$\text{where}\ \Omega = 
 \begin{cases}
   0 \leq x \leq \pi,\\
   0 \leq y \leq sin(x)
 \end{cases}$$
 
 using the Monte Carlo method. Get parallel and sequential solution and compare the results of work. Perform parallelization using **POSIX Threads**.
 
 # Files 
 
 * [main.cpp](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/Definite%20integral%20(POSIX%20Threads)/main.cpp)
  
 Here is the code for solving the problem, run through the console. To **compile**, use the command: gcc %filename% -lpthread -lm. Instructions for the **executive file**: %filename% %processes_number%. 
 
 **Output** - the value of the integral calculated in parallel and sequentially (if you remove the relevant comments). The final answer is obtained by combining the responses of each thread in two ways (check output format).
 
 * [Graphs.ipynb](https://github.com/EjenY-Poltavchiny/Multithreaded-programming-practice/blob/main/Definite%20integral%20(POSIX%20Threads)/Graphs.ipynb)

Python code for plotting acceleration graphs depending on the number of processes for N = $10^9$.
