#ifndef RENDERBOI__WINDOW__GAMEPAD_STATE_HPP
#define RENDERBOI__WINDOW__GAMEPAD_MANAGER_HPP

namespace Renderboi
{

/// @brief Aggegate type representing the state of a gamepad controller as a 
/// collection of buttons which are pressed or released, as well
/// as axes whose assigned values vary continuously.
struct GamepadState
{
    /// @brief Collection of litterals describing the possible state of a DPad.
    enum DPadDirection : unsigned char
    {
        None = 0,
        Left,
        Right,
        Up,
        Down
    };

    /// @brief Whether or not button A is pressed on the gamepad.
    bool A;

    /// @brief Whether or not button B is pressed on the gamepad.
    bool B;

    /// @brief Whether or not button X is pressed on the gamepad.
    bool X;

    /// @brief Whether or not button Y is pressed on the gamepad.
    bool Y;

    /// @brief Whether or not the left bumper is pressed on the gamepad.
    bool LeftBumper;

    /// @brief Whether or not the right bumper is pressed on the gamepad.
    bool RightBumper;

    /// @brief Whether or not the left thumbstick is pressed on the gamepad.
    bool LeftThumb;

    /// @brief Whether or not the right thumbstick is pressed on the gamepad.
    bool RightThumb;

    /// @brief Whether or not the DPad is pressed to the left on the gamepad.
    bool DPadLeft;

    /// @brief Whether or not the DPad is pressed to the right on the gamepad.
    bool DPadRight;

    /// @brief Whether or not the DPad is pressed upwards on the gamepad.
    bool DPadUp;

    /// @brief Whether or not the DPad is pressed downwards on the gamepad.
    bool DPadDown;

    /// @brief Direction in which the DPad is pressed of the gamepad.
    DPadDirection DPad;

    /// @brief Whether or not the Start button is pressed on the gamepad.
    bool Start;

    /// @brief Whether or not the Select button is pressed on the gamepad.
    bool Select;

    /// @brief Whether or not the Home button is pressed on the gamepad.
    bool Home;

    /// @brief Value of the left trigger axis on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float LeftTrigger;

    /// @brief Value of the right trigger axis on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float RightTrigger;

    /// @brief Value of the X axis of the left thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float LeftX;

    /// @brief Value of the Y axis of the left thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float LeftY;

    /// @brief Value of the X axis of the right thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float RightX;

    /// @brief Value of the Y axis of the right thumbstick on the gamepad.
    /// Guaranteed to be in [-1.f ; 1.f].
    float RightY;    
};

}

#endif//RENDERBOI__WINDOW__GAMEPAD_MANAGER_HPP