#ifndef RENDERBOI__WINDOW__GAMEPAD__CONCRETE_GAMEPAD_HPP
#define RENDERBOI__WINDOW__GAMEPAD__CONCRETE_GAMEPAD_HPP

#include "../window_backend.hpp"
#include "abstract_gamepad.hpp"

namespace Renderboi
{

template <WindowBackend W>
class ConcreteGamepad : public AbstractGamepad
{
public:
    using Axis = Window::Input::Gamepad::Axis;
    using Button = Window::Input::Gamepad::Button;

    /// @brief Get the current state of the gamepad.
    ///
    /// @return The current state of the gamepad.
    const GamepadState& getState() override = delete;
};

}

#endif//RENDERBOI__WINDOW__GAMEPAD__CONCRETE_GAMEPAD_HPP