#include "enums.hpp"

unsigned int operator&(unsigned int left, Window::Input::Modifier right)
{
    return left & static_cast<unsigned int>(right);
}

unsigned int operator|(unsigned int left, Window::Input::Modifier right)
{
    return left | static_cast<unsigned int>(right);
}

bool any(Window::Input::Modifier value)
{
    return value != Window::Input::Modifier::None;
}
