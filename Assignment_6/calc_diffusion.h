/// @file calc_diffusion.h
/// @brief Header file for calculating the diffusion term du/dt = d^2u/dx^2 for the Fisher-KPP equation
/// @author Patrick Deng
/// @date March 11, 2025

#include <cmath>
#include <rarray>
#include <complex>                           /// Required for complex numbers with the FFT
#include <fftw3.h>

using complex = std::complex<double>;       /// Define complex numbers with double precision as the default

#ifndef CALC_DIFFUSION_H
#define CALC_DIFFUSION_H

/// @brief Computes the timestep for the diffusion term in the kpp-fisher equation through Fourier Transform and inverse Fourier Transform.
/// @param dt The time step size.
/// @param L Size of the domain [0, L].
/// @param u The solution vector in real space.
/// @param u_hat The solution vector in Fourier space.
/// @param p The fftw_plan object for the Fourier Transform from real to complex.
/// @param p_inv The fftw_plan object for the inverse Fourier Transform from complex to real.
void calc_diffusion(const double dt, const double L, rvector<double>& u, rvector<complex>& u_hat, fftw_plan p, fftw_plan p_inv);

#endif  // CALC_DIFFUSION_H


