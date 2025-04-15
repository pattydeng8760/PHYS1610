# Revised Makefile for Assignment 10
# Finite Difference Solver to the 3D KPP-Fisher Equation with OMP and MPI optimization
# Part of assignment 10 of PHY1610H, Winter 2025
# (Modules required: gcc/12.3, rarray/2.8.0, boost/1.85.0, openmpi/4.1.5)

# This Makefile builds two executables:
#   pkkfisher3d_hybrid        - MPI+OpenMP parallelized with I/O
#   pkkfisher3d               - MPI parallelized with I/O only

# The executible for MPI configuration
MPI_Exe = pkkfisher3d

# The executable for hybrid (MPI+OpenMP) configuration
MPI_OMP_Exe = pkkfisher3d_hybrid

CXX = mpic++
CXXFLAGS = -g -O3 -march=native -Wall -Wfatal-errors
CXXFLAGS_omp = -fopenmp -g -O3 -march=native -Wall -Wfatal-errors
LDFLAGS_omp = -fopenmp
LDLIBS = -lboost_program_options
TIME = /usr/bin/time -f %es
RUNOPTIONS = -P 10 -L 15.0 -A 0.2 -N 100 -T 10 -D 0.001 -F

all: $(MPI_OMP_Exe) $(MPI_Exe)

# Build the hybrid (MPI+OpenMP) executable
$(MPI_OMP_Exe): pkkfisher3d_hybrid.o output_hybrid.o readcommandline.o ticktock.o
	$(CXX) $(LDFLAGS_omp) -o $@ $^ $(LDLIBS)

# Build the MPI-only executable
$(MPI_Exe): pkkfisher3d.o output.o readcommandline.o ticktock.o
	$(CXX) -o $@ $^ $(LDLIBS)

pkkfisher3d_hybrid.o: pkkfisher3d_hybrid.cpp params.h output_hybrid.h
	$(CXX) $(CXXFLAGS_omp) -c -o $@ $<

pkkfisher3d.o: pkkfisher3d.cpp params.h output.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

output_hybrid.o: output_hybrid.cpp output_hybrid.h
	$(CXX) $(CXXFLAGS_omp) -c -o $@ $<

output.o: output.cpp output.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

readcommandline.o: readcommandline.cpp readcommandline.h params.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

ticktock.o: ticktock.cpp ticktock.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) output.o output_hybrid.o readcommandline.o pkkfisher3d.o $(MPI_Exe) \
           pkkfisher3d_hybrid.o $(MPI_OMP_Exe) output1.dat output4.dat ticktock.o \
           output1_hybrid.dat output4_hybrid.dat

.PHONY: all run run_hybrid clean

# Run targets for testing the executables (MPI-only version)
run: $(MPI_Exe)
	$(TIME) mpirun -np 1 ./$(MPI_Exe) $(RUNOPTIONS) output1.dat
	$(TIME) mpirun -np 4 ./$(MPI_Exe) $(RUNOPTIONS) output4.dat
	diff -q output1.dat output4.dat

# Run targets for testing the hybrid version (MPI+OpenMP)
run_hybrid: $(MPI_OMP_Exe)
	# Set OMP_NUM_THREADS to an appropriate value, e.g., 2
	export OMP_NUM_THREADS=2; \
	$(TIME) mpirun -np 1 ./$(MPI_OMP_Exe) $(RUNOPTIONS) output1_hybrid.dat; \
	$(TIME) mpirun -np 4 ./$(MPI_OMP_Exe) $(RUNOPTIONS) output4_hybrid.dat; \
	diff -q output1_hybrid.dat output4_hybrid.dat