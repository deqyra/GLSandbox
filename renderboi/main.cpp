#include <stdexcept>
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
#include <renderboi/window/glfw3/glfw3_utilities.hpp>

#include <renderboi/examples/gl_sandbox.hpp>
#include <renderboi/examples/gl_sandbox_parameters.hpp>
#include <renderboi/examples/gl_sandbox_runner.hpp>
#include <renderboi/examples/lighting_sandbox.hpp>
#include <renderboi/examples/shadow_sandbox.hpp>

#include <renderboi/utilities/gl_utilities.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include "project_env.hpp"
#include "main_functions.hpp"
#include "renderboi_parameters.hpp"

namespace rb = Renderboi;
namespace rbw = rb::Window;
namespace fs = std::filesystem;

//Initialise OpenGL and display a window with an active GL context
int main(int argc, char** argv)
{
	if (AppBackend == rbw::WindowBackend::Unknown)
	{
		std::cerr << "No window backend was selected for the app. Cannot run examples." << std::endl;
		return EXIT_FAILURE;
	}

	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

	RenderboiParameters rbParams = {
		fs::current_path()		// .assetsPath
	};
	if (!processArguments(argc, argv, rbParams))
	{
		printHelp();
		std::cerr 	<< "Could not parse arguments.\n"
					<< "Aborting..." << std::endl;

		return EXIT_FAILURE;
	}

	fs::path assetsDir = fs::absolute(rbParams.assetsPath / "assets/");
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
	ReLoc::setPrefixFor(ReType::Any,          assetsDir);

    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    std::cout << COPYRIGHT_NOTICE << '\n';
	std::cout << MIT_LICENSE_NOTICE << '\n' << std::endl;

	if (!AppWindowFactory::InitializeBackend())
	{
		std::cout << "Failed to initialize window backend. Aborting..." << std::endl;
		return EXIT_FAILURE;
	}

	// Init window, GL context and GL pointers
	rbw::GLWindowPtr window;
	rbw::WindowCreationParameters windowParams = {
		"RenderBoi",						// title
		1280,								// width
		720,								// height
		true,								// resizable
		GL_CONTEXT_VERSION_MAJOR,			// glVersionMajor
		GL_CONTEXT_VERSION_MINOR,			// glVersionMinor
		rb::Window::OpenGLProfile::Core,	// glProfile
		nullptr,							// shareContext
		nullptr,							// monitor
		false,								// borderlessFullscreen
		false,								// autoMinimize
		true,								// decorated
		false,								// transparentFramebuffer
		true,								// visible
		false,								// maximized
		false,								// alwaysOnTop
		true,								// focused
		true,								// focusOnShow
		true,								// scaleToMonitor
		true								// debug
	};

	try
	{
		window = AppWindowFactory::MakeWindow(windowParams);
	}
	catch(const std::exception& e)
	{
		std::cerr 	<< "Exception thrown during window creation:\n"
				  	<< e.what() << '\n'
					<< "Window creation failed. Aborting..." << std::endl;

		AppWindowFactory::TerminateBackend();
		return EXIT_FAILURE; 
	}

	rb::glIgnoreDebugMessagesOfType(GL_DEBUG_TYPE_PERFORMANCE_ARB);

	const rb::GLSandboxParameters sbParams = {
		.debug = true
	};

    // Run examples
	rb::GLSandboxRunner<rb::ShadowSandbox> shadowSandbox =
	rb::GLSandboxRunner<rb::ShadowSandbox>(window, sbParams, false);

	shadowSandbox.worker->run();
	shadowSandbox.startEventPollingLoop();

	/* Old loop for running examples
    for (auto ex : examples)
    {
		ex->setUp(window, sbParams);

		std::thread th(&rb::GLSandbox::run, ex, window, sbParams);
		window->startEventPollingLoop();

		th.join();
		ex->tearDown(window);

        delete ex;
    }
	*/

	shadowSandbox.worker->waitUntilFinalized();

	// Destroy window by resetting what should be the only shared pointer to it
	window.reset();
	AppWindowFactory::TerminateBackend();

	return EXIT_SUCCESS + 1;
}
