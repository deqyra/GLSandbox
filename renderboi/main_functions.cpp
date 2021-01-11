#include "main_functions.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// Header where getopt is defined
#ifdef _WIN32
	#include <getoptwin/getopt.h>
#elif defined __linux__
    #include <getopt.h>
#endif

#include <renderboi/examples/lighting_sandbox.hpp>
#include <renderboi/examples/shadow_sandbox.hpp>

#include "project_env.hpp"

namespace rb = Renderboi;
namespace fs = std::filesystem;

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

bool processArguments(int argc, char** argv, RenderboiParameters& dest)
{
    static option cliOptions[] = {
        {"assets", required_argument, nullptr, 'a'}
    };

    int opt;
    int index;
    while ((opt = getopt_long(argc, argv, "a:", cliOptions, &index)) != -1)
    {
        switch (opt)
        {
        case 'a':
            dest.assetsPath = fs::current_path() / fs::path(optarg);
            break;

        default:
            break;
        }
    }

    return optind >= argc;
}

void printHelp()
{
    std::cout   << PROJECT_NAME << " demo executable, v" << PROJECT_VERSION << "\n"
                << "Usage: " << PROJECT_NAME << " [(-a|--assets) <path>]\n"
                << "\n"
                << "<path>: path to the directory where assets/ is located." << std::endl;
}
