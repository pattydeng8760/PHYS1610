/// @file generate_grid.h
/// @brief Header file for generating the porous media grid for the simulation
/// @author Patrick Deng
/// @date March 19, 2025
/// This module generates the porous media grid for the simulation.
#ifndef GENERATE_GRID_H
#define GENERATE_GRID_H

#include <random>
#include <rarray>

/// Initializes the simulation by generating the porous media grid and setting the initial state of the simulation.
/// Each cell is set to true (empty) with probability p.
/// @param N The number of rows in the grid.
/// @param M The number of columns in the grid.
/// @param p The probability that a cell is empty.
/// @param rng The random number generator with ranlux24 engine.
/// @param grid The grid to be generated, the grid is a boolean rmatrix of size N x M. each cell is set to true (empty) with probability p.
void generate_grid(const int N, const int M, const double p, std::ranlux24 &rng, rmatrix<bool> &grid);

#endif