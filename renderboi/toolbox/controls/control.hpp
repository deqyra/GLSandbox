#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP

#include <string>

#include <renderboi/window/enums.hpp>

#include <cpptools/utility/hash_combine.hpp>

namespace Renderboi
{

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

std::string to_string(const Control& control);

}//namespace Renderboi

namespace std
{

template<>
class less<Renderboi::Control>
{
    public:
        bool operator()(const Renderboi::Control& left, const Renderboi::Control& right) const;
};

}//namespace std


#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP