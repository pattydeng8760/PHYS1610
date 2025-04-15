# PHYS1610H – High-Performance Scientific Computing  
**Winter 2025 – University of Toronto**

Assignemnts for PHYS1610 - High performance compute for Physists at the university of toronto
This course includes best practices for developing software as it applies to scientific computations, common numerical techniques and packages, and aspects of high performance computing. 
Topics include modelling, floating point computations, molecular dynamics, fast Fourier transforms, solving partial differential equations, parallel programming concepts, and hybrid programming.  
Students are required to have some programming experience in a compiled language (Fortran, C, C++).  The course uses C++ as the programming language.



## Overview

This repository contains assignments, simulations, and resources developed for **PHYS1610H: High-Performance Scientific Computing**, a graduate-level course focused on numerical modeling and scientific software development using C++, OpenMP, and MPI.

Through a series of computational physics problems, students explore performance-critical code design, parallel programming paradigms, and best practices in scientific reproducibility. This course emphasizes algorithm development, parallel scalability, and efficient memory and thread management for large-scale simulations.

---

## Contents

### 🧮 Assignments
| Assignment | Topic | Features |
|-----------|-------|----------|
| A1 | Numerical Integration | Simpson’s Rule, Trapezoidal Rule |
| A2 | Diffusion Equation | Explicit and Implicit Solvers |
| A3 | Wave Equation | Stability and Courant Analysis |
| A4 | Spectral Methods | FFT-based Poisson Solver |
| A5 | Parallel Histogram | MPI, OpenMP, I/O Performance |
| A6 | MPI Stencil | Domain Decomposition, Halo Exchange |
| A7 | OpenMP Mandelbrot | Loop Collapse, Static/Dynamic Scheduling |
| A8 | Monte Carlo Simulation | Random Walkers, Thread Safety |
| A9 | Parallel Reduction | MPI_Reduce vs. MPI_Allreduce |
| A10 | 3D KPP Equation | Hybrid OpenMP + MPI, Performance Benchmarking |

---

## Build Instructions

Load required modules (example for Niagara or teaching cluster):

```bash
module load gcc/12.3 rarray/2.8.0 boost/1.85.0 openmpi/4.1.5
```

To clean and build:

```bash
make clean  
make all
```
You may also build specific targets using:

```bash
make pkkfisher3d  
make pkkfisher3d_hybrid
```

based on the assignment 

---

## Testing & Validation

Each assignment folder includes:
- .cpp files
- header files for moduarization
- Makefile for compilation instructions
- README for instructions

---

## Tips

- Use `-fopenmp` for OpenMP and `mpic++` for MPI compilation.
- Always test serial performance before parallelizing.
- Commit often and document your assumptions.
- Validate against analytical or known numerical results when possible.

---

## 👨‍💻 Author

**Patrick Deng**  
PhD Student, Institute for Aerospace Studies
University of Toronto  
