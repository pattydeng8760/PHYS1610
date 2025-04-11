/// @file check_state.cpp
/// @brief Function for checking the state of the walker to see if it is still within the grid bound or if the cell is empty
/// @author Patrick Deng
/// @date March 19, 2025

# include <rarray>
# include "check_state.h"

/// Checks the state of the walker to see if it is still within the grid bounds and if the cell is empty.
/// if the walker is within the grid bounds and the cell is empty, the function returns true.
/// Otherwise, the the grid bounds are exceeded or the cell is not empty, the function returns false.
/// @param grid The grid to be checked.
/// @param current_row The current row of the walker.
/// @param current_col The current column of the walker.
bool check_state(const rmatrix<bool>& grid, const int current_row, const int current_col) {
    // Get the number of rows and columns in the grid
    int N = grid.extent(0);
    int M = grid.extent(1);
    // Check if the row index is within the domain of the grid
    if (current_row < 0 || current_row >= N) {
        return false;
    }
    // Check if the column index is within the domain of the grid
    if (current_col < 0 || current_col >= M) {
        return false;
    }
    // Check if the cell at (current_row, current_col) is empty 
    if (!grid[current_row][current_col]) {
        return false;
    }
    // All checks passed; the cell is accessible
    return true;
}


