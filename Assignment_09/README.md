# Assignment 9: Parallel Histogram Computation in MPI

This assignment extends Assignment 8 by processing a dataset 200× larger using MPI. The goal is to compute the histogram of the logarithm of the number of time steps walkers need to reach the bottom of a porous medium.

Two versions of the MPI code are provided:

1. **Scatter Version (`mpi_hist_scatter`)**
   - **Step 1:** The root process reads command-line arguments (log base, filename, and batch size Z).
   - **Step 2:** The root process reads a batch of numbers of size Z from the file and distributes (scatters) these data points to all MPI processes using `MPI_Scatterv`.
   - **Step 3:** Each process computes a local histogram of the logarithm of its data points (using the same log base).
   - **Step 4:** The local histograms are reduced to a global normalized histogram, which is printed by the root process.

2. **Parallel I/O Version (`mpi_hist_parallel_io`)**
   - **Step 1:** The root process reads the command-line arguments (log base, filename, batch size).
   - **Step 2:** All MPI processes collectively open the file using MPI I/O, compute their file offsets, and read their designated portions in parallel.
   - **Step 3:** Each process converts its portion of the data to integers, computes the logarithm (using the same log base), and builds a local histogram.
   - **Step 4:** The local histograms are reduced to form a global normalized histogram, which is printed by the root process.

Both versions use the `TickTock` class to time the execution of the main computation. Aside from differences in file I/O (centralized vs. parallel), the outputs (except timing) should be identical.

---

## Files Included

- **mpi_hist_scatter.cpp** – Implements the scatter version.
- **mpi_hist_parallel_io.cpp** – Implements the MPI I/O (parallel) version.
- **init.cpp / init.h** – Functions for parsing command-line arguments and initializing simulation parameters.
- **ticktock.cpp / ticktock.h** – Provides the `TickTock` class for timing measurements.
- **Makefile** – Builds both versions.
- **Job Scripts** – (Optional) Example job scripts for running on the Teach cluster with various process counts (e.g., 1, 8, 20, 40, 80).

---

## Compilation

Ensure that MPI, OpenMP, and the rarray library are installed on your system. To compile both versions, run:

```bash
make
```

to run compute
```bash
nproc=1
base=1.1
batch=100000
filename="morestepnumbers_1.7GB.dat"
mpirun -np $nproc ./$(SCATTER_EXE) -base $base -filename $filename -batch $batch
```

## Results
The run time for the distibuted scatter IO processing: 
np=1: 15.37s
np=8: 11.12s
np=20: 11.08s
np=40: 11.43s
np=80: 11.53s

The run time for the praallel I/O processing: 
np=1: 16.35s
np=8: 2.165s
np=20: 1.128s
np=40: 0.9738s
np=80: 0.0772s

# Conclusion
The runtime results clearly demonstrate the benefits of using parallel I/O over the distributed scatter I/O approach, particularly as the number of MPI processes increases

- **Scatter I/O Version:**  
  - With a single process, the runtime is 15.37 seconds
  - Increasing the process count to 8, 20, 40, and 80 results in runtimes of approximately 11 seconds with little improvement beyond 8 processes, reaching a performance plateau
  This plateau suggests that the root process, which is responsible forhandling and distributing data, becomes a bottleneck. The overhead of centralized I/O and communication limits the scalability of this method

- **Parallel I/O Version:**  
  - The single-process runtime is slightly higher at 16.35 seconds, but the advantage becomes clear as more processes are used
  - At 8 processes, the runtime drops dramatically to 2.165 seconds.
  - Further improvements are observed with 20 and 40 processes, achieving runtimes of 1.128 seconds and 0.9738 seconds,
  - With 80 processes, the runtime falls to 0.0772 seconds.  21.17x faster than the single process
  This near-linear scaling demonstrates that allowing each process to read its own portion of the file in parallel effectively eliminates the centralized I/O bottleneck.

- Overall, the parallel I/O approach offers substantial performance benefits for large datasets by distributing the file reading workload across all processes, resulting in significantly lower execution times as the number of processes increases.**





