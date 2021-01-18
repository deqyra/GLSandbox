#ifndef RENDERBOI__WINDOW__GAMEPAD__ABSTRACT_GAMEPAD_HPP
#define RENDERBOI__WINDOW__GAMEPAD__ABSTRACT_GAMEPAD_HPP

#include <memory>

#include "../enums.hpp"
#include "gamepad_state.hpp"

namespace Renderboi
{

/// @brief Base class for entities whose purpose is to implement concrete
/// mechanisms to access a gamepad.
class AbstractGamepad
{
public:
    using Axis = Window::Input::Gamepad::Axis;
    using Button = Window::Input::Gamepad::Button;

    /// @brief Get the current state of the gamepad.
    ///
    /// @return The current state of the gamepad.
    virtual const GamepadState& getState() = 0;
};

using AbstractGamepadPtr = std::shared_ptr<AbstractGamepad>;

}

#endif//RENDERBOI__WINDOW__GAMEPAD__ABSTRACT_GAMEPAD_HPP