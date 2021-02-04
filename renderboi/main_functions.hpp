#ifndef RENDERBOI__MAIN_FUNCTIONS_HPP
#define RENDERBOI__MAIN_FUNCTIONS_HPP

#include <string>
#include <vector>

#include <renderboi/examples/gl_sandbox.hpp>
#include <renderboi/window/gl_window.hpp>

#include "renderboi_parameters.hpp"

// Halt the execution with a clean exit
int abortWithError(const std::string& message, const bool terminateBackend = true);

// Process command line arguments and parse parameters
bool processArguments(const int argc, char* const* argv, RenderboiParameters& dest);

// Print usage message and exit cleanly
void printHelp();

// Instantiate all available sandboxes
std::vector<Renderboi::GLSandbox*> createAllSandboxes();

#endif//RENDERBOI__MAIN_FUNCTIONS_HPP