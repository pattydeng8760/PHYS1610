# Makefile for Assignment 5
#
# Part of assignment 5 of PHY1610H, Winter 2025
# load module load gcc/12.3 rarray/2.8.0 openblas/0.3.24

# Compiler, optimizations, and flags
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wfatal-errors -std=c++17
LDLIBS = -lfftw3

# Executable name
TARGET = pde_solver

# Source files
SRCS = Compute_PDE.cpp calc_diffusion.cpp calc_reaction.cpp init.cpp initial_boundary_condition.cpp
HDRS = calc_diffusion.h init.h initial_boundary_condition.h calc_reaction.h
OBJS = $(SRCS:.cpp=.o)

# Default target (build everything)
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compilation rules for source files
Compute_PDE.o: Compute_PDE.cpp initial_boundary_condition.h init.h calc_reaction.h calc_diffusion.h
	$(CXX) $(CXXFLAGS) -c -o $@ Compute_PDE.cpp

calc_diffusion.o: calc_diffusion.cpp calc_diffusion.h
	$(CXX) $(CXXFLAGS) -c -o $@ calc_diffusion.cpp

calc_reaction.o: calc_reaction.cpp calc_reaction.h
	$(CXX) $(CXXFLAGS) -c -o $@ calc_reaction.cpp

init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -c -o $@ init.cpp

initial_boundary_condition.o: initial_boundary_condition.cpp initial_boundary_condition.h
	$(CXX) $(CXXFLAGS) -c -o $@ initial_boundary_condition.cpp

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

doc: initial_boundary_condition.h init.h calc_reaction.h calc_diffusion.h Compute_PDE.cpp README.md
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
test: $(TARGET)
	./pde_solver -P 400 -L 5 -A 1 -N 100 -T 10 -dt 0.01