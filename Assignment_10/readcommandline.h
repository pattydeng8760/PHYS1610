/// @file readcommandline.h
///
/// Module to parse the command line for parameters.
///
/// See @ref params.h
///
/// Part of the assignment 10 of the PHY1610 Winter 2025 course.
///
#ifndef READCMDLINEH
#define READCMDLINEH

#include "params.h"

///
/// @brief Function to read parameters from the command line
///
/// @param argc  number of arguments (int)
/// @param argv  arguments (array of char*)
/// @param param the Param struct to fill (reference)
///
/// @returns 0 if all went ok, 1 if help was requested, 2 if parsing went wrong
///
int read_command_line(int argc, char* argv[], Param& param);

#endif
