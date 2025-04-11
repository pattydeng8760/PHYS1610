///@file mpi_parallel_io.cpp
///@author Patrick Deng
///@date April 3, 2025
///@brief This program reads a file containing integer records, computes the logarithm of each record, and generates a histogram of the log values using MPI for parallel processing.
/// The program is designed to be run in a distributed environment using MPI with with parallel io where Each process opens the file collectively and reads its own designated portion concurrently using MPI I/O .

#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <rarray> 
#include "init.h"
#include "ticktock.h"


int main(int argc, char* argv[]){
    //--------
    // Initialize MPI environment
    //--------
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //--------
    // Parse command line arguments
    //--------
    SimulationParams params;
    if (rank == 0) {
        std::cout << "\n" << std::string(30, '=') << "Beginning Parallel I/O Calculation" 
                  << std::string(30, '=') << "\n\n";
        try {
            params = parse_arguments(argc, argv);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        std::cout << "Total number of processes: " << size << std::endl;
    }
    // Broadcast parameters to all processes.
    MPI_Bcast(&params.base, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // The batch size may not be needed here but we broadcast it for consistency.
    MPI_Bcast(&params.batch_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast filename: send its length then the string.
    int filename_length = 0;
    if (rank == 0) {
        filename_length = params.file.size();
    }
    MPI_Bcast(&filename_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char filename_buf[256];
    if (rank == 0) {
        std::copy(params.file.begin(), params.file.end(), filename_buf);
        filename_buf[filename_length] = '\0';
    }
    MPI_Bcast(filename_buf, 256, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        params.file = std::string(filename_buf);
    }

    // Synchronize and start the timer.
    MPI_Barrier(MPI_COMM_WORLD);
    TickTock stopwatch;
    if (rank == 0){
        stopwatch.tick();
    }

    //--------
    // Open the data file collectively using MPI I/O for each process
    //--------
    MPI_File fh;            // file handle for mpi in parallel I/O
    int ret = MPI_File_open(MPI_COMM_WORLD, params.file.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);      // open the file
    if (ret != MPI_SUCCESS) {       // error handling to check if the file is correctly opened  
        if (rank == 0)
            std::cerr << "Error opening file " << params.file << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    // Determine file size using MPI_File_get_size
    MPI_Offset file_size;
    MPI_File_get_size(fh, &file_size);
    // Each record is 9 bytes long (8 characters + newline)
    int total_records = file_size / 9;
    // Broadcast total_records to ensure all processes have the same value
    MPI_Bcast(&total_records, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //--------
    // Compute the number of records for each process
    //--------
    int records_per_proc = total_records / size;            // Get the number of records for each process
    int remainder = total_records % size;                   // Remainder records
    int my_records = records_per_proc + (rank < remainder ? 1 : 0);     // The record that is assigned to this process
    // Compute the starting record offset for this process.
    int offset_records = (rank < remainder)             // if rank is less than remainder
                           ? rank * (records_per_proc + 1)      // each process gets one more record
                           : remainder * (records_per_proc + 1) + (rank - remainder) * records_per_proc;        // otherwise the remaining records are distributed evenly
    MPI_Offset offset_bytes = offset_records * 9; // each record is 9 bytes! 
    // Allocate a buffer to read this process's portion of the file
    rvector<char> buffer(my_records * 9);
    MPI_File_read_at(fh, offset_bytes, buffer.data(), my_records * 9, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    //--------
    // Convert records to integers stoi(). Each record is 8 characters (ignore the newline).
    //--------
    rvector<int> local_data(my_records);
    for (int i = 0; i < my_records; i++){
        std::string record(buffer.data() + i * 9, 8);
        try {
            local_data[i] = std::stoi(record);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error: Cannot convert record '" << record 
                      << "' at index " << i << " to integer." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    //--------
    // Each process computes the logarithm
    //--------
    rvector<double> local_log(local_data.size());
    for (size_t i = 0; i < local_data.size(); i++){
        local_log[i] = std::log(local_data[i]) / std::log(params.base);
    }

    //--------
    // Compute local minimum and maximum of the log values same as the scatter workflow
    //--------
    double local_min = (local_log.size() > 0) ? local_log[0] : 0;
    double local_max = (local_log.size() > 0) ? local_log[0] : 0;
    for (size_t i = 0; i < local_log.size(); i++){
        if (local_log[i] < local_min) local_min = local_log[i];
        if (local_log[i] > local_max) local_max = local_log[i];
    }
    // Global reduction to determine the overall min and max.
    double global_min, global_max;
    MPI_Allreduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);         // Find the global minimum and maximum, broadcast to all processes
    MPI_Allreduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    
    //--------
    // Compute the histogram
    //--------
    int start_order = static_cast<int>(std::floor(global_min));
    int end_order = static_cast<int>(std::floor(global_max));
    int num_orders = end_order - start_order + 1;
    int num_bins = num_orders;  // one bin per order
    // Each process computes a local histogram.
    rvector<double> local_hist(num_bins);       // Initialize local histogram
    local_hist.fill(0);                         // Fill with zeros
    for (size_t i = 0; i < local_log.size(); i++){      // Loop through local log values
        double pos = local_log[i] - start_order;        // Calculate position in histogram
        int bin = static_cast<int>(std::floor(pos));    // Determine bin index
        if (bin >= 0 && bin < num_bins)                 // Check if bin index is valid
            local_hist[bin] += 1;                       // Increment histogram bin
    }
    
    //--------
    // Reduce all local histograms to a global histogram to the root process
    //--------
    rvector<double> global_hist(num_bins);
    global_hist.fill(0);
    MPI_Reduce(local_hist.data(), global_hist.data(), num_bins, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Root process normalizes and prints the histogram and timing
    if (rank == 0) {
        double total_count = 0;
        for (int i = 0; i < num_bins; i++){
            total_count += global_hist[i];
        }
        std::cout << "\nHistogram (log bin start, fraction):\n";
        for (int i = 0; i < num_bins; i++){
            double bin_log_start = start_order + i;
            double fraction = global_hist[i] / total_count;
            std::cout << bin_log_start << "    " << fraction << "\n";
        }
        stopwatch.tock("\nTotal time:     ");
        std::cout << "\n" << std::string(30, '=') 
                  << "Calculation Complete" << std::string(30, '=') << "\n\n";
    }
    
    MPI_Finalize();
    return 0;
}