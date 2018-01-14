#!/bin/bash

module load compilers/gnu-5.4.0 
module load libraries/openmpi-2.0.1-gcc-5.4.0

LOCATION=`pwd`

echo -e "\t\tSerial\t\t"
cd "$LOCATION/serial"
make clean && make
time make run

echo -e "\n\t\tOMP\t\t"
cd "$LOCATION/parallel/omp"
make clean && make
time make run

echo -e "\n\t\tPosix Threads\t\t"
cd "$LOCATION/parallel/pthread"
make clean && make
time make run

echo -e "\n\t\tMPI\t\t"
cd "$LOCATION/parallel/mpi"
make clean && make
time make run

echo -e "\n\t\tHybrid (MPI + OMP)\t\t"
cd "$LOCATION/parallel/hybrid"
make clean && make
time make run

exit 0
