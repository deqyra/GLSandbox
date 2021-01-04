#include <exception>
#include <iostream>
#include <string>
#include <vector>

// Header where getopt is defined
#ifdef _WIN32
	#include <windows.h>
	#include <getoptwin/getopth.h>
#elif defined __linux__
	#include <unistd.h>
#endif

#include <renderboi/window/enums.hpp>
#include <renderboi/window/window_factory.hpp>
#include <renderboi/window/window_backend.hpp>

#include <renderboi/examples/gl_sandbox.hpp>
#include <renderboi/examples/lighting_sandbox.hpp>
#include <renderboi/examples/shadow_sandbox.hpp>

#include <renderboi/utilities/gl_utilities.hpp>

#include "project_macros.hpp"

#include <renderboi/window/glfw3/glfw3_window_factory.hpp>
#include <renderboi/window/glfw3/glfw3_window_callbacks.hpp>

namespace rb = Renderboi;

static constexpr rb::WindowBackend UsedBackend = rb::WindowBackend::GLFW3;
using AppWindowFactory = rb::WindowFactory<UsedBackend>;

static const void* UsedErrorCallback = (void*)(&rb::globalGlfwErrorCallback);

// Halt the execution with a clean exit
int abortWithError(std::string message, bool terminateBackend = true);

// Instantiate all available sandboxes
std::vector<rb::GLSandbox*> createAllSandboxes();

//Initialise OpenGL and display a window with an active GL context
int main(int argc, char** argv)
{
	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    std::cout << COPYLEFT_NOTICE << std::endl;

	AppWindowFactory::SetErrorCallback(UsedErrorCallback);

	if (!AppWindowFactory::InitializeBackend())
		return EXIT_FAILURE;

	// Init window, GL context and GL pointers
	rb::GLWindowPtr window;
	try
	{
		window = AppWindowFactory::MakeWindow(
			"RenderBoi",
			1280, 720,
			GL_CONTEXT_VERSION_MAJOR, GL_CONTEXT_VERSION_MINOR,
			rb::Window::OpenGLProfile::Core,
			true
		);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown during window creation:\n"
			<< e.what() << std::endl;
		return abortWithError("Window creation failed. Aborting...");
	}

	rb::glIgnoreDebugMessagesOfType(GL_DEBUG_TYPE_PERFORMANCE_ARB);

    // Instantiate and run examples
	std::vector<rb::GLSandbox*> examples = createAllSandboxes();
    for (auto it = examples.begin(); it != examples.end(); it++)
    {
        (*it)->run(window);
        delete (*it);
    }

	// Destroy window by resetting what should be the only shared pointer to it
	window = nullptr;
	AppWindowFactory::TerminateBackend();
	return EXIT_SUCCESS;
}

int abortWithError(std::string message, bool terminateBackend)
{
	std::cerr << message;
	if (terminateBackend)
	{
		AppWindowFactory::TerminateBackend();
	}

	return EXIT_FAILURE;
}

std::vector<rb::GLSandbox*> createAllSandboxes()
{
    // Try and instantiate all sandboxes.
    try
    {
        return std::vector<rb::GLSandbox*>({
            //new rb::LightingSandbox(),
			new rb::ShadowSandbox()
        });
    }
    catch (std::runtime_error e)
    {
        std::cerr << "createAllSandboxes: " << e.what();
    }

    return std::vector<rb::GLSandbox*>();
}