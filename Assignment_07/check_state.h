/// @file check_state.h
/// @brief Header file for checking the state of the walker to see if it is still within the grid bound or if the cell is empty
/// @author Patrick Deng
/// @date March 19, 2025
#ifndef CHECK_STATE_H
#define CHECK_STATE_H

#include <rarray>
/// Checks the state of the walker to see if it is still within the grid bounds and if the cell is empty.
/// if the walker is within the grid bounds and the cell is empty, the function returns true.
/// Otherwise, the the grid bounds are exceeded or the cell is not empty, the function returns false.
/// @param grid The grid to be checked.
/// @param current_row The current row of the walker.
/// @param current_col The current column of the walker.
bool check_state(const rmatrix<bool>& grid, const int current_row, const int current_col);

#endif