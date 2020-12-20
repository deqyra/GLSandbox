#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP

#include <renderboi/window/enums.hpp>

#include <cpptools/hash_combine.hpp>

enum class ControlKind : unsigned int
{
    Keyboard,
    Mouse,
    GamepadButton,
    GamepadAxis,
    Joystick
};

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

class ControlHash
{
    std::size_t operator()(Control const& c) const 
    {
        std::size_t res = 0;
        const unsigned int* data = reinterpret_cast<const unsigned int*>(&c);
        hash_combine(res, data[0]);
        hash_combine(res, data[1]);
        return res;
    }
};


#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_HPP