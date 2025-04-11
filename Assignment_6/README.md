# Assignment 6: Simulation of KPP-Fisher's Equation in Fourier Space

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
  - $$u(0, t) = u(L, t) = 0 $$
- **Initial Condition:**
  - $$u(x, 0) =  A \sin^2(\Pi x /L )^2 $$ 

### Numerical Method
The simulation uses an **Spectral Approach**:
- The homogenous diffusion equation is solved with Fourier Transform in space through the fftw3 library
- The source term is solved exactly 
- Through superposition, the two solutions are appended for each timestep

### Input Parameters
The following parameters are input to the code:
- **P**: Number of snapshots.
- **L**: Length of the spatial domain.
- **A**: Amplitude at $$x = 0 $$ for the boundary condition.
- **N**: Number of grid points.
- **T**: Total simulation time.
- **\(\Delta t$$**: Time step size.

For the main test case, use:
- $$L = 5 $$
- $$A = 1 $$
- $$N = 100 $$
- $$T = 10 $$
- $$P = 400 $$
- $$dt = 0.01 $$

### Compilation and Execution
A `Makefile` is provided with the following targets:

- **all:** Compiles the executable.
- **clean:** Removes compiled files.

- **test:** Creates the testing conditions specified above as the requirement for the assignment

To compile the code, run:
```bash
make all
make test
```

To test the code with specified test cases, run:

```bash
./pde_solver -P 400 -L 5 -A 0.2 -N 100 -T 10 -dt 0.001 -sol full
```