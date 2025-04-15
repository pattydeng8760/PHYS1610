# Assignment 10: Hybrid MPI+OpenMP Simulation of the 3D KPP-Fisher Equation

This is assignment 10 of the PHY1610H course in Winter 2025.

The MPI code given here simulates the three-dimension KPP-Fisher's equation:

Author: Patrick Deng

$$
\frac{\partial u}{\partial t} - \frac{\partial^2 u}{\partial x^2} - \frac{\partial^2 u}{\partial y^2} - \frac{\partial^2 u}{\partial z^2} = u(1 - u)
$$

which is a diffusion equation with a non-linear reaction term.

The $$x$$, $$y$$, and $$z$$ values are restricted to a cube $$[0,L] \times [0,L] \times [0,L]$$, with the condition that $$u = A$$ on all the boundaries.

The initial value is given by 

$$
u(x,y,z,0) = 0
$$

(except on the boundaries).

The solution is numerically computed with an explicit time-stepping scheme using timesteps $$\Delta t$$ up to a time $$T$$, using a discretization of the interval into $$N$$ points and forward Euler integration. The simulation employs a standard stencil method (without BLAS or FFTW) for both diffusion and reaction terms.

The main code is in `pkkfisher3d.cpp`. It is kept relatively flat for clarity. Some modularity is provided by the helper codes in `output.h`, `output.cpp`, `readcommandline.h`, and `readcommandline.cpp`, which handle the output of data and parsing of input parameters.

"make" compiles the application, producing the executable `pkkfisher3d`. On the Teach cluster, this requires the following modules to be loaded:
- gcc/12.3
- rarray/2.8.0
- boost/1.85.0
- openmpi/4.1.5

There is also a file called `setup` that you can use (e.g., `source setup`) to load these modules.

During the simulation, $$P$$ snapshots are printed to a file $$F$$ containing the values of $$u$$ on all $$N-2$$ interior points (i.e., excluding the boundaries where $$u = A$$). Each line in the output file contains five numbers separated by spaces: the time, $$x$$, $$y$$, $$z$$, and the value of $$u$$ at that time and position.

The input parameters $$P$$, $$L$$, $$A$$, $$N$$, $$T$$, $$\Delta t$$, and $$F$$ are provided as command-line arguments.

When you run `"make run"`, the application executes using the following default parameters:
- $$L = 15$$
- $$A = 0.2$$
- $$N = 100$$
- $$T = 10$$
- $$P = 10$$
- $$\Delta t = 0.001$$

It is run once in serial and once with 4 MPI processes. The results stored in `output1.dat` and `output4.dat` should be identical.

---

## Hybrid MPI+OpenMP Version

The task for this assignment was to create a hybrid MPI+OpenMP version of the code and scale up to a larger problem using:
- $$N = 200$$
- $$D = 0.00125$$
while keeping the other parameters unchanged.

Additionally, the code was modified to utilize OpenMP for parallelizing the compute-intensive inner loops (the "deeper" levels), while MPI continues to handle domain decomposition in the $$i$$-direction. Key modifications include:

- **Code Changes:**
  - In `pkkfisher3d.cpp` (or `pkkfisher3d_hybrid.cpp`), inner loops (over $$j$$ and $$k$$) have been parallelized with OpenMP using directives 
    `#pragma omp parallel for collapse(2) schedule(static)`  
    This augments the MPI parallelization.
  - The output routine in `output.cpp` was modified to create a hybrid version (`output_hybrid.cpp`). In this routine, OpenMP is used to build the output string in parallel by partitioning the work into per-$$i$$-slice buffers, which are then merged.

- **Job Scripts:**
  - Job scripts were developed for running the code on 1, 2, and 3 Teach-cluster nodes with 40, 80, and 120 MPI processes respectively.
  - Additional job scripts explore various hybrid combinations by varying `ntasks_per_node` (MPI processes) and `cpus_per_task` (OpenMP threads) to fully exploit the 40 cores per node.
  - A total of 6 cases were tested with the MPI-OMP implementation: 

- **Performance Tuning and Findings:**
  - Various scheduling policies were tested. For the diffusion and reaction loops, `schedule(static)` resulted in lower overhead compared to dynamic scheduling for a uniformly balanced workload.
  - The optimal configuration was achieved with 20 MPI processes per node and 2 OpenMP threads per process (utilizing 40 cores per node). This configuration provided the best speedup over the MPI-only version.
  - The revised output routine using per-slice buffers and OpenMP resulted in more efficient I/O generation compared to the MPI-only version.

---

## Build and Execution

### Building the Code

The provided `Makefile` builds two executables:
- `pkkfisher3d`: the MPI-only version
- `pkkfisher3d_hybrid`: the hybrid MPI+OpenMP version

To compile, run:
```bash
make 
```

### Testing the Code

To run the test case for the MPI and MPI_OMP cases run: 
```bash
make run
make run_hybrid
```

## Results

The simulation was run with the following input parameters:

| Parameter | Value   |
|-----------|---------|
| $$L$$         | 15      |
| $$A$$         | 0.2     |
| $$N$$         | 200     |
| $$T$$         | 10      |
| $$P$$         | 10      |
| $$\Delta t$$  | 0.00125 |

### MPI-only Version Timings

| Configuration             | Timing (s) |
|---------------------------|------------|
| 1 node - 40 processes     | 33.2 s     |
| 2 nodes - 80 processes    | 23.74 s    |
| 3 nodes - 120 processes   | 20.48 s    |

### Hybrid MPI+OpenMP Version Timings (3 Nodes)

| MPI Processes per Node | OpenMP Threads per Process | Total MPI Processes | Timing (s) |
|------------------------|----------------------------|---------------------|------------|
| 1                      | 40                         | 3                   | 79.89 s    |
| 2                      | 20                         | 6                   | 16.64 s    |
| 4                      | 10                         | 12                  | 15.41 s    |
| 10                     | 4                          | 30                  | 15.86 s    |
| 20                     | 2                          | 60                  | 17.19 s    |
| 40                     | 1                          | 120                 | 21.04 s    |

### Discussion

The pure MPI implementation shows a clear decrease in runtime as the total number of processes increases:
- With 40 processes on one node, the runtime is 33.2 seconds.
- With 80 processes on two nodes, the runtime decreases to 23.74 seconds.
- With 120 processes on three nodes, the runtime further decreases to 20.48 seconds.

In the hybrid MPI+OpenMP case, running on 3 nodes, the performance varies significantly depending on the balance between MPI processes and OpenMP threads:

- **1 MPI process per node (40 threads each):** The runtime was **79.89 s**, which is considerably slower. This configuration likely suffers from high thread management overhead and suboptimal parallel workload distribution.
- **2 MPI processes per node (20 threads each):** A runtime of **16.64 s** was observed, showing a significant improvement as the workload is more evenly distributed.
- **4 MPI processes per node (10 threads each):** The runtime further improves to **15.41 s**, indicating an optimal balance for this particular problem and hardware configuration.
- **10 MPI processes per node (4 threads each):** With 30 MPI processes in total, the runtime is **15.86 s**; this is comparable to the 4-per-node configuration, though slightly slower.
- **20 MPI processes per node (2 threads each):** With 60 MPI processes in total, the runtime increases modestly to **17.19 s**, perhaps due to increased communication overhead among a larger number of MPI tasks.
- **40 MPI processes per node (1 thread each):** This essentially reverts to a pure MPI setup (120 MPI processes total), giving a runtime of **21.04 s**, which is slower than the optimal hybrid configurations but similar to the MPI only configuration. Likly due to overhead associated with the program still needing to initialize the OMP process.


### Optimization Curve

Analyzing the performance of the hybrid MPI+OpenMP runs reveals a classic U-shaped optimization curve when balancing the number of MPI processes against the number of OpenMP threads per process.

- **Excessive Threading (Low MPI, High OpenMP):**  
  When using a low number of MPI processes with a high number of OpenMP threads per process (e.g., 1 MPI process per node with 40 threads), the runtime was very high (79.89 s). This suggests that the overhead associated with managing a large number of threads—such as thread creation, synchronization, and contention on shared resources—dominates the computation, resulting in poor performance.

- **Balanced Configuration (Moderate MPI and Threads):**  
  As the configuration is tuned toward a moderate number of MPI processes and a moderate number of OpenMP threads (for instance, 2 processes per node with 20 threads each or 4 processes per node with 10 threads each), the runtime drops significantly (16.64 s and 15.41 s, respectively). This optimal region is where the computation is efficiently divided between inter-node communication (handled by MPI) and intra-node parallelism (handled by OpenMP), minimizing both communication and threading overhead.

- **Excessive MPI Processes (High MPI, Low OpenMP):**  
  Finally, pushing the configuration in the other direction—using many MPI processes per node with fewer threads (40 MPI processes per node with 1 thread each, essentially pure MPI)leads to an increased runtime (21.04 s). Here, while the OpenMP overhead is minimized, the communication overhead among a large number of MPI processes starts to dominate, and the benefits of reduced thread overhead are offset by inefficient domain decomposition and increased inter-process communication.

### Interpretation of the Optimization Curve

The overall trend forms a U-shaped curve:
- **Left Side of the Curve (High Threading Overhead):** Too few MPI processes lead to excessive OpenMP threading within each MPI task. Although the computation itself is parallelized, the overhead of managing 40 threads per process results in a slowdown.
- **Middle of the Curve (Balanced Approach):** An optimal balance is achieved when the number of MPI processes and the number of OpenMP threads are well-tuned to the underlying hardware. In our experiments, configurations with 4 MPI processes per node (10 threads each) or 10 MPI processes per node (4 threads each) yielded the best performance.
- **Right Side of the Curve (High Communication Overhead):** Too many MPI processes (with correspondingly fewer threads per process) increase the inter-process communication overhead. In our case, pushing to 40 MPI processes per node (essentially pure MPI) resulted in a higher runtime than the optimal hybrid configuration.

- **Optimization Trend:**  
  The data shows that the hybrid approach reaches an optimal performance zone when the balance between MPI processes and OpenMP threads minimizes both thread management and communication overhead. For the given hardware and problem size, this optimum is around 4 or 10 MPI processes per node.

- **Trade-Off Analysis:**  
  - **Too Few MPI Processes:** High thread count per process increases overhead from thread management and memory contention, which degrades performance.
  - **Too Many MPI Processes:** Although reducing threading overhead, increasing MPI processes leads to higher inter-process communication costs, which also negatively impact performance.

- **Visualizing the Curve:**  
  If you plot the total execution time against the number of MPI processes (with the corresponding OpenMP thread count adjusted so that the total cores remain constant), you would see a U-shaped curve. The minimum point on the curve represents the optimal balance where the cost of threading overhead and the cost of MPI communication are minimized.

## Conclusion

Careful tuning of the hybrid configuration is key to maximizing performance. For this simulation:
- The optimal configurations were around 4 MPI processes per node (10 threads each) or 10 MPI processes per node (4 threads each).
- These settings provided better performance than both extreme approaches (i.e., too many threads with few MPI processes, or too many MPI processes with few threads).

This analysis underscores the importance of balancing the two levels of parallelism to achieve optimal performance in hybrid MPI+OpenMP applications.

Overall, the best performance in the hybrid mode was achieved with either 4 MPI tasks per node (10 threads per task) or 10 MPI tasks per node (4 threads per task). These configurations balance the inter-node communication overhead and the intra-node threading overhead effectively. The results highlight that careful tuning of the MPI and OpenMP parameters is essential to fully exploit the available hardware and improve performance over a pure MPI implementation.

