// output_c2.cpp
#include "output.h"
#include <fstream>
#include <catch2/catch_all.hpp>

// Test to check if the writeText function works correctly
TEST_CASE("writeText test")
{
    // create file:
    rvector<double> a(3);
    a = 1,2,3;
    writeText("testoutputarr.txt", a);
    // read back:
    std::ifstream in("testoutputarr.txt");
    std::string s[3];
    in >> s[0] >> s[1] >> s[2];
    // check
    REQUIRE(s[0]=="1");
    REQUIRE(s[1]=="2");
    REQUIRE(s[2]=="3");
}

// Test to check the output module for the writeBinary function. for three values

TEST_CASE("writeBinary test")
{
    // create file:
    rvector<double> a(3);
    a = 1,2,3;
    writeBinary("testoutputarr.bin", a);
    // read back:
    std::ifstream in("testoutputarr.bin", std::ios::binary);
    rvector<double> b(3);
    in.read(reinterpret_cast<char*>(b.data()), b.size()*sizeof(double));
    // check with the catch approx framework for approximation
    REQUIRE(b[0]==Catch::Approx(1));
    REQUIRE(b[1]==Catch::Approx(2));
    REQUIRE(b[2]==Catch::Approx(3));
}


