// gameof1d.cpp
//
// This code computes the evolution of a one-dimensional variant of Conway's Game of Life,
// as conceived by Jonathan K. Millen and published in BYTE magazine in December, 1978.
//
// This system is a linear set of cells that are either "alive" or "dead".
// Time in this system progresses in discrete steps.
//
// The state of each cell at the next time step depends on its present
// state and that of its neighbors, in the following way:
//
//   - First, count the number of alive neighbors at most 2 cells away
//   - An alive cell stays alive if that count is 2 or 4, else it dies
//   - A dead cell becomes alive if that count is 2 or 3, else it stays dead.
//
// Since the first two and the last two cells would not have enough neighbours to apply
// this rule, we use cells on the other side as neighbours, i.e., 'periodic boundaries'.
//
// The initial state of this system is constructed with a given fraction of alive
// cells which are (nearly) equally spaced among dead cells.
//
// Without command line arguments, the program computes the time
// evolution for 120 steps, and for each step, prints out a line with
// a representation of the state and fraction of alive cells.
//
// This code is part of assignment 2 of the 2025 Winter PHY1610 course.
//
// Ramses van Zon, 2023-2025, University of Toronto

#include "init.h"
#include "cell_alive.h"
#include "cell_print.h"
#include "cell_update.h"
//#include <iostream>
//#include "rarray" // rarray is a local library, thus the quotes 
//
// Determine the next alive status of the cell at location 'index'
// within the linear set of cells 'cell_alive', based on the alive
// status of its neighboring cells.
//
//bool is_cell_alive_next(const rvector<bool>& cell_alive, int index);
//std::pair <double,double> initialize_cells(rvector<bool>& alive_status, double& target_fraction);
//void update_cells(rvector<bool>& alive_status, double& fraction);
//void print_cells(int step, rvector<bool>& alive_status, double& fraction);

int main(int argc, char* argv[])
{
    // Set default simulation parameters then accept command line arguments
    int num_cells = 70;
    int num_steps = 120;
    double target_fraction = 0.35;
    try {
        if (argc > 1)
            num_cells = std::stoi(argv[1]);
        if (argc > 2)
            num_steps = std::stoi(argv[2]);
        if (argc > 3)
            target_fraction = std::stod(argv[3]);
    } catch(...) {
        std::cout <<
            "Computes a 1d version of Conway's game of life\n\n"
            "Usage:\n"
            "  gameof1d [-h | --help] | [NUMCELLS [NUMSTEPS [FRACTION]]]\n\n";
        if (std::string(argv[1]) != "-h" and std::string(argv[1]) != "--help") {
            std::cerr << "Error in arguments!\n";
            return 1;
        } else
            return 0;
    }
    // Simulation system is just the alive status of each cell
    rvector<bool> alive_status(num_cells);
    double fraction = initialize_cells(alive_status, target_fraction);        // Initialize cells
    // Output time step, alive of cells, and fraction of alive cells
    print_cells(0, alive_status, fraction);                 // The first time step t=0
    // Time evolution loop
    for (int step = 1; step <= num_steps; step++) {
        update_cells(alive_status, fraction);                // Update cells
        print_cells(step, alive_status, fraction);          // output the state of the system
    }
} // end main





