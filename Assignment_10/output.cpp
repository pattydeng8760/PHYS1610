/// @file output.cpp
///
/// Part of assignment 10 of the PHY1610 Winter 2025 course.
///
/// See @ref output.h
///
#include "output.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

/// @brief convert a value to ascii with given width and precision. Will prepended with '0' for padding.
std::string double_to_string(double value, int width, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision);
    oss << std::setw(width) << std::setfill('0') << value;
    return oss.str();
}

/// 
void output(std::string fn, double t, double dx, const rtensor<double>&a, MPI_Comm comm)
{
    // where are we in the communicator?
    int rank;
    int size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    int isize = a.extent(0)-2;
    int ioffset = 0;
    MPI_Exscan(&isize, &ioffset, 1, MPI_INT, MPI_SUM, comm);
    // print a log message
    if (rank == 0) {
        std::cout << "Computation is at time " << t << '\n';
    }
    // construct file content
    int colwidth = 16;
    int numwidth = colwidth-1;
    int precision = 11;
    MPI_Offset numchars = ((a.extent(1)-2)*(a.extent(2)-2)*5*colwidth+1)*(a.extent(0)-2);
    MPI_Offset pos = 0;
    std::string asciistr(numchars,' ');
    for (int i = 1; i < a.extent(0)-1; i++) {
        for (int j = 1; j < a.extent(1)-1; j++) {
            for (int k = 1; k < a.extent(2)-1; k++) {
                double x = (ioffset+i)*dx;        
                double y = j*dx;
                double z = k*dx;
                asciistr.replace(pos, numwidth, double_to_string(t,numwidth,precision));
                pos += colwidth;
                asciistr.replace(pos, numwidth, double_to_string(x,numwidth,precision));
                pos += colwidth;
                asciistr.replace(pos, numwidth, double_to_string(y,numwidth,precision));
                pos += colwidth;
                asciistr.replace(pos, numwidth, double_to_string(z,numwidth,precision));
                pos += colwidth;
                asciistr.replace(pos, colwidth, double_to_string(a[i][j][k],numwidth,precision)+"\n");
                pos += colwidth;
            }
        }
        asciistr.replace(pos, 1, "\n");
        pos++;
    }
    // write with Collective MPI-IO
    MPI_Offset offset = 0;
    MPI_Exscan(&numchars, &offset, 1, MPI_INT, MPI_SUM, comm);
    MPI_File file;
    if (t == 0.0)  {
        if (rank == 0)
            if (std::filesystem::exists(fn))
                std::filesystem::remove(fn);
        MPI_Barrier(comm);
        MPI_File_open(comm, fn.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    } else {
        MPI_File_open(comm, fn.c_str(), MPI_MODE_APPEND | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
        MPI_Offset filesize;
        MPI_File_get_size(file, &filesize);
        offset += filesize;
    }
    MPI_File_write_at_all(file, offset, &asciistr[0], numchars, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&file);
}
