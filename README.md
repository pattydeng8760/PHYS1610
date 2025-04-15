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

###  Assignments
| Assignment | Topic | Features |
|-----------|-------|----------|
| A1 | Introduction to C++ | C++ coding and syntax |
| A2 | Modularization | headerfile and makefile |
| A3 | Testing and Version Control | Git utilization, testing |
| A4 | Numerics and Cluster Operation | Cluster utilization and benchmarking|
| A5 | 1-D Partial-Differential Equations (FDM with kpp-fisher equation) | Tensor operations, linear algebra package utilization (BLAS)|
| A6 | 1-D Partial-Differential Equations (Spatial Fourier Transform with kpp-fisher equation) | Fast Fourier Transform package utilization (FFTW) |
| A7 | Percolation Simulation | Random Number Generation RNG Package utilization |
| A8 | Monte Carlo Simulation | OpenMP programming |
| A9 | Monte Carlo Simulation  | Message Passing Interface (MPI) Programming |
| A10 | 3-D Partial-Differential Equations (FDM with kpp-fisher equation) | Hybrid OpenMP + MPI |

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

## Author

**Patrick Deng**  
PhD Student, Institute for Aerospace Studies
University of Toronto  
