/// @file friction.h
/// @brief Header file for the friction module.
/// @author Patrick Deng
/// @date February 12, 2025
/// This module computes the friction rate based on velocity samples v taken a time dt apart.
#ifndef FRICTIONH
#define FRICTIONH

#include <rarray>
/// @brief Computes friction force given velocity and coefficient.
/// @param dt the timstep size (s).
/// @param v the velcoity gradient, output from numdiff
/// @return Computed friction rate
double frictionrate(double dt, const rvector<double>& v);

/// @brief Estimates the velocities using finite differences of the position sample.
/// @param dt the timstep size (s).
/// @param z the position of the marble
/// @return Computed velocity gradient
rvector<double> numdiff(double dt, const rvector<double>& z);

#endif
