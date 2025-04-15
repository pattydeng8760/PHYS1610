#!/bin/bash
#SBATCH --job-name=mpi_hist_parallel
#SBATCH --nodes=3
#SBATCH --ntasks-per-node=10
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00
#SBATCH --output=log_mpi_omp_nodes_10_tasks_4_proc.out

# This script is used to submit a job to the SLURM scheduler for running a parallel MPI program without OMP
module load gcc/12.3 rarray/2.8.0 boost/1.85.0 openmpi/4.1.5
module list

exec="./pkkfisher3d_hybrid"
timer="/usr/bin/time"

# Set OpenMP threads to 4 per MPI task (3nodes*10 MPI tasks/nodes * 4 threads/tasks)
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
echo "Number of MPI processes: $SLURM_NTASKS"
echo "Number of OpenMP threads per MPI process: $OMP_NUM_THREADS"


# Launch MPI processes
"$timer" -f "%es" mpirun -np "$SLURM_NTASKS" "$exec" -P 10 -L 15.0 -A 0.2 -N 200 -T 10 -D 0.00125 -F output_omp_${SLURM_NTASKS}nodes_${SLURM_CPUS_PER_TASK}threads.dat
