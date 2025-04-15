/// @file Compute_PDE.cpp
/// @brief Main file for solving the kpp-fisher equation with Fourier spectral method
/// @author Patrick Deng
/// @date March 11, 2025
/// This program solves the kpp-fisher equation using the Fourier Transform
/// The solution will iterate for the reaction term f(x,t) = u*(1 - u) exactly and the diffusion term through Fourier Transform
/// Subsequently, through the theory of superposition, the solution will be the sum of the two components

#include <init.h>
#include <initial_boundary_condition.h>
#include <calc_reaction.h>
#include <calc_diffusion.h>
#include <rarray>
#include <cmath>
#include <fftw3.h>

/// @param -P Number of snapshots (save interval = T / P)
/// @param -L Size of the domain [0, L]
/// @param -A Amplitude of the initial condition
/// @param -N Number of grid points
/// @param -T Total simulation time
/// @param -dt Time step size

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

    // Initialize spatial variables
    rvector<double> u(N);         // Solution vector u
    u.fill(0.0);                   // Initialize solution vector
    rvector<complex> u_hat(N);     // Fourier Transform of u
    u_hat.fill(0.0);               // Initialize Fourier Transform vector

    // Initialize temporal variables
    double time = 0.0;
    int num_tsteps = T / dt, snapshot_interval = num_tsteps / P;
    if (snapshot_interval == 0) snapshot_interval = 1; // Avoid division by zero

    // The filename to write the snapshots
    std::ostringstream filename_stream;
    filename_stream << "output_" << P << "_" << L << "_" << A << "_" << N << "_" << T << "_" << dt << ".txt";
    std::string filename = filename_stream.str();

    // Defining the fftw plans for the Fourier Transform and inverse Fourier Transform, defined here for efficiency
    // The fftw_plan object is used to store the plan for the Fourier Transform from real to complex
    fftw_plan p = fftw_plan_dft_r2c_1d(N,
        reinterpret_cast<double*>(u.data()),
        reinterpret_cast<fftw_complex*>(u_hat.data()),
        FFTW_ESTIMATE);

    fftw_plan p_inv = fftw_plan_dft_c2r_1d(N,
        reinterpret_cast<fftw_complex*>(u_hat.data()),
        reinterpret_cast<double*>(u.data()),
        FFTW_ESTIMATE);


    // Begin computation
    std::cout<<"\n" << std::string(30, '=')<< "Compute Start" << std::string(30, '=')<<"\n\n";

    // Apply the initial condition
    initial_condition(u, A, L);
    // Apply time-stepping loop
    for (int n = 0; n < num_tsteps; n++) {
        // Apply boundary condition
        boundary_condition(u);                          
        // Apply the time stepping with the reaction term exactly
        calc_reaction(dt,u);
        // Apply the time stepping with the diffusion term through Fourier Transform
        calc_diffusion(dt, L, u, u_hat, p, p_inv);
        time += dt;                                      // Increment time

        // Print snapshots at regular intervals
        if (n % snapshot_interval == 0 || n == num_tsteps - 1) {
            std::cout << "Snapshot at time t = " << time << std::endl;
            for (int i = 0; i < N; ++i) {
                double x_position = L * i / (N-1);
                std::cout << "  x = " << x_position << ", u(x,t) = " << u[i] << std::endl;
            }
            std::cout << std::endl;
            // Write entire array u to file, not just a single value
            write_snapshot_to_file(filename, n, time, L, N, u, snapshot_interval, num_tsteps);
        }
    }
    fftw_destroy_plan(p);  // Free the plan for the Fourier Transform
    fftw_destroy_plan(p_inv);  // Free the plan for the inverse Fourier Transform

    std::cout <<"\n"<< std::string(30, '=')<< "Compute Complete" << std::string(30, '=')<<"\n";
    // Output final solution
    std::cout << "\nFinal solution u(x,T):\n";
    for (int i = 0; i < N; i++) {
        std::cout << u[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}