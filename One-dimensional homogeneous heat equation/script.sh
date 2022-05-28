1 #!/bin/bash
2 mpiCC second.cpp
3 rm for_graph.txt
4 for nodes in 2000 10000 50000
5 do
6 for pr in 1 2 3 4 5 6 7 8
7 do
8 mpiexec -np ${pr} ./a.out ${nodes} » for_graph.txt
9 done
10 echo -e "\n" » for_graph.txt
11 done
