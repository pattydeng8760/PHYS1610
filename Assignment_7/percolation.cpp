/// @file main.cpp
/// @brief Main function for the porous media simulation
/// @author Patrick Deng
/// @date March 19, 2025
/// This file contains the main function for the porous media simulation

#include <iostream>
#include <random>
#include <rarray>
#include "init.h"
#include "generate_grid.h"
#include "walker.h"
#include "check_state.h"

int main(int argc, char* argv[]) {
    std::cout <<"\n"<< std::string(30, '=')<< "Simulating Percolation" << std::string(30, '=')<<"\n";
    SimulationParams params;  // Declare the struct to store simulation parameters
    try {
        params = parse_arguments(argc, argv);  // Assign parsed parameters to struct from command-line arguments
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    // Assign parsed parameters to local variables
    int M = params.M;
    int N = params.N;
    double p = params.p;
    double g = params.g;
    int K = params.K;
    int S = params.S;
    int seed = params.seed;

    // Initialize the other intermediate variables
    int successful_walkers = 0;
    int total_walkers = 0;
    int max_steps = S * (M * M + N * N);
    rmatrix<bool> grid;                 // Declare the grid as a boolean rmatrix
    // Initialize the random number generator with the seed
    std::ranlux24 rng(seed);            // Using the  ranlux24 random number generator 

    // Initialize the grid as a boolean rmatrix of size N x M as the probability that the cell is empty
    grid = rmatrix<bool>(N, M);      
    generate_grid(N, M, p, rng, grid);  // Generate the grid with the given parameters
    
    // Loop over the top row of the grid to start the walkers at each non-empty cell
    for (int col = 0; col < M; col++) {
        // if the top row cell is empty, start walkers from here
        if (grid[0][col] == true) { 
            // Loop over the number of walkers for K times and return the number of successful walkers
            for (int k = 0; k < K; k++) {
                bool Bottom = simulate_walker(grid, g, max_steps, 0, col, rng);
                // Increment the total number of walkers
                total_walkers++;
                // Check if the walker reached the bottom row, if so, increment the successful walkers
                if (Bottom)
                    successful_walkers++;
            }
        }
    }
    // Calculate and display the fraction of walkers that reached the bottom.
    double fraction = (total_walkers > 0) ? static_cast<double>(successful_walkers) / total_walkers : 0.0;
    std::cout << "Total number of walkers simulated: " << total_walkers << std::endl;
    std::cout << "Number of walkers that reached the bottom of lattice: " << successful_walkers << std::endl;
    std::cout << "Fraction of walkers that reached the bottom of lattice: " << fraction << std::endl;
    std::cout <<"\n"<< std::string(30, '=')<< "Simulation Complete" << std::string(30, '=')<<"\n";
    return 0;
}   

