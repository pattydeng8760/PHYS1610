# Assignment 5: Simulation of KPP-Fisher's Equation

## Overview
This assignment involves the numerical simulation of the **KPP-Fisher's equation**:

$$
\frac{\partial u}{\partial t} = u(1-u) + \frac{\partial^2 u}{\partial x^2}
$$

This equation describes a diffusion process with a non-linear reaction term.

## Problem Description

### Domain and Boundary Conditions
- **Spatial Domain:** $$ x \in [0, L] $$
- **Boundary Conditions:**
  - $$ u(0, t) = A \sin^2(t) $$ for all $$ t $$
  - $$ u(L, t) = 0 $$ for all $$ t $$
- **Initial Condition:**
  - $$ u(x, 0) = 0 $$

### Numerical Method
The simulation uses an **explicit time-stepping scheme (Forward Euler)**:
- The spatial interval is discretized into $$ N $$ points.
- Time is advanced with a timestep of $$ \Delta t $$ until the final time $$ T $$.
- During the simulation, $$ P $$ snapshots are printed. Each output line contains three space-separated values: the time, the position, and the value of $$ u $$.

### Input Parameters
The following parameters are input to the code:
- **P**: Number of snapshots.
- **L**: Length of the spatial domain.
- **A**: Amplitude at $$ x = 0 $$ for the boundary condition.
- **N**: Number of grid points.
- **T**: Total simulation time.
- **$$\Delta t$$**: Time step size.
- **sol**: Type of solution method: full matrix (full), full matrix with BLAS (full-blas), or banded matrix with BLAS (banded-blas), default is full matrix with BLAS

For the main test case, use:
- $$ L = 5 $$
- $$ A = 0.2 $$
- $$ N = 100 $$
- $$ T = 10 $$
- $$ P = 400 $$
- sol = full-blas
The timestep $$ \Delta t $$ must be chosen to ensure numerical stability.

### Implementation Variants
You are required to write **three codes** for this assignment:

1. **Full Matrix with Explicit Triple Loop:**
   - Use a full matrix representation.
   - Implement matrix-vector multiplication with explicit triple loop.
2. **Full Matrix with BLAS:**
   - Use a full matrix.
   - Use a BLAS routine (OpenBLAS) for matrix-vector multiplication.
3. **Sparse (Banded) Matrix:**
   - Use only the non-zero elements of the matrix for the simulation.


### Compilation and Execution
A `Makefile` is provided with the following targets:

- **all:** Compiles the executable.
- **clean:** Removes compiled files.

- **test_full:** Runs the simulation using the full matrix with explicit loops.
- **test_BLAS_full:** Runs the simulation using the full matrix with BLAS.
- **test_BLAS_banded:** Runs the simulation using the sparse (banded) matrix.

To compile the code, run:

```bash
make
```

To test the code with specified test cases, run:

```bash
./pde_solver -P 400 -L 5 -A 0.2 -N 100 -T 10 -dt 0.001 -sol full
```