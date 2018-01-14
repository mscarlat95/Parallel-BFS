#!/bin/bash

#module load compilers/gnu-5.4.0 
#module load libraries/openmpi-2.0.1-gcc-5.4.0

LOCATION=`pwd`
NUM_THREADS="16"

echo -e "\t\tSerial\t\t"
cd "$LOCATION/serial"
make -s clean && make -s
time make run

echo -e "\n\t\tOMP\t\t"
cd "$LOCATION/parallel/omp"
make -s clean && make -s
time make ARGS=$NUM_THREADS run

echo -e "\n\t\tPosix Threads\t\t"
cd "$LOCATION/parallel/pthread"
make -s clean && make -s
time make ARGS=$NUM_THREADS run

echo -e "\n\t\tMPI\t\t"
cd "$LOCATION/parallel/mpi"
make -s clean && make -s
time make ARGS=$NUM_THREADS run

echo -e "\n\t\tHybrid (MPI + OMP)\t\t"
cd "$LOCATION/parallel/hybrid"
make -s clean && make -s
time make ARGS=$NUM_THREADS run

exit 0
