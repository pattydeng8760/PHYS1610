# Percolation Simulation

This project simulates a percolation model on a two-dimensional porous media grid. Each cell in the grid is either empty or filled, with the probability of a cell being empty specified by the parameter `p`. Walkers are introduced at every empty cell in the top row and attempt to traverse the grid. The walker's movement is biased by a parameter `g` (with `g > 1`), which increases the probability of moving downward. The simulation tracks the fraction of walkers that reach the bottom of the grid.

## Project Structure

- **init.cpp / init.h:**  
  Parses command-line arguments and initializes simulation parameters. Expected parameters include grid dimensions, empty-cell probability, bias parameter, number of walkers per starting cell, step multiplier, and a fixed seed for reproducibility.

- **generate_grid.cpp / generate_grid.h:**  
  Generates the porous media grid. Each cell is set to `true` (empty) with probability `p` using a uniform random distribution.

- **walker.cpp / walker.h:**  
  Contains the function that simulates a single walker moving through the grid. The walker moves based on weighted probabilities (left, right, up, and down) determined by whether a move is possible and the bias parameter `g`.

- **check_state.cpp / check_state.h:**  
  Provides helper functions (such as `check_state`) to determine if a move to a given cell is allowed (i.e., within the grid bounds and the cell is empty).

- **main.cpp:**  
  The driver of the simulation. It initializes the grid, seeds the random number generator with a specific seed for reproducibility, launches walkers from each empty top-row cell, and then calculates and prints the fraction of walkers that reach the bottom.

## Compilation

Ensure that you have a C++ compiler with C++17 support and that the `rarray` library is available. To compile the project and run on the default setting:

```bash
make all; make run
```

To run a custom setting:

```bash
./percolation_solver -M 200 -N 200 -p 0.7 -g 2 -K 25 -S 20 -seed 1234
```

## Expected Output
==============================Simulating Percolation==============================
  number of cell width (M): 200
  number of cell height (N): 200
  cell empty probability (p): 0.7
  downwards tendency (g): 2.0
  number of walkers (K): 25
  maximum allowed time coefficient (S): 20

Number of walkers that reached the bottom: <number>
Total number of walkers: <number>
Fraction of walkers that reached the bottom: <fraction>

==============================Simulation Complete==============================

