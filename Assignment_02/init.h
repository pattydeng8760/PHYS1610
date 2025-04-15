#ifndef INIT_H
#define INIT_H
// This is the header file for the gameof1d_init.cpp file.
#include "rarray"  // Include the rarray library
#include <utility> // For std::pair

// Function prototype (declaration)
double initialize_cells(rvector<bool>& alive_status, double& target_fraction);

#endif //INIT