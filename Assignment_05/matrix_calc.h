/// @file matrix_calc.h
/// @brief Header file for matrix-vector multiplication.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module performs matrix-vector multiplication for the Fisher-KPP equation.
#ifndef MATRIX_CALC_H
#define MATRIX_CALC_H

#include <rarray>
#include <cmath>
#include <cblas.h>

/// @brief Function for matrix-vector multiplication: u_{n+1} = u_n + dt * (G * u_n + f) using explicit loop.
/// @param G Diffusion matrix.
/// @param u Solution vector.
/// @param f Source term vector for the Fisher-KPP equation: f = u*(1-u)
/// @param dt Time step size.
void explicit_matvec_mult(const rmatrix<double>& G, rvector<double>& u, const rvector<double>& f, const double dt);

// @brief Function for matrix-vector multiplication: u_{n+1} = u_n + dt * (G * u_n + f) using BLAS.
/// @param G Diffusion matrix.
/// @param u Solution vector.
/// @param f Source term vector for the Fisher-KPP equation: f = u*(1-u)
/// @param dt Time step size.
void blas_matvec_mult(const rmatrix<double>& G, rvector<double>& u, const rvector<double>& f, const double dt);


/// @brief Function for matrix-vector multiplication: u_{n+1} = u_n + dt * (G * u_n + f) using BLAS banded storage.
/// @param G Diffusion matrix.
/// @param u Solution vector.
/// @param f Source term vector for the Fisher-KPP equation: f = u*(1-u)
/// @param dt Time step size.
void blas_matvec_banded(const rmatrix<double>& G, rvector<double>& u, const rvector<double>& f, const double dt);

/// @brief Function to convert a full matrix to banded storage.
/// @param A Full matrix to convert to banded storage.
/// @param B Banded storage matrix.
/// @param kl Number of subdiagonals in the banded storage.
/// @param ku Number of superdiagonals in the banded storage.
void convert_to_banded(const rmatrix<double>& A, rmatrix<double>& B, const int kl, const int ku);


#endif  // MATRIX_CALC_H