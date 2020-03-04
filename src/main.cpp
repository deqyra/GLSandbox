/**
	GLTest, main.cpp
	Purpose: Display windows with different OpenGL basic examples. Uses GLFW and GLAD.

	@author François Brachais (deqyra)
	@version 1.0 10/06/2019
 */

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "tools/gl_utils.hpp"
#include "gl_sandbox.hpp"
#include "gl_sandboxes/lighting_example.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::runtime_error;
using std::vector;

// Shortcut-funciton to halt the execution of the program with a clean exit.
int abortWithError(string message, bool terminateGLFW = true);

// Instantiate all available examples
vector<GLSandbox*> createAllSandboxes();

//Initialises OpenGL and displays a window with an active GL context
int main(int argc, char** argv)
{
	glfwInit();
	// Init window, GL context and GL pointers
	GLFWwindow* window = makeWindow("Test", 1280, 720);

	// If no window could be fetched, exit with an error.
	if (!window)
	{
		return abortWithError("Window creation failed. Aborting...");
	}

	vector<GLSandbox*> examples = createAllSandboxes();
    for (auto it = examples.begin(); it != examples.end(); it++)
    {
        (*it)->run(window);
        delete (*it);
    }

	// Tear down GL context and destroy window
	destroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

int abortWithError(string message, bool terminateGLFW)
{
	cerr << message;
	if (terminateGLFW)
	{
		glfwTerminate();
	}

	return EXIT_FAILURE;
}

vector<GLSandbox*> createAllSandboxes()
{
    try
    {
        return vector<GLSandbox*>({
            new LightingExample()
        });
    }
    catch (runtime_error e)
    {
        cerr << "Exception caught: " << e.what();
    }

    return vector<GLSandbox*>();
}