#include "matrix_calc.h"
#include <cmath>
#include <rarray>
#include <cblas.h>
#include <iostream>


// Function for matrix-vector multiplication: v = G * u, explicit over loop
void explicit_matvec_mult(const rmatrix<double>& G, rvector<double>& u, const rvector<double>& f, const double dt) {
    int n = G.extent(0);
    // Compute Euler update directly: u^{n+1}[i] = u^{n}[i] + dt * (G * u + f)
    // Looping over rows of G
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += G[i][j] * u[j];  // Compute G * u inline
        }
        u[i] += dt * (sum + f[i]); // Update the rows of u 
    }
}

// Blas is for v=αGu+βv type of operations and
// Euler explict: u^(n+1) = u^n + dt * (G * u^n + f)
void blas_matvec_mult(const rmatrix<double>& G, rvector<double>& u, const rvector<double>& f, const double dt) {
    int n = G.extent(0);            // Get the size of the matrix
    double alpha = dt;             // Scaling factor of alpha = dt/(dx*dx), but (dx*dx) is already included in G
    int inc = 1;                  // Stride for vectors (use 1 for contiguous memory)
    rvector<double> v(n);      // Temporary vector to store G * u result
    v.fill(0.0);
    
    // Compute v = dt * G * u using BLAS
    cblas_dgemv(CblasRowMajor, CblasNoTrans, n, n, alpha, G.data(), n,  u.data(), inc,0.0, v.data(), inc);  // v = alpha * G * u + 0.0 * v
    
    // Update u with dt * f using BLAS: v = v + dt * f = dt * G * u + dt * f
    cblas_daxpy(n, dt, f.data(), inc, v.data(), inc);

    // Update u with v using BLAS: u = u + v
    cblas_daxpy(n, 1.0, v.data(), inc, u.data(), inc);
}


void blas_matvec_banded(const rmatrix<double>& G_banded, rvector<double>& u, const rvector<double>& f, const double dt) {
    int n = G_banded.extent(0);            // Get the size of the matrix
    double alpha = dt;             // Scaling factor of alpha = dt/(dx*dx), but (dx*dx) is already included in G
    int inc = 1;                  // Stride for vectors (use 1 for contiguous memory)
    rvector<double> v(n);       // Temporary vector to store G * u result
    v.fill(0.0);

    int kl = 1, ku = 1;         // Number of subdiagonals and superdiagonals for a tridiagonal matrix
    int ldab = kl + ku + 1;  // ldab should be 3 for a tridiagonal matrix

    // Compute v = dt * G * u using BLAS banded matrix-vector multiplication
    // G is stored as a banded matrix in G_banded with row-major order  
    cblas_dgbmv(CblasRowMajor, CblasNoTrans, n, n, ku, kl, alpha, 
                G_banded.data(), ldab, u.data(), inc, 0.0, v.data(), inc);

    // Update v with dt*f
    cblas_daxpy(n, dt, f.data(), inc, v.data(), inc);
    // Update u with v
    cblas_daxpy(n, 1.0, v.data(), inc, u.data(), inc);
}

