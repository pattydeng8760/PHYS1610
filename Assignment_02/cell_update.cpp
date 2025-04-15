#include "cell_update.h"
#include "cell_alive.h"

void update_cells(rvector<bool>& alive_status, double& fraction)
{
    rvector<bool> next_alive_status(alive_status.size());
    for (int i = 0; i < alive_status.size(); i++) 
        next_alive_status[i] = is_cell_alive_next(alive_status, i);
    std::swap(alive_status, next_alive_status);  
    fraction = std::count(alive_status.begin(), alive_status.end(), true)/ double(alive_status.size());
}

