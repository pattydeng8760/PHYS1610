#include "init.h"
// Initialize the cells such that the fraction of alive cells is roughly target_fraction
double initialize_cells(rvector<bool>& alive_status, double& target_fraction)
{
    double fill = 0.0;
    for (bool& alive: alive_status) {
        fill += target_fraction;
        if (fill >= 1.0) {
            alive = true;
            fill -= 1.0;
        } else {
            alive = false;
        }
    };
    double fraction = std::count(alive_status.begin(), alive_status.end(), true) / double(alive_status.size());
    return fraction;
}