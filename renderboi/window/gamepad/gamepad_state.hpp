#ifndef RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_STATE_HPP
#define RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_STATE_HPP

#include "../enums.hpp"

namespace Renderboi
{
namespace Window
{

/// @brief Class representing the state of a gamepad controller as a collection 
/// of buttons which are pressed or released, as well as axes whose assigned 
/// values vary continuously.
class GamepadState
{
friend void swap(GamepadState&, GamepadState&);

public:
    using Button = Window::Input::Gamepad::Button;
    using Axis = Window::Input::Gamepad::Axis;

    GamepadState();

    GamepadState(const GamepadState& other) = default;

    /// @brief Collection of litterals describing the possible state of a DPad.
    enum DPadDirection : unsigned char
    {
        None = 0,
        Left,
        Right,
        Up,
        Down
    };

    /// @brief Direction in which the DPad is pressed of the gamepad.
    DPadDirection DPad;

    /// @brief Array containing the state of all buttons on the gamepad.
    bool Buttons[15];

    /// @brief Array containing the value of all axes on the gamepad.
    float Axes[6];

    /// @brief Whether or not button A is pressed on the gamepad.
    bool& A = Buttons[(unsigned int)Button::A];

    /// @brief Whether or not button B is pressed on the gamepad.
    bool& B = Buttons[(unsigned int)Button::B];

    /// @brief Whether or not button X is pressed on the gamepad.
    bool& X = Buttons[(unsigned int)Button::X];

    /// @brief Whether or not button Y is pressed on the gamepad.
    bool& Y = Buttons[(unsigned int)Button::Y];

    /// @brief Whether or not the left bumper is pressed on the gamepad.
    bool& LeftBumper = Buttons[(unsigned int)Button::LeftBumper];

    /// @brief Whether or not the right bumper is pressed on the gamepad.
    bool& RightBumper = Buttons[(unsigned int)Button::RightBumper];

    /// @brief Whether or not the Select button is pressed on the gamepad.
    bool& Select = Buttons[(unsigned int)Button::Select];

    /// @brief Whether or not the Start button is pressed on the gamepad.
    bool& Start = Buttons[(unsigned int)Button::Start];

    /// @brief Whether or not the Home button is pressed on the gamepad.
    bool& Home = Buttons[(unsigned int)Button::Home];

    /// @brief Whether or not the left thumbstick is pressed on the gamepad.
    bool& LeftThumb = Buttons[(unsigned int)Button::LeftThumb];

    /// @brief Whether or not the right thumbstick is pressed on the gamepad.
    bool& RightThumb = Buttons[(unsigned int)Button::RightThumb];

    /// @brief Whether or not the DPad is pressed upwards on the gamepad.
    bool& DPadUp = Buttons[(unsigned int)Button::DPadUp];

    /// @brief Whether or not the DPad is pressed to the right on the gamepad.
    bool& DPadRight = Buttons[(unsigned int)Button::DPadRight];

    /// @brief Whether or not the DPad is pressed downwards on the gamepad.
    bool& DPadDown = Buttons[(unsigned int)Button::DPadDown];

    /// @brief Whether or not the DPad is pressed to the left on the gamepad.
    bool& DPadLeft = Buttons[(unsigned int)Button::DPadLeft];

    /// @brief Value of the X axis of the left thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at 0.f.
    float& LeftX = Axes[(unsigned int)Axis::LeftX];

    /// @brief Value of the Y axis of the left thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at 0.f.
    float& LeftY = Axes[(unsigned int)Axis::LeftY];

    /// @brief Value of the X axis of the right thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at 0.f.
    float& RightX = Axes[(unsigned int)Axis::RightX];

    /// @brief Value of the Y axis of the right thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at 0.f.
    float& RightY = Axes[(unsigned int)Axis::RightY];

    /// @brief Value of the left trigger axis on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at -1.f.
    float& LeftTrigger = Axes[(unsigned int)Axis::LeftTrigger];

    /// @brief Value of the right trigger axis on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f]. Rest position at -1.f.
    float& RightTrigger = Axes[(unsigned int)Axis::RightTrigger];

    /// @brief Quick access for a button in the array.
    ///
    /// @param button Litteral describing the button whose state to access.
    bool& operator[](const Button& button);

    /// @brief Quick access for a button in the array.
    ///
    /// @param button Litteral describing the button whose state to access.
    const bool& operator[](const Button& button) const;

    /// @brief Quick access for an axis in the array.
    ///
    /// @param button Litteral describing the axis whose value to access.
    float& operator[](const Axis& button);

    /// @brief Quick access for an axis in the array.
    ///
    /// @param button Litteral describing the axis whose value to access.
    const float& operator[](const Axis& button) const;

    /// @param other GamepadState to copy state from.
    GamepadState& operator=(GamepadState other);
};

}//namespace Window

using GamepadState = Window::GamepadState;

}//namespace Renderboi

#endif//RENDERBOI__WINDOW__GAMEPAD__GAMEPAD_STATE_HPP