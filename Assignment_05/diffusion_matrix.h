/// @file diffusion_matrix.h
/// @brief Header file for constructing the diffusion matrix.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module computes the diffusion matrix operator G with the finite difference approximation 2nd order centered difference for diffusion.
#ifndef DIFFUSION_MATRIX_H
#define DIFFUSION_MATRIX_H

#include <rarray>
#include <cmath>
/// @brief Constructs the diffusion matrix G with the finite difference approximation 2nd order centered difference for diffusion.
/// @param n the number of grid points.
/// @param dx the grid spacing for the structured grid.
/// @param G the diffusion matrix initialized with zeros.
/// @return Computed diffusion matrix operator G
void diffusion_matrix_full(const int n,const double dx, rmatrix<double>& G);


/// @brief Constructs the diffusion matrix G with the finite difference approximation 2nd order centered difference for diffusion in banded storage.
/// @param n the number of grid points.
/// @param dx the grid spacing for the structured grid.
/// @param G_banded the diffusion matrix in banded storage initialized with zeros.
/// @return Computed diffusion matrix operator G in banded storage in row major order.
void diffusion_matrix_banded(const int n,const double dx, rmatrix<double>& G_banded);
#endif
