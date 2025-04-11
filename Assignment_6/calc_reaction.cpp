/// @file calc_reaction.cpp
/// @brief File for calculating the reaction term f(x,t) = u*(1 - u) for the Fisher-KPP equation
/// @author Patrick Deng
/// @date March 11, 2025
/// This file contains the function for calculating the reaction term f(x,t) = u*(1 - u) for the Fisher-KPP equation

#include "calc_reaction.h"
#include <cmath>
#include <rarray>

/// @brief computes the timestep for the reaction term in the kpp-fisher equation exactly.

void calc_reaction(const double dt, rvector<double>& u) {
    for (int i = 0; i < u.size(); i++) {
        u[i] =  u[i]/ (u[i] + (1-u[i])*std::exp(-dt));
    }
}