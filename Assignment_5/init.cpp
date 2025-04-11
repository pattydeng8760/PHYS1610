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

/// @brief Function to convert a SolutionType enum to a string for printout
/// @param type The SolutionType enum class.
/// @return A string representation of the SolutionType.
std::string solution_type_to_string(SolutionType type) {
    switch (type) {
        case SolutionType::FULL_MATRIX:      return "Full_Matrix_Multiplication";
        case SolutionType::FULL_MATRIX_BLAS: return "Full_Matrix_BLAS";
        case SolutionType::BANDED_BLAS:       return "Banded_BLAS";
    }
    // Default should never be reached, but just in case:
    return "Full Matrix with BLAS for Euler Explicit";
}

/// @brief Function to convert a string to SolutionType enum
/// @param sol_type_str String representation of the solution type: "full", "full-blas", "banded-blas".
/// @return The SolutionType enum class corresponding to the string representation.
// Function to convert a string to SolutionType enum
SolutionType get_solution_type(const std::string& sol_type_str) {
    if (sol_type_str == "full")          return SolutionType::FULL_MATRIX;
    if (sol_type_str == "full-blas")     return SolutionType::FULL_MATRIX_BLAS;
    if (sol_type_str == "banded-blas")   return SolutionType::BANDED_BLAS;
    // Default to BLAS full if the string is unrecognized
    return SolutionType::FULL_MATRIX_BLAS;
}

/// @brief Initialize function to take in the command line arguments and return a SimulationParams struct
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line argument strings.
/// @return A struct containing parsed simulation parameters.
/// @throws std::runtime_error if any required argument is missing or invalid.
SimulationParams parse_arguments(int argc, char* argv[]) {
    // Default solution type is FULL_MATRIX_BLAS
    SimulationParams params = {-1, -1, -1, -1, -1, -1, SolutionType::FULL_MATRIX_BLAS};

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
        } else if (flag == "-sol") {
            params.solution_type = get_solution_type(value);
        } else {
            throw std::runtime_error("Error: Unknown flag " + flag);
        }
    }

    // Ensure all required parameters are provided
    if (params.P == -1 || params.L == -1 || params.A == -1 ||
        params.N == -1 || params.T == -1 || params.dt == -1) {
        throw std::runtime_error("Usage: ./diffusion_solver -P <snapshots> -L <length> -A <amplitude> "
                                 "-N <grid points> -T <total time> -dt <time step> -sol <solution_type>");
    }

    // Print out the settings
    std::cout <<"\n"<< std::string(30, '=')<< "Simulation Settings" << std::string(30, '=')<<"\n";
    std::cout << "\n" << "  Snapshots (P): " << params.P << "\n"
              << "  Length (L): " << params.L << "\n"
              << "  Amplitude (A): " << params.A << "\n"
              << "  Grid Points (N): " << params.N << "\n"
              << "  Total Time (T): " << params.T << "\n"
              << "  Time Step (dt): " << params.dt << "\n"
              << "  Solution Type: " << solution_type_to_string(params.solution_type) << "\n"
              << std::endl;

    return params;
}

/// @brief Function to check the stability of the explicit Euler method.
/// @param dt Time step size.
/// @param dx Grid spacing.
/// @return 0 if the method is stable, 1 if the method is unstable. Require dt < dx^2/(2*k).
int check_stability(const double dt, const double dx){
    double dt_lim = dx * dx/2;  // CFL condition for the diffusion equation
    if (dt > dt_lim) {
        std::cerr << "ERROR! \n Timestep is too Large!\n Explicit Euler is non-stable: dt = " << dt << " dt_lim=" << dt_lim << std::endl;
        return 1;
    }
    return 0;
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
