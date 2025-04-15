/// @file init.h
/// @brief Header file for initializing the simulation parameters.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module initializes the simulation parameters
#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <iostream>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <rarray>

/// @file initialize.h
/// @brief Header file for initializing the simulation parameters.
/// @author Patrick Deng
/// @date March 3, 2025
/// @details This module initializes the simulation parameters and parses command-line arguments.

/// @brief Structure to store command-line inputs.
struct SimulationParams {
    double P;                   ///< Number of snapshots (save interval = T / P)
    double L;                   ///< Size of the domain [0, L]
    double A;                   ///< Amplitude of the initial condition
    int N;                      ///< Number of grid points
    double T;                   ///< Total simulation time
    double dt;                  ///< Time step size
};

/// @brief Parses command-line arguments and returns a SimulationParams struct.
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]);

/// @brief Writes the snapshot to a file.
/// @param filename The name of the file to write the snapshot to.
/// @param n The current time step.
/// @param time The current time.
/// @param L The size of the domain.
/// @param N The number of grid points.
/// @param u The solution vector.
/// @param snapshot_interval The interval to save the snapshot.
void write_snapshot_to_file(const std::string& filename, int n, double time, double L, int N, const rarray<double,1>& u, int snapshot_interval, int num_tsteps);

#endif // INITIALIZE_H
