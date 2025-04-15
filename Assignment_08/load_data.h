/// @file load_data.h
/// @brief Function for initizlizing the computation of the histogram of the logarithm of the step count.
/// @author Patrick Deng
/// @date March 25, 2025
/// This file contains the functions for loading the data from the file 

#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <fstream>              /// Library for file input/output
#include <iostream>             /// Library for standard input/output
#include <string>               /// Library for string manipulation
#include <cstdlib>              /// Library for exit() function
#include <rarray>               /// Library for rarray data structure
#include <omp.h>                /// Library for OpenMP to parallelize the i/o

/// Function that loads the data from the file and stores it in the step_count vector
/// @param filename Name of the input file
rvector<int> load_data(std::string filename);

#endif
