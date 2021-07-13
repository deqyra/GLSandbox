#include "control.hpp"

#include <string>

#include <renderboi/window/enums.hpp>

namespace Renderboi
{

Control::Control(const Window::Input::Key key) :
    kind(ControlKind::Key),
    key(key)
{

}

Control::Control(const Window::Input::MouseButton mouseButton) :
    kind(ControlKind::MouseButton),
    mouseButton(mouseButton)
{

}

bool Control::operator==(const Control& other)
{
    return Renderboi::operator==(*this, other);
}

bool Control::operator<(const Control& other)
{
    return Renderboi::operator<(*this, other);
}

bool operator==(const Control& left, const Control& right)
{
    // Safe only because the Control struct is made up of enums which are all 
    // based on the unsigned int data types
    const unsigned int* dataLeft = reinterpret_cast<const unsigned int*>(&left);
    const unsigned int* dataRight = reinterpret_cast<const unsigned int*>(&right);

    return (dataLeft[0] == dataRight[0]) && (dataLeft[1] == dataRight[1]);
}

bool operator<(const Control& left, const Control& right)
{
    const unsigned int* dataLeft = reinterpret_cast<const unsigned int*>(&left);
    const unsigned int* dataRight = reinterpret_cast<const unsigned int*>(&right);

    return   (dataLeft[0] <  dataRight[0]) || 
            ((dataLeft[0] == dataRight[0]) && (dataLeft[1] < dataRight[1]));
}

std::size_t ControlHash::operator()(Control const& c) const
{
    std::size_t res = 0;

    const unsigned int* data = reinterpret_cast<const unsigned int*>(&c);
    cpptools::hash_combine(res, data[0]);
    cpptools::hash_combine(res, data[1]);

    return res;
}

std::string to_string(const Control& control)
{
    switch(control.kind)
    {
        case ControlKind::Key:
            return to_string(control.key);
        case ControlKind::MouseButton:
            return to_string(control.mouseButton);
        case ControlKind::GamepadButton:
            return to_string(control.gamepadButton);
        case ControlKind::GamepadAxis:
            return to_string(control.gamepadAxis);
        case ControlKind::Joystick:
            return to_string(control.joystick);
    }
    return "Unknown control";
}

}//namespace Renderboi

namespace std
{

bool less<Renderboi::Control>::operator()(const Renderboi::Control& left, const Renderboi::Control& right) const
{
    return left < right;
}

}
