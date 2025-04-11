/// @file init.cpp
/// @brief Function for initizlizing the computation of the histogram of the logarithm of the step count.
/// @author Patrick Deng
/// @date March 25, 2025
/// This file contains the functions for initializing the porous media simulation.

#include "init.h"

/// @brief Initialize function to take in the command line arguments and return a SimulationParams struct
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]) {
    // Initialize the simulation parameters with default values
    SimulationParams params = {-1, "-1"};
    // Assign the values to the parameters
    for (int i = 1; i < argc; i++) {
        if (i + 1 >= argc) {
            throw std::runtime_error("Error: Missing value for " + std::string(argv[i]));
        }
        std::string flag = argv[i];
        std::string value = argv[++i];  // Get next argument as value

        if (flag == "-base") {
            params.base = std::stof(value);
        } else if (flag == "-filename") {
            params.file = value;
        }
        else {
            throw std::runtime_error("Error: Unknown flag " + flag);
        }
    }

    // Ensure all required parameters are provided
    if (params.base == -1 || params.file == "-1" ) {
        throw std::runtime_error("Usage: ./hist_calc -base <base of the logarithm> -filename <filename of the input file>");
    }
    // Appying checks to all the parameters to ensure robustness
    // Check if the number of cell width is a positive integer
    if (params.base <= 0) {
        throw std::runtime_error("Error: the base of the logarithm must be a positive integer");
    }
    // Print out the settings
    std::cout << "\n" << "  logarithm base (base): " << params.base << "\n"
              << "  filename (filename): " << params.file << "\n"
              << std::endl;
    return params;
}

