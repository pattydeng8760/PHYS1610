/// @file init.h
/// @brief Header file for initializing the simulation parameters for the percolation solver
/// @author Patrick Deng
/// @date March 19, 2025
/// This module initializes the simulation parameters for the percolation solver.
#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <rarray>

/// @brief Structure to store command-line inputs.
/// @details This struct stores the command-line inputs for the simulation.
/// @param M Number of cell width
/// @param N Number of cell height
/// @param p Cell empty probability
/// @param g Downwards tendency
/// @param K Number of walkers
/// @param S Multiplier for maximum number of steps allowed for each walker
/// @param seed Seed for random number generator
struct SimulationParams {
    int M;                   ///< Number of cell width
    int N;                   ///< Number of cell height
    double p;                   ///< Cell empty probability
    double g;                   ///< Downwards tendency
    int K;                   ///< Number of walkers
    int S;                   ///< Multiplier for maximum number of steps allowed for each walker
    int seed;                ///< Seed for random number generator
};

/// @brief Parses command-line arguments and returns a SimulationParams struct.
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]);

#endif
