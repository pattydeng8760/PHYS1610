/// @file load_data.cpp
/// @brief Function for initizlizing the computation of the histogram of the logarithm of the step count.
/// @author Patrick Deng
/// @date March 25, 2025
/// This file contains the functions for loading the data from the file 

#include "load_data.h"          ///< Header file for the load_data function

#include <fstream>              /// Library for file input/output
#include <iostream>             /// Library for standard input/output
#include <string>               /// Library for string manipulation
#include <cstdlib>              /// Library for exit() function
#include <rarray>               /// Library for rarray data structure
#include <omp.h>                /// Library for OpenMP to parallelize the i/o

rvector<int> load_data(std::string filename) {
    // Open the file in binary mode
    std::ifstream file(filename, std::ios::binary);
    if (!file) {  // Check if the file was successfully opened
        std::cerr << "Unable to open file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Determine the size of the file based on the number of records
    file.seekg(0, std::ios::end);                       // Move the file pointer to the end
    std::streampos file_size = file.tellg();            // Get the size of the file
    file.seekg(0, std::ios::beg);                       // Reset the file pointer to the beginning

    // each record is 8 characters long + 1 character for the newline
    int record_length = 9;
    // Compute the number of records.
    int count = file_size / record_length;

    // Read the entire file into a character buffer
    rvector<char> buffer(file_size);                // Create a buffer to store the file contents
    file.read(buffer.data(), file_size);            // Read the file into the buffer variable as a block of characters
    file.close();                                   // Close the file

    // vector to store the step counts as integers
    rvector<int> step_count(count);
    step_count.fill(0);

    // Parallel loop to convert each 8-character record into an integer
    #pragma omp parallel for default(none) shared(buffer, step_count, count, record_length)
    // Loop over the records in the buffer, converting each 8-character record to an integer, 
    // shared variable buffer, step_count, and count betwen threads
    for (int i = 0; i < count; i++) {
            // Extract an 8-character substring for this record
            std::string record(buffer.data() + i * record_length, 8);
            // Convert the 8 character substring to an integer through the stoi function
            int value = std::stoi(record);
            // Store the integer in the step_count vector at the corresponding index
            step_count[i] = value;
        }

    return step_count;
}