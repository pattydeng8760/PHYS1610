# Makefile for Assignment 5
#
# Part of assignment 5 of PHY1610H, Winter 2025
# load module load gcc/12.3 rarray/2.8.0 openblas/0.3.24

# Compiler, optimizations, and flags
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wfatal-errors -std=c++17
LDLIBS = -lopenblas

# Executable name
TARGET = pde_solver

# Source files
SRCS = Compute_PDE.cpp diffusion_matrix.cpp init.cpp boundary_condition.cpp matrix_calc.cpp
HDRS = diffusion_matrix.h init.h boundary_condition.h matrix_calc.h
OBJS = $(SRCS:.cpp=.o)

# Default target (build everything)
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compilation rules for source files
Compute_PDE.o: Compute_PDE.cpp diffusion_matrix.h init.h boundary_condition.h matrix_calc.h
	$(CXX) $(CXXFLAGS) -c -o $@ Compute_PDE.cpp

diffusion_matrix.o: diffusion_matrix.cpp diffusion_matrix.h
	$(CXX) $(CXXFLAGS) -c -o $@ diffusion_matrix.cpp

init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -c -o $@ init.cpp

boundary_condition.o: boundary_condition.cpp boundary_condition.h
	$(CXX) $(CXXFLAGS) -c -o $@ boundary_condition.cpp

matrix_calc.o: matrix_calc.cpp matrix_calc.h
	$(CXX) $(CXXFLAGS) -c -o $@ matrix_calc.cpp

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

doc: diffusion_matrix.h init.h boundary_condition.h matrix_calc.h Compute_PDE.cpp README.md
	@echo "Generating Doxyfile if it doesn't exist..."
	@if [ ! -f Doxyfile ]; then doxygen -g; fi
	@echo "Configuring Doxyfile..."
	sed -i 's/^\(PROJECT_NAME[ ]*=[ ]*\).*/\1pde_solver/' Doxyfile
	sed -i 's/^\(OUTPUT_LANGUAGE[ ]*=[ ]*\).*/\1English/' Doxyfile
	sed -i 's/^\(USE_MDFILE_AS_MAINPAGE[ ]*=[ ]*\).*/\1README.md/' Doxyfile
	@echo "Running Doxygen..."
	doxygen
	make -C latex
	clear
	@echo "Documentation generated in ./html/ and ./latex/"
docclean:
	rm -rf docs/html docs/latex

# Test rule to run the program with the given parameters in the assignment
test_full: $(TARGET)
	./pde_solver -P 400 -L 5 -A 0.2 -N 100 -T 10 -dt 0.001 -sol full
test_BLAS_full: $(TARGET)
	./pde_solver -P 400 -L 5 -A 0.2 -N 100 -T 10 -dt 0.001 -sol full-blas
test_BLAS_banded: $(TARGET)
	./pde_solver -P 400 -L 5 -A 0.2 -N 100 -T 10 -dt 0.001 -sol banded-blas

# test all the test cases
test_all: test_full test_BLAS_full test_BLAS_banded