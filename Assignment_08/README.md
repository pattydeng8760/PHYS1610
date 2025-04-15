# Assignment 8: Histogram of Walker Steps

## Overview
This assignment reads a data file (`stepnumbers.dat`) containing fixed-width integer records, computes the logarithm (in a user-specified base) of each step count, and produces a histogram. The data loading is parallelized via OpenMP, as is the computation and histogram filling.

## File Structure

- **Makefile**  
  Compiles the program with OpenMP and produces the executable `calc_hist`.

- **calc_hist.cpp**  
  The main program. It:
  1. Parses command-line arguments (log base and filename).  
  2. Loads data from file (using `load_data.cpp`).  
  3. Computes log of each step count in parallel.  
  4. Finds min and max of the log values in parallel.  
  5. Allocates and fills a histogram in parallel.  
  6. Outputs the histogram and timing information.

- **load_data.cpp**  
  Provides a function `load_data(std::string filename)` that:
  1. Opens the file in binary mode.  
  2. Reads its contents into a buffer.  
  3. Parallelizes the conversion of each record from text to an integer.

- **ticktock.cpp**  
  A small utility for measuring elapsed time, using C++’s `<chrono>` library.

- **init.cpp / init.h**  
  Functions and data structures used to parse and store command-line arguments (like the log base, filename, etc.).

- **SimulationParams**  
  A struct defined in `init.h` that holds input parameters such as `base` (logarithm base) and `file` (data filename).

## Usage

1. **Compile:**
```bash
   module load gcc rarray
   make all
```

2. **Test:**
```bash
   ./calc_hist -base <base> -filename stepnumbers.dat
```