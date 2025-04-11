///@file mpi_hist_scatter.cpp
///@author Patrick Deng
///@date April 3, 2025
///@brief This program reads a file containing integer records, computes the logarithm of each record, and generates a histogram of the log values using MPI for parallel processing.
/// The program is designed to be run in a distributed environment using MPI with scratter processing where the root process reads the file and distributes the data to other processes.

#include <mpi.h>            /// MPI header file for mpi functions
#include <iostream>         /// Standard I/O header file
#include <fstream>          /// filestream for reading the datafile
#include <cmath>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
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
    // Synchronize all processes before starting the timer.
    MPI_Barrier(MPI_COMM_WORLD);
    TickTock stopwatch;     // Create a stopwatch object
    if (rank == 0){
        stopwatch.tick();     // Start timer on the root process
    }
    //--------
    // Parse command line arguments
    //--------
    SimulationParams params;
    if (rank == 0) {
        std::cout <<"\n"<< std::string(30, '=') << "Beginning Calculation" << std::string(30, '=')<<"\n\n";
        // Use the first process to parse command line arguments
        try {
            params = parse_arguments(argc, argv);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        std::cout << "Total number of processes: " << size << std::endl;
    }
    // Broadcast base and batch_size.
    MPI_Bcast(&params.base, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&params.batch_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast filename: first send its length, then the characters to all processes for subsequent receving
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
    // Determine total_records (only rank 0 reads the file size)
    int total_records = 0;
    const int record_length = 9; // 8 characters per record + newline.

    //--------
    // Using the root process to get the file size.
    //--------
    if (rank == 0) {
        try {
            auto file_size = std::filesystem::file_size(params.file);
            total_records = static_cast<int>(file_size / record_length);
        } catch(const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }
    MPI_Bcast(&total_records, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Preallocate the local_data array on each process knowing the final number of records
    int my_total = total_records / size + (rank < (total_records % size) ? 1 : 0);
    rvector<int> local_data(my_total);              // local data buffer
    int local_offset = 0;                           // counter for storing received records

    //--------
    // Main block for reading the file and distributing data
    //--------
    if (rank == 0) {
        // Root process reads the file and distributes the data.
        std::ifstream file(params.file, std::ios::binary);
        if (!file) {            // check if file opened successfully
            std::cerr << "Unable to open file " << params.file << std::endl;
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        int records_remaining = total_records;              // remaining records to read
        while (records_remaining > 0) {
            // Determine the number of records to read in this batch.
            int current_batch = std::min(params.batch_size, records_remaining);
            // Read current batch into a character buffer.
            rvector<char> buffer(current_batch * record_length);
            file.read(buffer.data(), current_batch * record_length);
            // Convert records to integers.
            rvector<int> batch_data(current_batch);
            for (int i = 0; i < current_batch; i++){
                std::string record(buffer.data() + i * record_length, 8);
                try {
                    batch_data[i] = std::stoi(record);
                } catch (const std::invalid_argument &e) {
                    std::cerr << "Error: Cannot convert record '" << record 
                              << "' at index " << i << " to integer." << std::endl;
                    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                }
            }
            // Compute send counts for MPI_Scatterv.
            rvector<int> send_counts(size);
            int base_count = current_batch / size;
            int remainder = current_batch % size;
            for (int i = 0; i < size; i++){
                send_counts[i] = base_count;
                if (i < remainder) send_counts[i]++;
            }
            // Compute displacements which is the offset for each process
            rvector<int> displs(size);
            displs[0] = 0;
            for (int i = 1; i < size; i++){
                displs[i] = displs[i-1] + send_counts[i-1];
            }
            // Broadcast current_batch and send_counts so non-root processes know how many to receive
            MPI_Bcast(&current_batch, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(send_counts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
            // Prepare receive buffer for this batch
            rvector<int> recv_buf(send_counts[rank]);
            MPI_Scatterv(batch_data.data(), send_counts.data(), displs.data(), MPI_INT,
                         recv_buf.data(), send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
            // Copy the received numbers into local_data
            for (int j = 0; j < send_counts[rank]; j++){
                local_data[local_offset + j] = recv_buf[j];
            }
            local_offset += send_counts[rank];          // Sending the received data to the local data buffer
            records_remaining -= current_batch;         // the number of records remaining to read 
        }
        file.close();
    // The non-root process for receiving data
    } else {
        // Non-root processes receive data.
        int records_received = 0;
        // Process only if the number of records received is less than the total number of records.
        while (records_received < total_records) {
            int current_batch;
            MPI_Bcast(&current_batch, 1, MPI_INT, 0, MPI_COMM_WORLD);               // broadcast current_batch to all processes
            rvector<int> send_counts(size); 
            MPI_Bcast(send_counts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);        // broadcast send_counts to all processes
            rvector<int> recv_buf(send_counts[rank]);           // buffer for receiving data
            // MPI_Scatterv to receive the data.
            MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT,
                         recv_buf.data(), send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
            // Copy received data into local_data for the non-root process for further processing
            for (int j = 0; j < send_counts[rank]; j++){
                local_data[local_offset + j] = recv_buf[j];
            }
            local_offset += send_counts[rank];
            records_received += current_batch;
        }
    }

    //--------
    // Each process computes the logarithm of its data.
    //--------
    rvector<double> local_log(local_data.size());
    for (size_t i = 0; i < local_data.size(); i++){
        local_log[i] = std::log(local_data[i]) / std::log(params.base);
    }
    // Compute local min and max for each process' data 
    double local_min = (local_log.size() > 0) ? local_log[0] : 0;
    double local_max = (local_log.size() > 0) ? local_log[0] : 0;
    for (size_t i = 0; i < local_log.size(); i++){
        if (local_log[i] < local_min) local_min = local_log[i];
        if (local_log[i] > local_max) local_max = local_log[i];
    }

    //--------
    // Compute global minimum and maximum
    //--------
    double global_min, global_max;
    // Use MPI_Allreduce to find the global min and max across all processes, combining the results.
    MPI_Allreduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    
    //--------
    // Compute local histogram for each process
    //--------
    // Initialize  histogram parameters
    int start_order = static_cast<int>(std::floor(global_min));
    int end_order = static_cast<int>(std::floor(global_max));
    int num_orders = end_order - start_order + 1;
    double bin_width = 1.0;
    int num_bins = num_orders;  // one bin per order
    rvector<double> local_hist(num_bins);
    local_hist.fill(0);
    // Computing the histogram
    for (size_t i = 0; i < local_log.size(); i++){
        double pos = local_log[i] - start_order;
        int bin = static_cast<int>(std::floor(pos));
        if (bin >= 0 && bin < num_bins)
            local_hist[bin] += 1;
    }
    // Sum the local histograms into a global histogram on rank 0.
    rvector<double> global_hist(num_bins);
    global_hist.fill(0);
    MPI_Reduce(local_hist.data(), global_hist.data(), num_bins, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Root normalizes and prints the histogram and the timing.
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
        // Stop the timer and print elapsed time
        stopwatch.tock("\nTotal time:     ");       // this combines the elapsed time measurement and output
        std::cout <<"\n"<< std::string(30, '=') << "Calculation Complete" << std::string(30, '=')<<"\n\n";
    }

    MPI_Finalize();
    return 0;
}
