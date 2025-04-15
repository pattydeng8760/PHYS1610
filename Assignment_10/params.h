/// @file params.h
///
/// Header to define the struct to hold parameters:
/// P (number of snapshots to output), L (length of the interval), A
/// (amplitude of the boundary driving), N (number of grid points), T
/// (time to simulate), and D (time step), and F (filename)
///
/// Part of assignment 10 of the PHY1610 Winter 2025 course.
///
#ifndef PARAMSH
#define PARAMSH

///
/// @brief Parameters for the simulation
///
struct Param {
    int    P; ///< number of snapshots to output
    double L; ///< length of the interval
    double A; ///< amplitude of the boundary driving
    int    N; ///< number of grid points
    double T; ///< time to simulate
    double D; ///< time step
    char   F[256]; ///< a file name
};

/// Default values
const Param defaultParam = { 400, 5.0, 0.2, 100, 10, 0.001, "output.dat" };

#endif
