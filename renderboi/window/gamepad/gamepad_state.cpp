#include "gamepad_state.hpp"

namespace Renderboi::Window
{

GamepadState::GamepadState() :
    Buttons{
        false,  // A
        false,  // B
        false,  // X
        false,  // Y
        false,  // LeftBumper
        false,  // RightBumper
        false,  // DPadUp
        false,  // DPadRight
        false,  // DPadDown
        false,  // DPadLeft
        false,  // LeftThumb
        false,  // RightThumb
        false,  // Select
        false,  // Start
        false   // Home
    },
    Axes{
         0.f,   // LeftX
         0.f,   // LeftY
         0.f,   // RightX
         0.f,   // RightY
        -1.f,   // LeftTrigger
        -1.f    // RightTrigger
    },
    DPad(DPadDirection::None)
{

}

GamepadState& GamepadState::operator=(GamepadState other)
{
    swap(*this, other);

    return *this;
}

bool& GamepadState::operator[](const Button& button)
{
    return Buttons[(unsigned int)button];
}

const bool& GamepadState::operator[](const Button& button) const
{
    return Buttons[(unsigned int)button];
}

float& GamepadState::operator[](const Axis& axis)
{
    return Axes[(unsigned int)axis];
}

const float& GamepadState::operator[](const Axis& axis) const
{
    return Axes[(unsigned int)axis];
}

void swap(GamepadState& left, GamepadState& right)
{
    using std::swap;

    swap(left.DPad,    right.DPad);
    swap(left.Buttons, right.Buttons);
    swap(left.Axes,    right.Axes);
}

}//namespace Renderboi::Window