/// @file pkkfisher3d.cpp
///
/// MPI simulation of the three-dimension KPP-Fisher's
///
/// Starter code for assignment 10 of PHY1610 (Winter 2025)
///
#include <rarray>                       // rarray header to use the rtensor class
#include <iostream>                      // Standard I/O header
#include <mpi.h>                        // MPI header to distribute the work amonst the processes
#include "params.h"                     // Parameters header to define the parameters of the simulation
#include "output.h"                     // Output header to define the output function
#include "readcommandline.h"            // Command line header to read the command line arguments
#include "ticktock.h"                   // Timer header to measure the time of the simulation

///
/// @brief Solution of the PDE using sparseness of the matrix in the
/// matrix-vector multiplication, by just writing it out (no blas, no
/// matrix multiplication).
///
/// @param p the parameters; see @ref params.h (Param)
/// 
void simulate(const Param& p, MPI_Comm comm)
{
    // where are we in the communicator
    int rank, size, left, right, guardleft, guardright;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    left  = rank - 1;
    if (left < 0)
        left = MPI_PROC_NULL;
    right = rank + 1;
    if (right >= size)
        right = MPI_PROC_NULL;
    // Derived parameters
    int    nsteps = p.T / p.D;
    double deltax = p.L/(p.N - 1);
    double alpha  = p.D / (deltax*deltax);
    if (rank==0) std::cerr  << "#alpha " << alpha << "\n";
    // Create distributed arrays
    int nguard = 2;
    int Ni = ((rank+1)*(p.N-nguard))/size - (rank*(p.N-nguard))/size + nguard; // divide system in i, minding guard cells
    int Nj = p.N;
    int Nk = p.N;
    if (Ni < 3) {
        std::cerr << "Too many processes for the size of the system\n";
        MPI_Abort(comm,1);        
    } 
    guardleft = 0;
    guardright = Ni - 1;
    rtensor<double> u(Ni, Nj, Nk); 
    rtensor<double> uold(Ni, Nj, Nk);
    // Initial state
    u.fill(0.0);
    // Boundary conditions (these points won't change)
    for (int j = 0; j < Nj; j++) {
        for (int k = 0; k < Nk; k++) {
            if (left == MPI_PROC_NULL)
                u[0][j][k] = p.A;
            if (right == MPI_PROC_NULL)
                u[Ni-1][j][k] = p.A;
        }
    }
    // Initialize the guard cells in the j-dimension
    for (int i = 0; i < Ni; i++)
        for (int k = 0; k < Nk; k++)
            u[i][0][k] = u[i][Nj-1][k] = p.A;
    // Initialize the guard cells on the k-dimension
    for (int i = 0; i < Ni; i++)
        for (int j = 0; j < Nj; j++)
            u[i][j][0] = u[i][j][Nk-1] = p.A;
    // Time stepping starts
    for (int s = 0; s <= nsteps; s++) {
        // output every so often
        if (s%(nsteps/p.P) == 0)        //output every p.P steps
            output(p.F, s*p.D, deltax, u, comm);
        // guard cell exchange with neighbours
        MPI_Sendrecv(&u[1][0][0],          Nj*Nk, MPI_DOUBLE, left, 11,
                     &u[guardright][0][0], Nj*Nk, MPI_DOUBLE, right,11,
                     comm, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&u[Ni-2][0][0],       Nj*Nk, MPI_DOUBLE, right,11,
                     &u[guardleft][0][0],  Nj*Nk, MPI_DOUBLE, left, 11,
                     comm, MPI_STATUS_IGNORE);
        // evolve: first diffuse, then react
        std::swap(u, uold);                         // update solution with Euler explicit step
        for (int i = 1; i < Ni-1; i++)
            for (int j = 1; j < Nj-1; j++)
                for (int k = 1; k < Nk-1; k++)
                    u[i][j][k] = uold[i][j][k]+alpha*(uold[i-1][j][k]+uold[i+1][j][k]
                                                      +uold[i][j-1][k]+uold[i][j+1][k]
                                                      +uold[i][j][k-1]+uold[i][j][k+1]
                                                      -6*uold[i][j][k] );
        
        for (int i = 1; i < Ni-1; i++)
            for (int j = 1; j < Nj-1; j++)
                for (int k = 1; k < Nk-1; k++)
                    u[i][j][k] += p.D * uold[i][j][k] * (1-uold[i][j][k]);
    }
}

/// 
/// @brief main function of the program to execute the simulation
///
int main(int argc, char* argv[])
{
    // Initialize MPI
    int root = 0;
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Synchronize and start the timer.
    MPI_Barrier(MPI_COMM_WORLD);
    TickTock stopwatch;
    if (rank == 0) {
        stopwatch.tick();
        std::cout << "\n" << std::string(30, '=') 
                  << "Beginning Calculation" << std::string(30, '=') << "\n\n";
    }   

    // Define defaults settings
    Param p = defaultParam;
    // Parse the command line
    int status;
    if (rank == root) {
        status = read_command_line(argc, argv, p);
        if (status==0) {
            std::cout << "#P " << p.P << "\n#L " << p.L << "\n"
                      << "#A " << p.A << "\n#N " << p.N << "\n"
                      << "#T " << p.T << "\n#D " << p.D << "\n"
                      << "#F " << p.F << "\n";
        }
    }
    MPI_Bcast(&status, 1, MPI_INT, root, MPI_COMM_WORLD);
    if (status > 1)
        MPI_Abort(MPI_COMM_WORLD, status - 1);
    // status == 1 means help was printed and that's all we want.
    if (status != 1) {
        // pass the parameters to everyone; assumes p is a "plain-old
        // datatype", ie., a struct without objects.
        MPI_Bcast(&p, sizeof(p), MPI_BYTE, root, MPI_COMM_WORLD);
        simulate(p, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    if (rank == 0) {
        stopwatch.tock("\nTotal time:     ");
        std::cout << "\n" << std::string(30, '=') 
                  << "Calculation Complete" << std::string(30, '=') << "\n\n";
    }    
    return 0;
}

