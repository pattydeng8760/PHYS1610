#include <iostream>
#include <rarray.h>
#include "eigenval.h"
#include "output.h"
#include "init.h"
#include <fstream>
#include <catch2/catch_all.hpp>

// Integrated test for the full hydrogen simulation
TEST_CASE("Integrated test: Full pipeline from initMatrix to writeBiniary"){
    const int n = 27;  // Use a smaller valid input for testing

    // Initialize the matrix
    rmatrix<double> matrix = initMatrix(n);
    REQUIRE(matrix.extent(0) == n);
    REQUIRE(matrix.extent(1) == n);

    //  Compute the ground state energy and eigenvector
    double e;
    rvector<double> eigenvector(n);
    REQUIRE_NOTHROW(groundState(matrix, e, eigenvector));

    REQUIRE(fabs(e) > 0.0); // Check that the energy is positive;

    // Check that the eigenvector is not null
    double norm = 0.0;
    for (int i = 0; i < n; ++i) {
        norm += fabs(eigenvector[i]);
    }
    REQUIRE(norm > 0.0); 
    
    // Test the binary and text outputs are written
    std::string text_file = "test_data.txt";
    std::string binary_file = "test_data.bin";
    
    writeText(text_file, eigenvector);
    writeBinary(binary_file, eigenvector);
    
    // Verifying the testfile outptu
    std::ifstream textin("test_data.txt");
    double s0, s1, s2;
    textin >> s0 >> s1 >> s2;
    // check
    REQUIRE(s0==Catch::Approx(eigenvector[0]));
    REQUIRE(s1==Catch::Approx(eigenvector[1]));
    REQUIRE(s2==Catch::Approx(eigenvector[2]));
    
    std::ifstream bin("test_data.bin", std::ios::binary);
    rvector<double> b(3);
    bin.read(reinterpret_cast<char*>(b.data()), b.size()*sizeof(double));

    // check with the eigenvector calculated to be the same
    REQUIRE(b[0]==Catch::Approx(eigenvector[0]));
    REQUIRE(b[1]==Catch::Approx(eigenvector[1]));
    REQUIRE(b[2]==Catch::Approx(eigenvector[2]));

    // removing the test files
    std::remove(text_file.c_str());
    std::remove(binary_file.c_str());
}