/// @file output_hybrid.cpp
/// @author Patrick Deng
/// @date 2025-04-13
/// @brief Output routine to write the elements of the field at time t to a file in five-column format
/// (t, x, y, z, a[t,x,y,z]) using OpenMP and MPI parallelization.
/// Part of assignment 10 of the PHY1610 Winter 2025 course.
/// See @ref output_hybrid.h
#include "output_hybrid.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <omp.h>
#include <filesystem>


/// @brief convert a value to ascii with given width and precision. Will prepended with '0' for padding.
std::string double_to_string(double value, int width, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision);
    oss << std::setw(width) << std::setfill('0') << value;
    return oss.str();
}


void output_hybrid(std::string fn, double t, double dx, const rtensor<double>& a, MPI_Comm comm)
{
    // Determine MPI rank and size.
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // The number of "inner" i indices (excluding boundaries).
    int isize = a.extent(0) - 2;
    int ioffset = 0;
    MPI_Exscan(&isize, &ioffset, 1, MPI_INT, MPI_SUM, comm);
    
    // Log the current simulation time.
    if (rank == 0) {
        std::cout << "Computation is at time " << t << '\n';
    }
    
    // Set up formatting parameters.
    int colwidth = 16;
    int numwidth = colwidth - 1;
    int precision_val = 11;
    
    // Dimensions for the inner loops (exclude boundaries in j and k).
    int M = a.extent(1) - 2; // number of j iterations
    int N = a.extent(2) - 2; // number of k iterations
    // Calculate the number of characters that will be produced per i-slice.
    int chars_per_i = (M * N * 5 * colwidth + 1);
    MPI_Offset numchars = chars_per_i * (a.extent(0) - 2);
    
    // Allocate one string per i-slice in a vector so that each thread writes its own buffer.
    int i_min = 1;
    int i_max = a.extent(0) - 1; // i iterates from 1 to a.extent(0)-1 (i.e. total slices = a.extent(0)-2)
    int num_slices = i_max - i_min;
    std::vector<std::string> slices(num_slices, std::string(chars_per_i, ' '));
    
    // Define bounds for j and k loops.
    int j_min = 1;
    int j_max = a.extent(1) - 1;
    int k_min = 1;
    int k_max = a.extent(2) - 1;
    
    // Parallelize over the i-slices using OpenMP.
    #pragma omp parallel for schedule(static) default(none) \
        shared(a, dx, colwidth, numwidth, precision_val, slices, i_min, i_max, j_min, j_max, k_min, k_max, t, ioffset, chars_per_i)
    // Parallelizing over i-slices provides coarse-grained, cache-friendly work units per thread.
    // Avoids thread contention and allows safe, independent writes to each slice buffer.
    for (int i = i_min; i < i_max; i++) {
        // Access the buffer for this i-slice.
        std::string &slice = slices[i - i_min];
        
        // Loop over j and k within this slice.
        for (int j = j_min; j < j_max; j++) {
            // For each (i, j) row, compute the starting offset in the slice.
            int total_per_j = (k_max - k_min) * 5 * colwidth;
            int pos_ij = (j - j_min) * total_per_j;
            for (int k = k_min; k < k_max; k++) {
                // Compute the starting position for each (i, j, k) cell in the slice.
                int pos_elem = pos_ij + (k - k_min) * 5 * colwidth;
                double x = (ioffset + i) * dx;
                double y = j * dx;
                double z = k * dx;
                
                // Write the five fields in sequence: time, x, y, z, and the field value a[i][j][k].
                slice.replace(pos_elem,          numwidth, double_to_string(t, numwidth, precision_val));
                pos_elem += colwidth;
                slice.replace(pos_elem,          numwidth, double_to_string(x, numwidth, precision_val));
                pos_elem += colwidth;
                slice.replace(pos_elem,          numwidth, double_to_string(y, numwidth, precision_val));
                pos_elem += colwidth;
                slice.replace(pos_elem,          numwidth, double_to_string(z, numwidth, precision_val));
                pos_elem += colwidth;
                slice.replace(pos_elem, colwidth, double_to_string(a[i][j][k], numwidth, precision_val) + "\n");
            } // end k loop
        } // end j loop
        // At the end of the i-slice, add an extra newline.
        slice.replace(chars_per_i - 1, 1, "\n");
    } // end i loop
    
    // Combine all slices into one contiguous output string.
    std::string asciistr;
    asciistr.reserve(numchars);
    for (const auto &s : slices) {
        asciistr.append(s);
    }
    
    // Write the combined ASCII string using collective MPI I/O.
    MPI_Offset offset = 0;
    MPI_Exscan(&numchars, &offset, 1, MPI_INT, MPI_SUM, comm);
    MPI_File file;
    if (t == 0.0) {
        if (rank == 0 && std::filesystem::exists(fn))
            std::filesystem::remove(fn);
        MPI_Barrier(comm);
        MPI_File_open(comm, fn.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    } else {
        MPI_File_open(comm, fn.c_str(), MPI_MODE_APPEND | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
        MPI_Offset filesize;
        MPI_File_get_size(file, &filesize);
        offset += filesize;
    }
    MPI_File_write_at_all(file, offset, asciistr.data(), numchars, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&file);
}