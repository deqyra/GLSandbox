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
    using Worker = cpptools::Worker;
    using InterruptiblePtr = cpptools::InterruptiblePtr;

    /// @brief Pointer to the hosted sandbox.
    SandboxPtr _sandbox;

    /// @brief Parameters to run the sandbox with.
    GLSandboxParameters _params;

    /// @brief Pointer to the window to run the sandbox on.
    GLWindowPtr _window;

    /// @brief Pointer to the gamepad manager of the window.
    GamepadManagerPtr _gamepadManager;

    /// @brief Tells whether a gamepad manager was successfully retrieved from 
    /// the window (in order to avoid having to test a smart pointer).
    const bool _gamepadManagerPresent;

    /// @brief Thread on which the sandbox will run.
    Worker _worker;

    /// @brief ID returned when registering the sandbox as a critical process.
    unsigned int criticalProcessRegistrationId;

public:
    /// @brief Reference to the worker object of the sandbox runner. 
    const InterruptiblePtr worker;

    /// @param window Pointer to the window to run the sandbox on.
    /// @param params Parameters to run the sandbox with
    /// @param startNow Whether to start running the sandbox right away. If 
    /// false is provided, the sandbox will need to be started using
    /// this->worker.run().
    GLSandboxRunner(GLWindowPtr window, GLSandboxParameters params, bool startNow = true) :
        _window(window),
        _gamepadManager(_window->getGamepadManager()),
        _gamepadManagerPresent(_gamepadManager != nullptr),
        _sandbox(std::make_shared<SandboxType>()),
        _params(params),
        _worker(
            [this](){               // Run the worker on the sandbox
                _sandbox->run(_window, _params);
            },
            false                   // Don't start the worker right away
        ),
        worker(&_worker)
    {
        _sandbox->setUp(_window, _params);
        criticalProcessRegistrationId = _window->criticalEventManager.registerCriticalProcess(worker);
        if (startNow)
        {
            _worker.run();
        }
    }

    ~GLSandboxRunner()
    {
        _worker.waitUntilFinalized(true);
        _window->criticalEventManager.detachCriticalProcess(criticalProcessRegistrationId);
        _sandbox->tearDown(_window);
    }

    SandboxPtr getSandbox()
    {
        return _sandbox;
    }

    void startEventPollingLoop()
    {
        // Reset the window exit signal
        _window->signalExit(false);

        // Poll events until exit requested
        while (!_window->exitSignaled())
        {
            _window->pollEvents();

            if (_gamepadManagerPresent)
            {
                _gamepadManager->refreshGamepadStatuses();
                _gamepadManager->pollGamepadStates();
            }
            
            // Process awaiting critical events
            _window->criticalEventManager.processCriticalEvents();
        }
    }
};

}

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_RUNNER_HPP