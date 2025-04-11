#!/bin/bash
#SBATCH --job-name=mpi_hist_scatter
#SBATCH --nodes=2
#SBATCH --ntasks=80
#SBATCH --time=00:10:00
#SBATCH --output=log_mpi_hist_scatter_%j.out

module load gcc/13 openmpi/5 rarray
module list

base=1.1
batch=100000
filename="morestepnumbers_1.7GB.dat"
# Run for different process counts:
mpirun -np 1 ./mpi_scatter -base $base -filename $filename -batch $batch
mpirun -np 8 ./mpi_scatter -base $base -filename $filename -batch $batch
mpirun -np 20 ./mpi_scatter -base $base -filename $filename -batch $batch
mpirun -np 40 ./mpi_scatter -base $base -filename $filename -batch $batch
mpirun -np 80 ./mpi_scatter -base $base -filename $filename -batch $batch
