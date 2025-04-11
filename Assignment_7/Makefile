# Makefile for Assignment 7
#
# Part of assignment 7 of PHY1610H, Winter 2025
# load module load gcc/12.3 rarray/2.8.0 

# Compiler, optimizations, and flags
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wfatal-errors -std=c++17
#LDLIBS =

# Executable name
TARGET = percolation_solver	

# Source files
SRCS = percolation.cpp init.cpp walker.cpp generate_grid.cpp check_state.cpp
HDRS = init.h walker.h generate_grid.h check_state.h
OBJS = $(SRCS:.cpp=.o)

# Default target (build everything)
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

# Compilation rules for source files
percolation.o: percolation.cpp init.h walker.h generate_grid.h check_state.h
	$(CXX) $(CXXFLAGS) -c -o $@ percolation.cpp

# Compilation rules for source files
init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -c -o $@ init.cpp

walker.o: walker.cpp walker.h
	$(CXX) $(CXXFLAGS) -c -o $@ walker.cpp

generate_grid.o: generate_grid.cpp generate_grid.h
	$(CXX) $(CXXFLAGS) -c -o $@ generate_grid.cpp

check_state.o: check_state.cpp check_state.h
	$(CXX) $(CXXFLAGS) -c -o $@ check_state.cpp

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# The standard case for running the program
run : $(TARGET)
	./$(TARGET) -M 200 -N 200 -p 0.7 -g 2 -K 25 -S 20 -seed 1234