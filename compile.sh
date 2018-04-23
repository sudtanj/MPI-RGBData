#!/bin/bash
mpicc ./src/$1.c -o ./build/$1
cd build
mpirun -n $2 ./$1 