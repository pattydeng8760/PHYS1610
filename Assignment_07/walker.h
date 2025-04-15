/// @file walker.h
/// @brief Header file for a single walker moving through the porous media grid
/// @author Patrick Deng
/// @date March 19, 2025
/// This module contains the function for simulating a single walker moving through the porous media grid.

#ifndef WALKER_H
#define WALKER_H

#include <random>
#include <rarray>
#include "check_state.h"

/// @brief Function for simulating a single walker moving through the porous media grid, returns true if the walker reaches the bottom row, false otherwise.
/// @param grid The grid representing the porous media.
/// @param g The parameter g for the walker.
/// @param maxSteps The maximum number of steps the walker can take.
/// @param startRow The starting row of the walker.
/// @param startCol The starting column of the walker.
/// @param rng The random number generator from ranlux24 from the main function.
bool simulate_walker(const rmatrix<bool>& grid, const double g, const int maxSteps, const int startRow, const int startCol, std::ranlux24 &rng);

#endif