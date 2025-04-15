/// @file initial_boundary_condition.h
/// @brief Header file for boundary conditions and source term.
/// @author Patrick Deng
/// @date March 11, 2025
/// This module applies boundary conditions and constructs the source term for the Fisher-KPP equation.
#ifndef INITIAL_BOUNDARY_CONDITION_H
#define INITIAL_BOUNDARY_CONDITION_H

#include <cmath>
#include <rarray>

/// @brief Applies the periodic boundary condition to the solution vector u.
/// @param u Solution vector.
void boundary_condition(rvector<double>& u);


/// @brief Applies the initial condition to the solution vector u.
/// @param u The initialized solution vector.
/// @param A Amplitude of the initial condition sinusoid term. 
/// @param L Length of the domain.
void initial_condition(rvector<double>& u, const double A, const double L);

#endif  // BOUNDARY_CONDITION_H