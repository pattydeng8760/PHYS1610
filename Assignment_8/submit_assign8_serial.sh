#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=01:00:00
#SBATCH --output=cal_hist_serial.log

module load gcc rarray
module list 
echo "C++ Modules loaded"

# Change to the source directory
source_dir="/home/l/lcl_uotphy1610/lcl_uotphy1610s1050/PHYS_1060/Assignment_8/"
cd "$source_dir"

# Build the executable
make all

export OMP_NUM_THREADS=1
echo "Number of threads: $OMP_NUM_THREADS"

{ time ./calc_hist -base 1.25 -filename stepnumbers.dat; } 2>&1