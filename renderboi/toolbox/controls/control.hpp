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

    /// @param key Litteral describing the key to assign to the control.
    Control(const Window::Input::Key key);

    /// @param mouseButton Litteral describing the mouse button to assign to the
    /// control.
    Control(const Window::Input::MouseButton mouseButton);

    bool operator==(const Control& other);

    bool operator<(const Control& other);
};

bool operator==(const Control& left, const Control& right);

bool operator<(const Control& left, const Control& right);

class ControlHash
{
    public:
        std::size_t operator()(Control const& c) const;
};

namespace std
{
    string to_string(const Control& control);

    template<>
    class less<Control>
    {
        public:
            bool operator()(const Control& left, const Control& right) const;
    };
}

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP