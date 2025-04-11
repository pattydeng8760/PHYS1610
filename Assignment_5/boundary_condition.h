/// @file boundary_condition.h
/// @brief Header file for boundary conditions and source term.
/// @author Patrick Deng
/// @date March 3, 2025
/// This module applies boundary conditions and constructs the source term for the Fisher-KPP equation.
#ifndef BOUNDARY_CONDITION_H
#define BOUNDARY_CONDITION_H

#include <cmath>
#include <rarray>

/// @brief Applies the boundary conditions to the solution vector u.
/// @param u Solution vector.
/// @param A Amplitude of the initial condition.
/// @param time Current time step time. 
void boundary_condition(rvector<double>& u, const double A, const double time);


/// @brief Constructs the source term f(x,t) = u(x,t)*(1 - u(x,t)) for the Fisher-KPP reaction term.
/// @param f Source term vector.
/// @param u Solution vector.
void source_term(rvector<double>& f, const rvector<double>& u);

#endif  // BOUNDARY_CONDITION_H