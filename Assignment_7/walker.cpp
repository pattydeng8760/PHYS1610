/// @file walker.cpp
/// @brief Function for simulating the walker moving through the porous media grid
/// @author Patrick Deng
/// @date March 19, 2025
/// This file contains the function for simulating the walker moving through the porous media grid.

# include <random>
# include <rarray>
# include "check_state.h"


/// @brief Function for simulating a single walker moving through the porous media grid, returns true if the walker reaches the bottom row, false otherwise.
/// @param grid The grid representing the porous media.
/// @param g The parameter g for the walker.
/// @param maxSteps The maximum number of steps the walker can take.
/// @param startRow The starting row of the walker.
/// @param startCol The starting column of the walker.
/// @param rng The random number generator from ranlux24 from the main function.
bool simulate_walker(const rmatrix<bool>& grid, const double g, const int maxSteps, const int startRow, const int startCol, std::ranlux24 &rng) {
    int N = grid.extent(0);                // Number of rows in the grid
    //int M = grid.extent(1);                // Number of columns in the grid
    int currentRow = startRow;          // current row of the walker
    int currentCol = startCol;          // current column of the walker starting from the startRow and startCol
    //double z = 1;                       // Initialzing the weight of the move
    // Applying the timestepping process
    for (int step = 0; step < maxSteps; step++) {
        // Check if the walker has reached the bottom row.
        if (currentRow == N - 1)
            return true;
        double z = 1;
        // The move weights
        // weight left is 1.0/z if the cell to the left is accessible, otherwise 0.0
        double weightLeft  = check_state(grid, currentRow, currentCol - 1) ? 1.0/z : 0.0;
        // weight right is 1.0/z if the cell to the right is accessible, otherwise 0.0
        double weightRight = check_state(grid, currentRow, currentCol + 1) ? 1.0/z : 0.0;
        // weight up is 1.0/(g*z) if the cell above is accessible, otherwise 0.0
        double weightUp    = check_state(grid, currentRow - 1, currentCol) ? 1.0/(g*z) : 0.0;
        // weight down is g/z if the cell below is accessible, otherwise 0.0
        double weightDown  = check_state(grid, currentRow + 1, currentCol) ? g/z : 0.0;
        // The total weight of the move is the sum of the individual weights
        z = weightLeft + weightRight + weightUp + weightDown;

        // Check if the walker is trapped with no valid moves, i.e., z = 0
        if (z == 0.0)
            return false;

        // A uniform distribution for generating random numbers between 0 and z and a random number r will be used to determine the move
        std::uniform_real_distribution<double> dist(0.0, z);
        // Returns a random number between 0 and z in the distribution
        double r = dist(rng);

        // Move the walker based on the random number generated
        // If the random number is less than the weight to the left, move left
        if (r < weightLeft && weightLeft > 0) {
            currentCol -= 1;        // Move the walker left
        } else if (r < weightLeft + weightRight && weightRight > 0) {
            currentCol += 1;        // Move the walker right
        } else if (r < weightLeft + weightRight + weightUp && weightUp > 0) {
            currentRow -= 1;        // Move the walker up
        } else if (r < weightLeft + weightRight + weightUp + weightDown && weightDown > 0) {
            currentRow += 1;        // Move the walker down
        }
        else {
            // The walker is trapped with no valid moves
            return false;
        }
    }
    return false;  // maximum steps reached without reaching the bottom
}