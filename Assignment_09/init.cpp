///@file init.cpp
///@author Patrick Deng
///@date April 3, 2025
///@brief This file contains the implementation of the function to parse command-line arguments for the MPI histogram program.

#include <stdexcept>
#include <iostream>
#include "init.h"

///@param base The base of the logarithm 
///@param file The filename of the input data
///@param batch_size The batchsize for each process to compute at a time
// Function to parse command-line arguments (executed on rank 0).
SimulationParams parse_arguments(int argc, char* argv[]) {
    SimulationParams params;
    params.base = -1;
    params.file = "";
    params.batch_size = -1;
    for (int i = 1; i < argc; i++) {
        std::string flag(argv[i]);
        if (i+1 >= argc) {
            throw std::runtime_error("Missing value for " + flag);
        }
        std::string value(argv[++i]);
        if (flag == "-base") {
            params.base = std::stof(value);
        } else if (flag == "-filename") {
            params.file = value;
        } else if (flag == "-batch") {
            params.batch_size = std::stoi(value);
        } else {
            throw std::runtime_error("Unknown flag " + flag);
        }
    }
    if (params.base <= 0 || params.file.empty() || params.batch_size <= 0) {
        throw std::runtime_error("Usage: mpirun -np <procs> ./mpi_hist_scatter -base <log_base> -filename <data_file> -batch <batch_size>");
    }
    std::cout << "Using base: " << params.base << "\nFile: " << params.file 
              << "\nBatch size: " << params.batch_size << std::endl;
    return params;
}