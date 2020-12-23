#include "control.hpp"

#include <string>

#include <renderboi/window/enums.hpp>

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
