/// @file init.h
/// @brief Header file for initializing the computation of the histogram of the logarithm of the step count.
/// @author Patrick Deng
/// @date March 25, 2025
/// This module initializes the simulation parameters
#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <rarray>

/// @brief Structure to store command-line inputs.
/// @details This struct stores the command-line inputs for the simulation.
/// @param base Base of the logarithm
/// @param file Name of the input file
struct SimulationParams {
    double base;
    std::string file;
};

/// @brief Parses command-line arguments and returns a SimulationParams struct.
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]);

#endif




