#include <exception>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
#endif//_WIN32

#include <renderboi/window/enums.hpp>
#include <renderboi/window/window_factory.hpp>
#include <renderboi/window/window_backend.hpp>

#include <renderboi/examples/gl_sandbox.hpp>

#include <renderboi/utilities/gl_utilities.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include "project_env.hpp"
#include "main_functions.hpp"
#include "renderboi_parameters.hpp"

namespace rb = Renderboi;
namespace fs = std::filesystem;

/////////////////// CONCRETE WINDOW BACKEND SPECIFICATION //////////////////////

#include <renderboi/window/glfw3/glfw3_window_factory.hpp>
#include <renderboi/window/glfw3/glfw3_utilities.hpp>

static const void* UsedErrorCallback = (void*)(&rb::globalGlfwErrorCallback);

////////////////////////////////////////////////////////////////////////////////

//Initialise OpenGL and display a window with an active GL context
int main(int argc, char** argv)
{
	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

	RenderboiParameters params = {
		fs::current_path()		// .assetsPath
	};
	if (!processArguments(argc, argv, params))
	{
		printHelp();
		std::cerr 	<< "Could not parse arguments.\n"
					<< "Aborting..." << std::endl;

		return EXIT_FAILURE;
	}

	fs::path assetsDir = fs::absolute(params.assetsPath / "assets/");
	if (!fs::exists(assetsDir))
	{
		std::cerr 	<< "Error: assets/ could not be found in the current directory, "
					<< "or in the path provided by command line argument.\n"
					<< "Path currently in use: " << assetsDir.string() << "\n"
					<< "Aborting..." << std::endl;
		
		return EXIT_FAILURE;
	}

	using ReLoc = rb::ResourceLocator;
	using ReType = rb::ResourceType;
	ReLoc::setPrefixFor(ReType::ShaderSource, assetsDir / "shaders/");
	ReLoc::setPrefixFor(ReType::Texture,      assetsDir / "textures/");

    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    std::cout << COPYRIGHT_NOTICE << '\n';
	std::cout << MIT_LICENSE_NOTICE << '\n' << std::endl;

	AppWindowFactory::SetErrorCallback(UsedErrorCallback);

	if (!AppWindowFactory::InitializeBackend())
	{
		std::cout << "Failed to initialize window backend. Aborting..." << std::endl;
		return EXIT_FAILURE;
	}

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
		(*it)->setUp(window);

		std::thread th(&rb::GLSandbox::run, *it, window);
		window->startEventPollingLoop();

		th.join();
		(*it)->tearDown(window);

        delete (*it);
    }

	// Destroy window by resetting what should be the only shared pointer to it
	window.reset();
	AppWindowFactory::TerminateBackend();

	int* a = new int;
	a[3] = 0;

	return EXIT_SUCCESS;
}
