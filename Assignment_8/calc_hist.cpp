/// @file calc_hist.cpp
/// @author Patrick Deng
/// @date 03/28/2025
/// @brief This program calculates the distribution of the number of steps taken by a walker in a 1D random walk simulation in log space.
/// The program also parallelizes the calculation of the histogram of the logarithm of the step count using OpenMP.

#include <iostream>
#include <fstream>
#include <rarray>
#include <cmath>
#include <init.h>
#include <omp.h>
#include "load_data.h"
#include "ticktock.h"
#include "init.h"
#include <chrono>

int main(int argc, char* argv[]){
    std::cout <<"\n"<< std::string(30, '=') << "Calculating Walker Distribution" << std::string(30, '=')<<"\n";
    SimulationParams params;  // Declare the struct to store simulation parameters
    try {
        params = parse_arguments(argc, argv);  // Assign parsed parameters to struct from command-line arguments
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    // define a stopwatch timer:
    TickTock stopwatch;
    stopwatch.tick();     // start timer:
    int nthreads, t;

    #pragma omp parallel default(none)  shared(nthreads) private(t)
    {   t = omp_get_thread_num();
        if (t == 0)
            nthreads = omp_get_num_threads();
    }
    std::cout << "\nUsing "<< nthreads<< " threads for compute.\n";

    // Assign parsed parameters to local variables
    double base = params.base;
    std::string filename = params.file;

    // Declare a vector to store the step count
    rvector<int> step_count = load_data(filename);
    rvector<double> log_count(step_count.size());

    // Calculate the logarithm of the step count Applying the change of base formula (log_b(x) = log_a(x) / log_a(b)) 
    // Use the parallel openmp pragma to parallelize the loop, 'for' command does the worksharing among the threads
    # pragma omp parallel for default(none) shared(step_count, log_count, base)
        for (int i = 0; i < step_count.size(); i++) {
            log_count[i] = std::log(step_count[i]) / std::log(base);
        }
    
    // Computing the histogram of the logarithm of the step count
    // Obtaining the min and max of the log_count vector and calculating the number of bins
    // Compute the minimum and maximum of log_count using OpenMP reduction.
    double min_log = log_count[1];
    double max_log = log_count[1];
    int num_records = log_count.size();
    // Using the reduction clause to perform the reduction operation on the variables min_log and max_log to avoid race conditions
    #pragma omp parallel for default(none) shared(log_count, num_records) reduction(min: min_log) reduction(max: max_log)
        for (int i = 0; i < num_records; i++) {
            if (log_count[i] < min_log) min_log = log_count[i];
            if (log_count[i] > max_log) max_log = log_count[i];
        }

    // Define orders corresponding to the whole number parts of the logarithms.
    // Each order represents an interval [b^k, b^(k+1)) where k is an integer.
    int start_order = static_cast<int>(std::floor(min_log));
    int end_order = static_cast<int>(std::floor(max_log));
    int num_orders = end_order - start_order + 1;

    // Each bin now has a width of 1/bin_width in log space.
    // For a given log_count value, determine its bin index.
  
    // Total number of bins: orders * bin_width per order
    double bin_width = 1;
    int num_bins = num_orders * bin_width;

    if (num_bins < 1) {
        std::cerr << "Error: Number of bins must be at least 1" << std::endl;
        return EXIT_FAILURE;
    }

    rvector<double> histogram(num_bins);
    histogram.fill(0);

    // Each bin now has a width of 1/bin_width in log space.
    // For a given log_count value, determine its bin index.
    #pragma omp parallel for default(none) shared(log_count, histogram, start_order, bin_width)
    for (int i = 0; i < log_count.size(); i++) {
        // The relative position in log space starting from start_order:
        double pos = log_count[i] - start_order;
        // Multiply by bin_width to get bins per order-of-magnitude, then floor.
        int bin = static_cast<int>(std::floor(pos * bin_width));
        #pragma omp atomic update
        histogram[bin]++;
    }
    double calctime = stopwatch.silent_tock();
    std::cout << "bin     count" << "\n";
    // Output the histogram.
    for (int i = 0; i < num_bins; i++) {
        // Lower bound in log-space
        double bin_log_start = start_order + static_cast<double>(i) / bin_width;
        // double actual_bin_start = std::pow(base, bin_log_start);     for original bin back to linear space
        std::cout << " " << bin_log_start << "    " << histogram[i] << "\n";
    }
    std::cout << "\nTiming summary:" << std::endl;
    std::cout << "Calculation time:\t" << calctime << " sec" << std::endl;
    stopwatch.tock("Total time:     "); // this combines the elapsed time measurement and output
    std::cout <<"\n"<< std::string(30, '=') << "Walker Distribution Calculation Complete" << std::string(30, '=')<<"\n\n";
    return 0;
}