/// @file generate_grid.cpp
/// @brief Function for generating the porous media grid for the simulation
/// @author Patrick Deng
/// @date March 19, 2025
/// This file contains the function for generating the porous media grid for the simulation.

# include <random>
# include <rarray>
# include "generate_grid.h"

/// Initializes the simulation by generating the porous media grid and setting the initial state of the simulation.
/// Each cell is set to true (empty) with probability p.
/// @param N The number of rows in the grid.
/// @param M The number of columns in the grid.
/// @param p The probability that a cell is empty.
/// @param rng The random number generator.
/// @param grid The grid to be generated.
void generate_grid(const int N, const int M, const double p, std::ranlux24 &rng, rmatrix<bool> &grid) {
    // Using the standard uniform distribution to generate random numbers between 0 and 1
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    // Applyign the probability p to each cell in the grid
    // Looping over the rows
    for (int i = 0; i < N; i++) {
        // Looping over the columns
        for (int j = 0; j < M; j++) {
            // The cell is empty with probability p if the random number generated is less than p
            grid[i][j] = (dist(rng) < p); 
        }
    }
}

// End of file


