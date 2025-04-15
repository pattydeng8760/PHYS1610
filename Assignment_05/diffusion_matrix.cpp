/// @file diffusion_matrix.cpp
/// @brief Source file for constructing the diffusion matrix for the finite difference approximation of the heat equation.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module constructs the diffusion matrix for the finite difference approximation of the heat equation.

#include "diffusion_matrix.h"
#include <cmath>
#include <rarray>

/// @brief Construct the diffusion matrix G for the finite difference approximation of the heat equation.
/// @param n Number of grid points.
/// @param dx Grid spacing.
/// @param G nxn rarray Matrix to store the diffusion matrix where G = 1/dx^2 * [1 -2 1] with Dirichlet boundary conditions.
void diffusion_matrix_full(const int n,const double dx, rmatrix<double>& G){
    // Constructing the banded matix G with the finite difference approximation 2nd order centered difference for diffusion
    // A = 1/dx^2 * [1 -2 1]
    for (int i = 0; i < n; i++){
        G[i][i] = -2.0/(dx*dx);                     // the diagonal elements
        if (i > 0) G[i][i-1] = 1.0/(dx*dx);         // the lower diagonal elements
        if (i < n-1) G[i][i+1] = 1.0/(dx*dx);       // the upper diagonal elements
    }
    // Applying the dirichlet boundary conditions to the matrix G
    G[0][0] = 1.0;                                  // the diagonal element at the first row for the dirichlet boundary condition
    G[0][1] = 0.0;                          // the upper diagonal element at the first row for the dirichlet boundary condition
    int last_row = G.extent(0) - 1;
    int last_col = G.extent(1) - 1;
    G[last_row][last_col] = 1.0;  // Set diagonal element at the last row
    G[last_row][last_col - 1] = 0.0;  // Set the subdiagonal element to 0
}

/// @brief Construct the tri-banded diffusion matrix G for the finite difference approximation of the heat equation.
/// @param n Number of grid points.
/// @param dx Grid spacing.
/// @param G_banded nx3 rarray Matrix to store the banded diffusion matrix where G = 1/dx^2 * [1 -2 1] with Dirichlet boundary conditions.
void diffusion_matrix_banded(const int n, const double dx, rmatrix<double>& G_banded) {
    int kl = 1, ku = 1;
    int ldab = kl + ku + 1;  // = 3
    // Allocate an n x ldab matrix (row-major layout)
    G_banded = rmatrix<double>(n, ldab);
    G_banded.fill(0.0);
    double coeff = 1.0 / (dx * dx);
    
    for (int i = 0; i < n; i++) {
        // Main diagonal if-else condition
        // For boundary rows (i == 0 or i == n - 1), enforce Dirichlet conditions by setting G_banded[i][ku] = 1.0,
        // For interior rows (1 ≤ i ≤ n-2), apply the standard second-order finite difference approximation,
        G_banded[i][ku] = (i == 0 || i == n - 1) ? 1.0 : -2.0 * coeff;
        if (i > 0) {
            // Subdiagonal A(i, i-1) at column index ku-1 (i.e., index 0)
            G_banded[i][ku + 1] = coeff;
        }
        if (i < n - 1) {
            // Superdiagonal A(i, i+1) at column index ku+1 (i.e., index 2)
            G_banded[i][ku - 1] = coeff;
        }
    }
    //G_banded[0][1] = 1.0;  // Dirichlet boundary condition for the main diagonal for the first element
    //G_banded[n - 1][1] = 1.0;  // Dirichlet boundary condition for the main diagonal for the last element
    G_banded[0][0] = 0.0;  // Dirichlet boundary condition for the superdiagonal for the first element
    G_banded[n - 1][2] = 0.0;  // Dirichlet boundary condition for the subrdiagonal for the last element

}