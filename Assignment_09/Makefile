# Makefile for Assignment 9
# Calculating the histogram of the logarithm of walker steps
# Part of assignment 9 of PHY1610H, Winter 2025
# load module load gcc/12.3 rarray/2.8.0 openmpi/5.0.3

# Makefile for MPI histogram programs
# Compiles two executables:
#   mpi_scatter      - Root reads file in batches and scatters data.
#   mpi_parallel_io  - All processes perform parallel I/O.
#
# Use the mpicxx compiler with OpenMP enabled.
# Updated Makefile for MPI histogram programs with init.cpp included

CXX = mpicxx
CXXFLAGS = -O3 -std=c++17 -fopenmp

# Source files for each executable
SCATTER_SRC = mpi_scatter.cpp init.cpp ticktock.cpp
PARALLEL_IO_SRC = mpi_parallel_io.cpp init.cpp ticktock.cpp

# Target executable names
SCATTER_EXE = mpi_scatter
PARALLEL_IO_EXE = mpi_parallel_io

.PHONY: all clean

# The filename to be used in the test
all: $(SCATTER_EXE) $(PARALLEL_IO_EXE)


# The executables for scatter and parallel I/O
$(SCATTER_EXE): $(SCATTER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_IO_EXE): $(PARALLEL_IO_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# cleanup the executables
clean:
	rm -f $(SCATTER_EXE) $(PARALLEL_IO_EXE)

# testcases with 1 process
test:  
	mpirun -np 1 ./$(SCATTER_EXE) -base 1.1 -filename $filename -batch 100000
	mpirun -np 1 ./$(PARALLEL_IO_EXE) -base 1.1 -filename $filename -batch 100000