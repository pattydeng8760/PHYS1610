/// @file Compute_PDE.cpp
/// @brief Main file for solving the 1D heat equation using explicit Euler method.
/// @author Patrick Deng
/// @date March 3, 2025
/// This program solves the 1D heat equation using the explicit Euler method with different solution types.
/// The solution types include full matrix, full matrix with BLAS, and banded matrix with BLAS.

#include <init.h>
#include <boundary_condition.h>
#include <diffusion_matrix.h>
#include <matrix_calc.h>
#include <cblas.h>

/// @param -P Number of snapshots (save interval = T / P)
/// @param -L Size of the domain [0, L]
/// @param -A Amplitude of the initial condition
/// @param -N Number of grid points
/// @param -T Total simulation time
/// @param -dt Time step size
/// @param -sol Type of solution method: full matrix, full matrix with BLAS, or banded matrix with BLAS, default is full matrix with BLAS

int main(int argc, char* argv[]) {
    SimulationParams params;  // Declare the struct to store simulation parameters
    try {
        params = parse_arguments(argc, argv);  // Assign parsed parameters to struct from command-line arguments
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    // Assign parsed parameters to local variables
    double P = params.P, L = params.L, A = params.A, T = params.T, dt = params.dt;
    int N = params.N;
    double dx = L / (N - 1);

    // Check the stability of the explicit Euler method
    if (check_stability(dt, dx)) {
        return EXIT_FAILURE;        // Exit program with failure status if return 0
    }
    // Initialize spatial variables
    rvector<double> u(N);         // Solution vector u
    rvector<double> f(N);         // Source term vector f
    int kl = 1, ku = 1;           // Number of sub/super diagonals for a tridiagonal matrix
    int ldab = kl + ku + 1;         // Leading dimension for banded storage
    u.fill(0.0);                   // Initialize solution vector
    f.fill(0.0);                    // Initialize source term vector

    // Initialize temporal variables
    double time = 0.0;
    int num_tsteps = T / dt, snapshot_interval = num_tsteps / P;
    if (snapshot_interval == 0) snapshot_interval = 1; // Avoid division by zero

    // Compute the diffusion matrix G based on the solution type 
    rmatrix<double> G;           // For full matrix methods
    rmatrix<double> G_banded;    // For banded BLAS method

    if (params.solution_type == SolutionType::FULL_MATRIX ||
        params.solution_type == SolutionType::FULL_MATRIX_BLAS) {
        G = rmatrix<double>(N, N);      // Allocate full matrix
        G.fill(0.0);
        diffusion_matrix_full(N, dx, G);
    } else if (params.solution_type == SolutionType::BANDED_BLAS) {
        G_banded = rmatrix<double>(N, ldab);  // Allocate banded storage
        G_banded.fill(0.0);                   // Initialize with zeros               
        diffusion_matrix_banded(N, dx, G_banded);
    } 

    // The filename to write the snapshots
    std::ostringstream filename_stream;
    filename_stream << "output_" << P << "_" << L << "_" << A << "_" << N << "_" << T << "_" << dt 
                    << "_" << solution_type_to_string(params.solution_type) << ".txt";
    std::string filename = filename_stream.str();

    // Begin computation
    std::cout<<"\n" << std::string(30, '=')<< "Compute Start" << std::string(30, '=')<<"\n\n";
    // Apply time-stepping loop
    for (int n = 0; n < num_tsteps; n++) {
        boundary_condition(u, A, time);                  // Apply boundary conditions
        source_term(f, u);                               // Compute source term
        //explicit_matvec_mult(G, u, f, dt);             // Compute the temporal integration u_{n+1} = u_n + dt * (G * u_n + f), explicit
        //blas_matvec_mult(G, u, f, dt);                 // Compute the temporal integration u_{n+1} = u_n + dt * (G * u_n + f), BLAS
        //blas_matvec_banded(G_banded, u, f, dt);          // Compute the temporal integration u_{n+1} = u_n + dt * (G * u_n + f), BLAS banded
        // Choose the solver based on the solution type
        switch (params.solution_type) {
            case SolutionType::FULL_MATRIX:
                explicit_matvec_mult(G, u, f, dt);
                break;
            case SolutionType::FULL_MATRIX_BLAS:
                blas_matvec_mult(G, u, f, dt);
                break;
            case SolutionType::BANDED_BLAS:
                blas_matvec_banded(G_banded, u, f, dt);
                break;
        }
        time += dt;                                      // Increment time

        // Print snapshots at regular intervals
        if (n % snapshot_interval == 0 || n == num_tsteps - 1) {
            std::cout << "Snapshot at time t = " << time << std::endl;
            for (int i = 0; i < N; ++i) {
                double x_position = L * i / N;
                std::cout << "  x = " << x_position << ", u(x,t) = " << u[i] << std::endl;
            }
            std::cout << std::endl;
            // Write entire array u to file, not just a single value
            write_snapshot_to_file(filename, n, time, L, N, u, snapshot_interval, num_tsteps);
        }
    }
    std::cout <<"\n"<< std::string(30, '=')<< "Compute Complete" << std::string(30, '=')<<"\n";
    // Output final solution
    std::cout << "\nFinal solution u(x,T):\n";
    for (int i = 0; i < N; i++) {
        std::cout << u[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}