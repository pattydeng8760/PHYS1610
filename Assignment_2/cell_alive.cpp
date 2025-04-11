#ifndef CELL_ALIVE_H
#define CELL_ALIVE_H

#include "rarray"  // Include the rarray library

bool is_cell_alive_next(const rvector<bool>& cell_alive, int index)
{
    int num_cells = cell_alive.size();
    int alive_neighbours = 0;
    for (int offset = -2; offset <= 2; offset++)
        if (offset != 0 and cell_alive[(index+offset+num_cells)%num_cells]) // modulus (%) enforces periodic boundary conditions
            alive_neighbours++;
    if (cell_alive[index] and
        (alive_neighbours == 2 or alive_neighbours == 4))
        return true;
    else if (not cell_alive[index] and
             (alive_neighbours == 2 or alive_neighbours == 3))
        return true;
    else
        return false;
}

#endif