# Makefile for Assignment 8 
# Calculating the histogram of the logarithm of walker steps
# Part of assignment 8 of PHY1610H, Winter 2025
# load module load gcc/12.3 rarray/2.8.0 

# Compiler, optimizations, and flags
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wfatal-errors -std=c++17 -fopenmp
#LDLIBS =

# Executable name
TARGET = calc_hist

# Source files
SRCS = calc_hist.cpp load_data.cpp init.cpp ticktock.cpp
HDRS = load_data.h init.h ticktock.h
OBJS = $(SRCS:.cpp=.o)

# Default target (build everything)
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compilation rules for source files
init.o: init.cpp init.h							# initialization function
	$(CXX) $(CXXFLAGS) -c -o $@ init.cpp

load_data.o: load_data.cpp load_data.h			# data i/o function
	$(CXX) $(CXXFLAGS) -c -o $@ load_data.cpp

ticktock.o: ticktock.cpp ticktock.h				# timing function
	$(CXX) $(CXXFLAGS) -c -o $@ ticktock.cpp	

calc_hist.o: calc_hist.cpp load_data.h init.h	# main calculation function
	$(CXX) $(CXXFLAGS) -c -o $@ calc_hist.cpp

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET) -base 1.25 -filename stepnumbers.dat


