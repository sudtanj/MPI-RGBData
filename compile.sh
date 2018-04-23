#!/bin/bash
mpicc ./src/$1.c -o ./build/$1
mpirun -n $2 ./build/$1 | tee ./output/$1