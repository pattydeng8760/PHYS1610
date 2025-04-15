#!/bin/bash
# This script compares multiple output files to verify that they are identical.
# All files in the FILES array are expected to have the same contents.
# Modify the FILES array below with the names of the files you wish to compare.

FILES=("output_omp_12nodes_10threads.dat" "output_omp_30nodes_4threads.dat" "output_omp_3nodes_40threads.dat" "output_omp_60nodes_2threads.dat" "output_omp_6nodes_20threads.dat" "output_omp_120nodes_1threads.dat")

# Use the first file in the list as the reference
BASE=("output_mpi_80.dat")
echo "Using reference file: $BASE"

# Loop over the rest of the files and compare each to the reference file
for file in "${FILES[@]:1}"; do
  if cmp --silent "$BASE" "$file"; then
    echo "File '$file' is identical to '$BASE'."
  else
    echo "File '$file' differs from '$BASE'."
  fi
done