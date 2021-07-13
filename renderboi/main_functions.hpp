#ifndef RENDERBOI__MAIN_FUNCTIONS_HPP
#define RENDERBOI__MAIN_FUNCTIONS_HPP

#include <string>
#include <vector>

#include <renderboi/examples/gl_sandbox.hpp>
#include <renderboi/window/gl_window.hpp>

#include "renderboi_parameters.hpp"

// Process command line arguments and parse parameters
bool processArguments(const int argc, char* const* argv, RenderboiParameters& dest);

// Print usage message and exit cleanly
void printHelp();

#endif//RENDERBOI__MAIN_FUNCTIONS_HPP