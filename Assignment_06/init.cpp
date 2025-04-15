/// @file init.cpp
/// @brief Source file for initializing the simulation parameters.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module initializes the simulation parameters and parses command-line arguments.

#include "init.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <fstream>

/// @brief Initialize function to take in the command line arguments and return a SimulationParams struct
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]) {
    // Default solution type is FULL_MATRIX_BLAS
    SimulationParams params = {-1, -1, -1, -1, -1, -1};

    for (int i = 1; i < argc; i++) {
        if (i + 1 >= argc) {
            throw std::runtime_error("Error: Missing value for " + std::string(argv[i]));
        }

        std::string flag = argv[i];
        std::string value = argv[++i];  // Get next argument as value

        if (flag == "-P") {
            params.P = std::stod(value);
        } else if (flag == "-L") {
            params.L = std::stod(value);
        } else if (flag == "-A") {
            params.A = std::stod(value);
        } else if (flag == "-N") {
            params.N = std::stoi(value);
        } else if (flag == "-T") {
            params.T = std::stod(value);
        } else if (flag == "-dt") {
            params.dt = std::stod(value);
        } else {
            throw std::runtime_error("Error: Unknown flag " + flag);
        }
    }

    // Ensure all required parameters are provided
    if (params.P == -1 || params.L == -1 || params.A == -1 ||
        params.N == -1 || params.T == -1 || params.dt == -1) {
        throw std::runtime_error("Usage: ./diffusion_solver -P <snapshots> -L <length> -A <amplitude> "
                                 "-N <grid points> -T <total time> -dt <time step>");
    }

    // Print out the settings
    std::cout <<"\n"<< std::string(30, '=')<< "Simulation Settings" << std::string(30, '=')<<"\n";
    std::cout << "\n" << "  Snapshots (P): " << params.P << "\n"
              << "  Length (L): " << params.L << "\n"
              << "  Amplitude (A): " << params.A << "\n"
              << "  Grid Points (N): " << params.N << "\n"
              << "  Total Time (T): " << params.T << "\n"
              << "  Time Step (dt): " << params.dt << "\n"
              << std::endl;

    return params;
}


/// @brief Function to write the snapshot to a file
/// @param filename The name of the file to write the snapshot to.
/// @param n The current time step.
/// @param time The current time.
/// @param L The size of the domain.
/// @param N The number of grid points.
/// @param u The solution vector.
/// @param snapshot_interval The interval to write the snapshot.
/// @param num_tsteps The total number of time steps.
void write_snapshot_to_file(const std::string& filename, int n, double time, double L, int N, const rarray<double,1>& u, int snapshot_interval, int num_tsteps) {
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    file << "Snapshot at time t = " << time << "\n";
    for (int i = 0; i < N; ++i) {
        double x_position = L * i / N;
        file << "  x = " << x_position << ", u(x,t) = " << u[i] << "\n";
    }
    file << "\n";

    file.close();
}
