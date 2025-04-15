/// @file output_hybrid.h
///
/// Write the elements of the field at time t to console in five-column format
/// (t, x, y, z, a[t,x,y,z])
///
/// Part of the assignment 10.
///
#ifndef OUTPUTH
#define OUTPUTH

#include <mpi.h>
#include <rarray>

///
/// @brief output routine to a file.  Omits the boundary and guard cells.
///
/// @param fn   the name of the file to write to.
/// @param t    time (double)
/// @param dx   grid spacing (double)
/// @param a    field at time t (rtensor<double>)
/// @param comm MPI communicator
///
void output_hybrid(std::string fn, double t, double dx, const rtensor<double>&a, MPI_Comm comm);

#endif

