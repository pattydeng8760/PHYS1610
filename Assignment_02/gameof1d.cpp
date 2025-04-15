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

#include <iostream>
#include "rarray" // rarray is a local library, thus the quotes 
//
// Determine the next alive status of the cell at location 'index'
// within the linear set of cells 'cell_alive', based on the alive
// status of its neighboring cells.
//
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
    // Fill cells such that the fraction of alive cells is roughly target_fraction
    double fill = 0.0;
    for (bool& alive: alive_status) {
        fill += target_fraction;
        if (fill >= 1.0) {
            alive = true;
            fill -= 1.0;
        } else {
            alive = false;
        }
    }
    // Output time step, alive of cells, and fraction of alive cells
    char alive_char = 'O';
    char dead_char = ' ';
    double fraction = std::count(alive_status.begin(), alive_status.end(), true)
                      / double(alive_status.size());
    std::string string_represention(alive_status.size(), ' ');
    for (int i = 0; i < alive_status.size(); i++)
        if (alive_status[i])
            string_represention[i] = alive_char;
        else
            string_represention[i] = dead_char;
    std::cout << 0 << "\t" << string_represention << " " << fraction << "\n";
    // Time evolution loop
    for (int step = 1; step <= num_steps; step++) {
        // Update cells
        rvector<bool> next_alive_status(alive_status.size());
        for (int i = 0; i < alive_status.size(); i++) 
            next_alive_status[i] = is_cell_alive_next(alive_status, i);
        // Make next status the current one using swap, which avoids a copy
        std::swap(alive_status, next_alive_status);  
        // Output time step, life status of cells, and fraction of alive cells
        fraction = std::count(alive_status.begin(), alive_status.end(), true)
                   / double(alive_status.size());
        for (int i = 0; i < num_cells; i++)
            if (alive_status[i]) 
                string_represention[i] = alive_char;
            else
                string_represention[i] = dead_char;
        std::cout << step << "\t" << string_represention << " " << fraction << "\n";
    }
} // end main

/*

Assignment 2:

Study the code, make sure you understand what's going on.  Make sure you can compile it, for which you will need to download rarray from https://raw.githubusercontent.com/vanzonr/rarray/refs/heads/main/rarray . Run it and save the initial output to a file so that you can check that the program still work as you start working on it.

Your task is to reorganize ('refactor') this code to be more modular. The aim is to have separate functionalities be implemented in separate functions. I.e., you should

  - Extract the part of the code that first fills the cell array with dead and alive cells, i.e., put that part of the code in a separate initialization function that then gets called from the main function; make sure the program still produce the same output.

  - Make this into a module, i.e., take out that initialization function and put it in a separate .cpp file and create a corresponding header file to be included in the original file.
  
  - Create a Makefile that can build the application.

  - Next, repeat these steps to create and use a module for the part of the code that performs a single time step.

  - Next, repeat these steps to create a module for the output part of the code.

  - Be sure to comment your code.
  
Your submission should consist of all source files, headerfiles, and the Makefile.  It is recommended to put them in a zip file or tar ball (but not rar).
    
*/
