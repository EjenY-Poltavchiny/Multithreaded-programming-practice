#!/bin/bash
mpiCC main.cpp
rm for_graph.txt
for nodes in 1000 1000000 100000000
do
for pr in 1 2 3 4 5 6 7 8
do
mpiexec -np ${pr} ./a.out ${nodes} >> for_graph.txt
done
echo -e "\n" >> for_graph.txt
done
