#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_RUNNER_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_RUNNER_HPP

#include <memory>
#include <thread>
#include <type_traits>

#include <cpptools/thread/worker.hpp>
#include <cpptools/thread/interfaces/interruptible.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/gamepad/gamepad_manager.hpp>

#include "gl_sandbox.hpp"
#include "gl_sandbox_parameters.hpp"

namespace Renderboi
{

/// @brief Encapsulates the instantiation, set-up, execution and tear-down of a 
/// GLSandbox.
///
/// @tparam T Type of the GLSandbox to instantiate.
template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GLSandbox, T>, GLSandbox>>
class GLSandboxRunner
{
public:
    using SandboxType = T;
    using SandboxPtr = std::shared_ptr<T>;

private:

    /// @brief Pointer to the hosted sandbox.
    SandboxPtr _sandbox;

    /// @brief Pointer to the window to run the sandbox on.
    GLWindowPtr _window;

    /// @brief Pointer to the gamepad manager of the window.
    GamepadManagerPtr _gamepadManager;

public:
    /// @param window Pointer to the window to run the sandbox on.
    /// @param params Parameters to run the sandbox with
    GLSandboxRunner(GLWindowPtr window, GLSandboxParameters params) :
        _window(window),
        _sandbox(std::make_shared<SandboxType>(window, params))
    {
    }

    ~GLSandboxRunner()
    {
        _sandbox->tearDown();
    }

    /// @brief Get a pointer to the sandbox being ran.
    ///
    /// @return A pointer to the sandbox being ran.
    SandboxPtr getSandbox()
    {
        return _sandbox;
    }

    /// @brief Poll the input of the window repeatedly, until the window's exit
    /// signal is raised by another thread. Then, the worker is requested to
    /// finalize, and the function returns when it is.
    void run()
    {
        _sandbox->setUp();

		std::thread th(&rb::GLSandbox::run, _sandbox);
		_window->startPollingLoop();

		th.join();
		_sandbox->tearDown();
    }
};

}

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_RUNNER_HPP