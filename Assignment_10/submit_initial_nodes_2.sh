#!/bin/bash
#SBATCH --job-name=mpi_hist_parallel
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=40
#SBATCH --time=00:10:00
#SBATCH --output=log_mpi_2_nodes_80_proc.out

# This script is used to submit a job to the SLURM scheduler for running a parallel MPI program without OMP
module load gcc/12.3 rarray/2.8.0 boost/1.85.0 openmpi/4.1.5
module list

exec="./pkkfisher3d"
timer="/usr/bin/time"
"$timer" -f "%es" mpirun -np 80 "$exec" -P 10 -L 15.0 -A 0.2 -N 200 -T 10 -D 0.00125 -F output_mpi_80.dat
