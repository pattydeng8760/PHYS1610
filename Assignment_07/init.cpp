/// @file init.cpp
/// @brief Function for initizlizing the porous media simulation 
/// @author Patrick Deng
/// @date March 19, 2025
/// This file contains the functions for initializing the porous media simulation.

#include "init.h"
#include <random>
#include <iostream>
#include <stdexcept>
#include <rarray>


/// @brief Initialize function to take in the command line arguments and return a SimulationParams struct
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]) {
    // Initialize the simulation parameters with default values
    SimulationParams params = {-1, -1, -1, -1, -1, -1, -1};

    for (int i = 1; i < argc; i++) {
        if (i + 1 >= argc) {
            throw std::runtime_error("Error: Missing value for " + std::string(argv[i]));
        }
        std::string flag = argv[i];
        std::string value = argv[++i];  // Get next argument as value

        if (flag == "-M") {
            params.M = std::stoi(value);
        } else if (flag == "-N") {
            params.N = std::stoi(value);
        } else if (flag == "-p") {
            params.p = std::stod(value);
        } else if (flag == "-g") {
            params.g = std::stod(value);
        } else if (flag == "-K") {
            params.K = std::stoi(value);
        } else if (flag == "-S") {
            params.S = std::stoi(value);
        } else if (flag == "-seed") {
            params.seed = std::stoi(value);
        }
        else {
            throw std::runtime_error("Error: Unknown flag " + flag);
        }
    }

    // Ensure all required parameters are provided
    if (params.M == -1 || params.N == -1 || params.p == -1 ||
        params.g == -1 || params.K == -1 || params.S == -1 || params.seed == -1) {
        throw std::runtime_error("Usage: ./percolation_solver -M <number of cell width> -N <number of cell height> -p <cell empty probability> "
                                 "-g <downwards tendency> -K <number of walkers> -S <multiplier for maximum number of steps allowed for each walker>"
                                 "-seed <seed for random number generator>");
    }

    // Appying checks to all the parameters to ensure robustness
    // Check if the number of cell width is a positive integer
    if (params.M <= 0) {
        throw std::runtime_error("Error: Number of cell width must be a positive integer");
    }
    if (params.N <= 0) {
        throw std::runtime_error("Error: Number of cell height must be a positive integer");
    }
    if (params.g < 0) {
        throw std::runtime_error("Error: Downwards tendency must be a non-negative integer");
    }
    if (params.K <= 0) {
        throw std::runtime_error("Error: Number of walkers must be a positive integer");
    }
    if (params.p < 0 || params.p > 1) {
        throw std::runtime_error("Error: Cell empty probability must be between 0 and 1, non-inclusive");
    }
    if (params.S <= 0) {
        throw std::runtime_error("Error: Multiplier for maximum number of steps allowed for each walker must be a positive integer");
    }
    // Print out the settings
    std::cout << "\n" << "  number of cell width (M): " << params.M << "\n"
              << "  number of cell height (N): " << params.N << "\n"
              << "  cell empty probability (p): " << params.p << "\n"
              << "  downwards tendency (g): " << params.g << "\n"
              << "  number of walkers per empty top row cell (K): " << params.K << "\n"
              << "  maximum allowed time coefficient (S): " << params.S << "\n"
              << "  seed for random number generator: (seed): " << params.seed << "\n"
              << std::endl;
    return params;
}


