#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP

#include <string>

#include <renderboi/window/enums.hpp>

#include <cpptools/hash_combine.hpp>

/// @brief Litterals describing the flavor of a control.
enum class ControlKind : unsigned int
{
    Key,
    MouseButton,
    GamepadButton,
    GamepadAxis,
    Joystick
};

/// @brief Struct describing a control as well as whence it originated.
struct Control
{
    ControlKind kind;
    union
    {
        Window::Input::Key key;
        Window::Input::MouseButton mouseButton;
        Window::Input::Gamepad::Button gamepadButton;
        Window::Input::Gamepad::Axis gamepadAxis;
        Window::Input::Joystick joystick;
    };
};

bool operator==(const Control& left, const Control& right);

bool operator<(const Control& left, const Control& right);

/// @brief Return a keyboard control initialized with the provided key.
///
/// @param key Litteral describing the key to assign to the control.
///
/// @return A keyboard control initialized with the provided key.
Control keyboardControl(Window::Input::Key key);

/// @brief Return a mouse button control initialized with the provided button.
///
/// @param mouseButton Litteral describing the button to assign to the control.
///
/// @return A mouse button control initialized with the provided buttony.
Control mouseButtonControl(Window::Input::MouseButton mouseButton);

class ControlHash
{
    public:
        std::size_t operator()(Control const& c) const;
};

namespace std
{
    std::string to_string(const Control& control);
}

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP