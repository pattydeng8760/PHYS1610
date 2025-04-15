/*
PHYS1610 Assignment 1
Author: Patrick Deng
Student ID: 1003037974
Write a program that can take 2d data sampled at discrete time points and compute a moving average of
the norm of the 2d points as a function of time
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <utility>
#include <exception> 

using namespace std;
namespace fs = std::filesystem; // declare the filesystem namespace

// Function that takes the variables as references, load the fil, and manipulate these references
int load_data(vector<int>& t, vector<double>& x, vector<double>& y, string filename) {
    std::ifstream file(filename);  // Open the file  for reading
    if (!file) {  // Check if the file was successfully opened
        cerr << "Unable to open file!" << endl;
        return 1;  // Return an error code if file can't be opened
    }
    int time=0;  // Declare time as an integer
    double coord_x=0, coord_y=0;        // Declare x and y as double
    while (file >> time >> coord_x >> coord_y) {  // Read t (int), x, y (double) values from file
        t.push_back(time);    // Store t as an integer in vector
        x.push_back(coord_x); // Store x as double in vector
        y.push_back(coord_y); // Store y as double in vector
    }
    file.close();  // Close the file after reading
    return 0;  // Return success
}

// Function to compute the norm of the vector that takes in the x,y vectors and returns the norm vector
// Again using references to pass the variables to avoid copying
int compute_norm(vector<double>& x, vector<double>& y,vector<double>& norm){
    cout << "Computing norm of positional vector of: "<< x.size()<<" length..." << endl;  // Print message if file exists
    // Looping over the data points to calculate the norm of the position 
    for (int i = 0; i<x.size(); i++){
        norm[i] = sqrt(pow(x[i],2)+pow(y[i],2));  // Compute the norm of the vector
    }
    return 0;
}

// Function to compute the moving average of the norm of the vector and time over the preceeding num_sum points
int compute_move_ave(vector<int>& t, vector<double>& norm, vector<double>& move_ave, vector<double>& time_ave, int n_sum){
    cout << "Computing moving average over "<< n_sum<<" elements..." << endl;  // Print message if file exists
    // Looping over all elements
    for (int j = 0; j<t.size(); j++){
        // initialize the sum of these variables to 0
        double sum_norm = 0, sum_t = 0;
        int count = 0;          // Adding a counter to keep track of the number of elements in the averaging
        // Computing the running average over the preceeding num_sum points
        for (int k = 0; k<n_sum; k++){
            // Check for valid postitive index
            if (j - k >= 0) { 
                sum_norm += norm[j-k];
                sum_t += t[j-k];
                count ++;       
            }
        }
        move_ave[j] = sum_norm/count;
        time_ave[j] = sum_t/count;
    }
    return 0;
}

// Function to write the output to a text file, also constant references to avoid copying and futher manipulation
int output_file(const vector<double>& move_ave, const vector<double>& time_ave, string filename_out){
    // Create and open the output file
    ofstream outFile(filename_out);
    // Check if the file opened successfully
    if (!outFile) { 
        cerr << "Error: Could not open file for writing!" << endl;
        throw std::runtime_error("Error: Could not open file for writing!");
    }
    // Write the data to the output file through a loop
    for (size_t i = 0; i < time_ave.size(); ++i) {
        outFile << time_ave[i] << "\t" << move_ave[i] << "\n";  // Write each pair of values
    }
    // Close the file after writing
    outFile.close();
    
    cout << "Data written to: " << filename_out<<"\n" << endl;
    return 0;
} 



int main(int argc, char* argv[]) {
    // Check if enough arguments are provided before using argv[1] and argv[2]
    if (argc < 4 || argc > 5) {
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename> <integer for summation>" << endl;
        return 1;  // Return an error code
    }

    // Declare the filename after ensuring it's safe to access argv[1]
    string filename = argv[1];
    string filename_out = argv[2];
    // Initialize vectors for data storage
    vector<int> t;                 // Temporal data
    vector<double> x, y;           // Positional data
    // Check if the file exists in the directory
    if (!fs::exists(filename)) {  // Check if the file exists in the current directory
        cerr << "\nFile does not exist!\n" << endl;       //print error message
        return 1;  // Return an error code 1
    }
    else {
        cout << "\nLoading file: "<<filename <<"..." << endl;  // Print message if file exists
    }   

    // Convert argv[2] to an integer safely by checking for the exception that the user input is non-integer
    int n_sum;
    try {
        n_sum = stoi(argv[3]);  
    } catch (const exception& e) {
        cerr << "Error: Invalid integer input for summation: " << argv[2] << endl;
        return 1;
    }
    //std::cout << "Filename: " << filename << std::endl;
    //std::cout << "num_sum: " << num_sum << std::endl;

    // worker function to load the data from the file
    load_data(t, x, y, filename);
    // The number of elements in the vector
    int num_array = t.size();
    // Vector to store the norm,moving average of norm and time of the vector with all elements initialized to 0
    vector<double> norm(num_array), move_ave(num_array), time_ave(num_array);  
    // worker function to compute the norm 
    compute_norm(x, y, norm);
    // workder function to compute the moving average
    compute_move_ave(t, norm, move_ave, time_ave, n_sum);
    // worker function to write the output to a file
    output_file(move_ave, time_ave, filename_out);
    return 0;
}
