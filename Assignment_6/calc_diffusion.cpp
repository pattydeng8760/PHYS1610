/// @file calc_reaction.cpp
/// @brief File for calculating the diffusion term du/dt = d^2u/dx^2 for the Fisher-KPP equation
/// @author Patrick Deng
/// @date March 11, 2025
/// This file contains the function for calculating the diffusion term thorough Fourier Transform

#include "calc_diffusion.h"
#include <cmath>
#include <rarray>
#include <complex>                           /// Required for complex numbers with the FFT
#include <fftw3.h>
using complex = std::complex<double>;       /// Define complex numbers with double precision as the default

/// @brief computes the timestep for the diffusion term in the kpp-fisher equation through Fourier Transform and inverse Fourier Transform.
/// @param dt The time step size.
/// @param L Size of the domain [0, L].
/// @param u The solution vector in real space.
/// @param u_hat The solution vector in Fourier space.
/// @param p The fftw_plan object for the Fourier Transform from real to complex.
/// @param p_inv The fftw_plan object for the inverse Fourier Transform from complex to real.

void calc_diffusion(const double dt, const double L, rvector<double>& u, rvector<complex>& u_hat,fftw_plan p, fftw_plan p_inv) {
    // Define a complex vector u_hat to store the Fourier Transform of u
    int n = u.size();
    // Perform the Fourier Transform and store the result in u_hat
    fftw_execute(p);
    // Looping over the spatial frequencies to apply the diffusion term 
    for (int i = 0; i < n; i++) {
        // Calculate the spatial frequency q as 
        double q = (i <= n/2) ? i : i - n; 
        // Calculate the wavenumber k, M_PI is the constant pi, k = 2*pi*q/L
        double k = 2.0 * M_PI * q / L;
        // Apply the diffusion term in Fourier space
        u_hat[i] *= std::exp(-dt * k * k);
    }
    // Create a plan for the inverse FFT from complex to real
    fftw_execute(p_inv);

    // Normalize FFTW inverse transform by dividing by the number of grid points
    for (int i = 0; i < n; i++) {
        u[i] /= n;
        }
}