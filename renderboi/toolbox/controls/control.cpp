#include "control.hpp"

#include <string>

#include <renderboi/window/enums.hpp>

bool operator==(const Control& left, const Control& right)
{
    const unsigned int* dataLeft = reinterpret_cast<const unsigned int*>(&left);
    const unsigned int* dataRight = reinterpret_cast<const unsigned int*>(&right);

    return (dataLeft[0] == dataRight[0]) && (dataLeft[1] == dataRight[1]);
}

bool operator<(const Control& left, const Control& right)
{
    const unsigned int* dataLeft = reinterpret_cast<const unsigned int*>(&left);
    const unsigned int* dataRight = reinterpret_cast<const unsigned int*>(&right);

    return (dataLeft[0] < dataRight[0]) || (dataLeft[1] < dataRight[1]);
}


Control keyboardControl(Window::Input::Key key)
{
    return {
        .kind = ControlKind::Key,
        .key = key
    };
}

Control mouseButtonControl(Window::Input::MouseButton mouseButton)
{
    return {
        .kind = ControlKind::MouseButton,
        .mouseButton = mouseButton
    };
}

std::size_t ControlHash::operator()(Control const& c) const
{
    std::size_t res = 0;

    const unsigned int* data = reinterpret_cast<const unsigned int*>(&c);
    hash_combine(res, data[0]);
    hash_combine(res, data[1]);

    return res;
}

namespace std
{
    std::string to_string(const Control& control)
    {
        switch(control.kind)
        {
            case ControlKind::Key:
                return std::to_string(control.key);
            case ControlKind::MouseButton:
                return std::to_string(control.mouseButton);
            case ControlKind::GamepadButton:
                return std::to_string(control.gamepadButton);
            case ControlKind::GamepadAxis:
                return std::to_string(control.gamepadAxis);
            case ControlKind::Joystick:
                return std::to_string(control.joystick);
        }
        return "Unknown control";
    }
}
