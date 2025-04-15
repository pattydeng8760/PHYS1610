#include <stdexcept>   // For std::invalid_argument
#include <vector>
#include "eigenval.h"
#include <iostream>  // For std::cerr
#include <catch2/catch_all.hpp>

// Test 1 for the groundState with a matrix with known values
TEST_CASE("groundState returns the the correct eigenvalue and eigenvector for a known matrix") {
    // Define the test matrix
    rmatrix<double> matrix(2, 2);
    matrix[0][0] = -1; matrix[0][1] = 1;
    matrix[1][0] = 2;  matrix[1][1] = 0;

    double e;
    rvector<double> a(2);  // `a` is the eigenvector returned from groundState

    // Compute the ground state energy and eigenvector
    groundState(matrix, e, a);

    // Check that the ground state energy is correct
    REQUIRE(e == Catch::Approx(-2));

    // Expected eigenvector for \lambda = -2
    rvector<double> expected_a(2);
    expected_a[0] = 1.0;
    expected_a[1] = -1.0;

    // Normalize both vectors 
    double norm_expected = sqrt(expected_a[0] * expected_a[0] + expected_a[1] * expected_a[1]);
    double norm_computed = sqrt(a[0] * a[0] + a[1] * a[1]);

    REQUIRE(a[0] / norm_computed == Catch::Approx(expected_a[0] / norm_expected));
    REQUIRE(a[1] / norm_computed == Catch::Approx(expected_a[1] / norm_expected));
}

// Test 2 for the groundState with the diagonal matrix
TEST_CASE("groundState returns the the correct eigenvalue for a diagonal matrix") {
    // Define the test matrix
    rmatrix<double> matrix(2, 2);
    matrix[0][0] = 3; matrix[0][1] = 0;
    matrix[1][0] = 0;  matrix[1][1] = 7;

    double e;
    rvector<double> a(2);  // `a` is the eigenvector returned from groundState

    // Compute the ground state energy and eigenvector
    groundState(matrix, e, a);
    // Check that the ground state energy is correct
    REQUIRE(e == Catch::Approx(3));
}

// Test 2 for the groundState with the diagonal matrix
TEST_CASE("groundState returns the an exception for a singular matrix") {
    // Define the test matrix
    rmatrix<double> matrix(2, 2);
    matrix[0][0] = 1; matrix[0][1] = 1;
    matrix[1][0] = 2;  matrix[1][1] = 0;

    double e;
    rvector<double> a(2);  // `a` is the eigenvector returned from groundState

    // Expect groundState to fail and throw an exception for the singular matrix
    REQUIRE_THROWS(groundState(matrix, e, a));
}