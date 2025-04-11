#include "init.h"      // Header for your init module
#include <stdexcept>   // For std::invalid_argument
#include <vector>
#include <iostream>  // For std::cerr
#include <catch2/catch_all.hpp>

// Test 1 for n = 0 (should return an empty matrix)
TEST_CASE("initMatrix returns an empty matrix for n=0", "[init]") {
    rmatrix<double> matrix = initMatrix(0);
    REQUIRE(matrix.size() == 0); // No rows should exist
}

// Test 2 for n = 1 (should return a 1x1 matrix)
TEST_CASE("initMatrix returns a 1x1 matrix for n=1", "[init]") {
    rmatrix<double> matrix = initMatrix(1);
    REQUIRE(matrix.extent(0) == 1); // One row should exist
    REQUIRE(matrix.extent(1) == 1); // One column should exist
    REQUIRE(fabs(matrix[0][0]) == Catch::Approx(97.0/2500.0));
}

// Test 2 for n = 2 (should return an error
TEST_CASE("initMatrix returns correctly sized matrix for valid n=27", "[init]") {
    rmatrix<double> matrix = initMatrix(27);
    REQUIRE(matrix.extent(0) == 27);
    REQUIRE(matrix.extent(1) == 27);
}
