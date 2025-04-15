/// @file calc_reaction.h
/// @author Patrick Deng
/// @date March 11, 2025
/// This file contains the function for calculating the reaction term f(x,t) = u*(1 - u) for the Fisher-KPP equation exactly
#include <cmath>
#include <rarray>

#ifndef CALC_REACTION_H
#define CALC_REACTION_H

/// @brief Computes the timestep for the reaction term in the kpp-fisher equation exactly.
/// @param dt The time step size.
/// @param u The solution vector.
void calc_reaction(const double dt, rvector<double>& u);

#endif  // CALC_REACTION_H