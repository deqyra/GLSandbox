#include "enums.hpp"

#include <cpptools/enum_map.hpp>

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

namespace std
{
    std::string to_string(const Window::Input::Mode::Target& target)
    {
        static bool runOnce = false;
        static std::unordered_enum_map<Window::Input::Mode::Target, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Mode::Target::Cursor]              = "Cursor";
            enumNames[Window::Input::Mode::Target::StickyKeys]          = "StickyKeys";
            enumNames[Window::Input::Mode::Target::StickyMouseButtons]  = "StickyMouseButtons";
            enumNames[Window::Input::Mode::Target::LockKeyMods]         = "LockKeyMods";
            enumNames[Window::Input::Mode::Target::RawMouseMotion]      = "RawMouseMotion";

            runOnce = true;
        }

        auto it = enumNames.find(target);
        if (it != enumNames.end()) return it->second;

        return "Unknown window input mode target";
    }

    std::string to_string(const Window::Input::Mode::Value& value)
    {
        static bool runOnce = false;
        static std::unordered_enum_map<Window::Input::Mode::Value, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Mode::Value::True]             = "True";
            enumNames[Window::Input::Mode::Value::False]            = "False";
            enumNames[Window::Input::Mode::Value::NormalCursor]     = "NormalCursor";
            enumNames[Window::Input::Mode::Value::HiddenCursor]     = "HiddenCursor";
            enumNames[Window::Input::Mode::Value::DisabledCursor]   = "DisabledCursor";

            runOnce = true;
        }

        auto it = enumNames.find(value);
        if (it != enumNames.end()) return it->second;

        return "Unknown window input mode value";
    }

    std::string to_string(const Window::Input::Action& action)
    {
        static bool runOnce = false;
        static std::unordered_enum_map<Window::Input::Action, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Action::Release]   = "Release";
            enumNames[Window::Input::Action::Press]     = "Press";
            // enumNames[Window::Input::Action::Repeat]    = "Repeat";      // FIX ME IF REPEAT KEYS MUST BE HANDLED

            runOnce = true;
        }

        auto it = enumNames.find(action);
        if (it != enumNames.end()) return it->second;

        return "Unknown input action";
    }

    std::string to_string(const Window::Input::Key& key)
    {
        static bool runOnce = false;
        static std::unordered_enum_map<Window::Input::Key, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Key::Unknown]          = "Unknown";
            enumNames[Window::Input::Key::Space]            = "Space";
            enumNames[Window::Input::Key::Apostrophe]       = "Apostrophe";
            enumNames[Window::Input::Key::Comma]            = "Comma";
            enumNames[Window::Input::Key::Minus]            = "Minus";
            enumNames[Window::Input::Key::Period]           = "Period";
            enumNames[Window::Input::Key::Slash]            = "Slash";
            enumNames[Window::Input::Key::Key0]             = "Key0";
            enumNames[Window::Input::Key::Key1]             = "Key1";
            enumNames[Window::Input::Key::Key2]             = "Key2";
            enumNames[Window::Input::Key::Key3]             = "Key3";
            enumNames[Window::Input::Key::Key4]             = "Key4";
            enumNames[Window::Input::Key::Key5]             = "Key5";
            enumNames[Window::Input::Key::Key6]             = "Key6";
            enumNames[Window::Input::Key::Key7]             = "Key7";
            enumNames[Window::Input::Key::Key8]             = "Key8";
            enumNames[Window::Input::Key::Key9]             = "Key9";
            enumNames[Window::Input::Key::Semicolon]        = "Semicolon";
            enumNames[Window::Input::Key::Equal]            = "Equal";
            enumNames[Window::Input::Key::A]                = "A";
            enumNames[Window::Input::Key::B]                = "B";
            enumNames[Window::Input::Key::C]                = "C";
            enumNames[Window::Input::Key::D]                = "D";
            enumNames[Window::Input::Key::E]                = "E";
            enumNames[Window::Input::Key::F]                = "F";
            enumNames[Window::Input::Key::G]                = "G";
            enumNames[Window::Input::Key::H]                = "H";
            enumNames[Window::Input::Key::I]                = "I";
            enumNames[Window::Input::Key::J]                = "J";
            enumNames[Window::Input::Key::K]                = "K";
            enumNames[Window::Input::Key::L]                = "L";
            enumNames[Window::Input::Key::M]                = "M";
            enumNames[Window::Input::Key::N]                = "N";
            enumNames[Window::Input::Key::O]                = "O";
            enumNames[Window::Input::Key::P]                = "P";
            enumNames[Window::Input::Key::Q]                = "Q";
            enumNames[Window::Input::Key::R]                = "R";
            enumNames[Window::Input::Key::S]                = "S";
            enumNames[Window::Input::Key::T]                = "T";
            enumNames[Window::Input::Key::U]                = "U";
            enumNames[Window::Input::Key::V]                = "V";
            enumNames[Window::Input::Key::W]                = "W";
            enumNames[Window::Input::Key::X]                = "X";
            enumNames[Window::Input::Key::Y]                = "Y";
            enumNames[Window::Input::Key::Z]                = "Z";
            enumNames[Window::Input::Key::LeftBracket]      = "LeftBracket";
            enumNames[Window::Input::Key::Backslash]        = "Backslash";
            enumNames[Window::Input::Key::RightBracket]     = "RightBracket";
            enumNames[Window::Input::Key::GraveAccent]      = "GraveAccent";
            enumNames[Window::Input::Key::World1]           = "World1";
            enumNames[Window::Input::Key::World2]           = "World2";
            enumNames[Window::Input::Key::Escape]           = "Escape";
            enumNames[Window::Input::Key::Enter]            = "Enter";
            enumNames[Window::Input::Key::Tab]              = "Tab";
            enumNames[Window::Input::Key::Backspace]        = "Backspace";
            enumNames[Window::Input::Key::Insert]           = "Insert";
            enumNames[Window::Input::Key::Delete]           = "Delete";
            enumNames[Window::Input::Key::Right]            = "Right";
            enumNames[Window::Input::Key::Left]             = "Left";
            enumNames[Window::Input::Key::Down]             = "Down";
            enumNames[Window::Input::Key::Up]               = "Up";
            enumNames[Window::Input::Key::PageUp]           = "PageUp";
            enumNames[Window::Input::Key::PageDown]         = "PageDown";
            enumNames[Window::Input::Key::Home]             = "Home";
            enumNames[Window::Input::Key::End]              = "End";
            enumNames[Window::Input::Key::CapsLock]         = "CapsLock";
            enumNames[Window::Input::Key::ScrollLock]       = "ScrollLock";
            enumNames[Window::Input::Key::NumLock]          = "NumLock";
            enumNames[Window::Input::Key::PrintScreen]      = "PrintScreen";
            enumNames[Window::Input::Key::Pause]            = "Pause";
            enumNames[Window::Input::Key::F1]               = "F1";
            enumNames[Window::Input::Key::F2]               = "F2";
            enumNames[Window::Input::Key::F3]               = "F3";
            enumNames[Window::Input::Key::F4]               = "F4";
            enumNames[Window::Input::Key::F5]               = "F5";
            enumNames[Window::Input::Key::F6]               = "F6";
            enumNames[Window::Input::Key::F7]               = "F7";
            enumNames[Window::Input::Key::F8]               = "F8";
            enumNames[Window::Input::Key::F9]               = "F9";
            enumNames[Window::Input::Key::F10]              = "F10";
            enumNames[Window::Input::Key::F11]              = "F11";
            enumNames[Window::Input::Key::F12]              = "F12";
            enumNames[Window::Input::Key::F13]              = "F13";
            enumNames[Window::Input::Key::F14]              = "F14";
            enumNames[Window::Input::Key::F15]              = "F15";
            enumNames[Window::Input::Key::F16]              = "F16";
            enumNames[Window::Input::Key::F17]              = "F17";
            enumNames[Window::Input::Key::F18]              = "F18";
            enumNames[Window::Input::Key::F19]              = "F19";
            enumNames[Window::Input::Key::F20]              = "F20";
            enumNames[Window::Input::Key::F21]              = "F21";
            enumNames[Window::Input::Key::F22]              = "F22";
            enumNames[Window::Input::Key::F23]              = "F23";
            enumNames[Window::Input::Key::F24]              = "F24";
            enumNames[Window::Input::Key::F25]              = "F25";
            enumNames[Window::Input::Key::Keypad0]          = "Keypad0";
            enumNames[Window::Input::Key::Keypad1]          = "Keypad1";
            enumNames[Window::Input::Key::Keypad2]          = "Keypad2";
            enumNames[Window::Input::Key::Keypad3]          = "Keypad3";
            enumNames[Window::Input::Key::Keypad4]          = "Keypad4";
            enumNames[Window::Input::Key::Keypad5]          = "Keypad5";
            enumNames[Window::Input::Key::Keypad6]          = "Keypad6";
            enumNames[Window::Input::Key::Keypad7]          = "Keypad7";
            enumNames[Window::Input::Key::Keypad8]          = "Keypad8";
            enumNames[Window::Input::Key::Keypad9]          = "Keypad9";
            enumNames[Window::Input::Key::KeypadDecimal]    = "KeypadDecimal";
            enumNames[Window::Input::Key::KeypadDivide]     = "KeypadDivide";
            enumNames[Window::Input::Key::KeypadMultiply]   = "KeypadMultiply";
            enumNames[Window::Input::Key::KeypadSubtract]   = "KeypadSubtract";
            enumNames[Window::Input::Key::KeypadAdd]        = "KeypadAdd";
            enumNames[Window::Input::Key::KeypadEnter]      = "KeypadEnter";
            enumNames[Window::Input::Key::KeypadEqual]      = "KeypadEqual";
            enumNames[Window::Input::Key::LeftShift]        = "LeftShift";
            enumNames[Window::Input::Key::LeftControl]      = "LeftControl";
            enumNames[Window::Input::Key::LeftAlt]          = "LeftAlt";
            enumNames[Window::Input::Key::LeftSuper]        = "LeftSuper";
            enumNames[Window::Input::Key::RightShift]       = "RightShift";
            enumNames[Window::Input::Key::RightControl]     = "RightControl";
            enumNames[Window::Input::Key::RightAlt]         = "RightAlt";
            enumNames[Window::Input::Key::RightSuper]       = "RightSuper";
            enumNames[Window::Input::Key::Menu]             = "Menu";

            runOnce = true;
        }

        auto it = enumNames.find(key);
        if (it != enumNames.end()) return it->second;

        return "Unknown keyboard key";
    }

    std::string to_string(const Window::Input::Modifier& mod)
    {
        static bool runOnce = false;
        static std::unordered_enum_map<Window::Input::Modifier, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Modifier::None]        = "None";
            enumNames[Window::Input::Modifier::Shift]       = "Shift";
            enumNames[Window::Input::Modifier::Control]     = "Control";
            enumNames[Window::Input::Modifier::Alt]         = "Alt";
            enumNames[Window::Input::Modifier::Super]       = "Super";
            enumNames[Window::Input::Modifier::CapsLock]    = "CapsLock";
            enumNames[Window::Input::Modifier::NumLock]     = "NumLock";

            runOnce = true;
        }

        auto it = enumNames.find(mod);
        if (it != enumNames.end()) return it->second;

        return "Unknown keyboard modifier";
    }

    std::string to_string(const Window::Input::MouseButton& mouseButton)
    {
        static bool runOnce = false;
        static std::unordered_map<Window::Input::MouseButton, std::string> enumNames;
        static std::unordered_map<Window::Input::MouseButton, std::string> specialNames;

        if (!runOnce)
        {
            enumNames[Window::Input::MouseButton::B1]   = "B1";
            enumNames[Window::Input::MouseButton::B2]   = "B2";
            enumNames[Window::Input::MouseButton::B3]   = "B3";
            enumNames[Window::Input::MouseButton::B4]   = "B4";
            enumNames[Window::Input::MouseButton::B5]   = "B5";
            enumNames[Window::Input::MouseButton::B6]   = "B6";
            enumNames[Window::Input::MouseButton::B7]   = "B7";
            enumNames[Window::Input::MouseButton::B8]   = "B8";

            specialNames[Window::Input::MouseButton::Left]      = "Left";
            specialNames[Window::Input::MouseButton::Right]     = "Right";
            specialNames[Window::Input::MouseButton::Middle]    = "Middle";

            runOnce = true;
        }

        auto it = specialNames.find(mouseButton);
        if (it != specialNames.end()) return it->second;

        it = enumNames.find(mouseButton);
        if (it != enumNames.end()) return it->second;

        return "Unknown mouse button";
    }

    std::string to_string(const Window::Input::Joystick& joystick)
    {
        static bool runOnce = false;
        static std::unordered_map<Window::Input::Joystick, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Joystick::J1]  = "J1";
            enumNames[Window::Input::Joystick::J2]  = "J2";
            enumNames[Window::Input::Joystick::J3]  = "J3";
            enumNames[Window::Input::Joystick::J4]  = "J4";
            enumNames[Window::Input::Joystick::J5]  = "J5";
            enumNames[Window::Input::Joystick::J6]  = "J6";
            enumNames[Window::Input::Joystick::J7]  = "J7";
            enumNames[Window::Input::Joystick::J8]  = "J8";
            enumNames[Window::Input::Joystick::J9]  = "J9";
            enumNames[Window::Input::Joystick::J10] = "J10";
            enumNames[Window::Input::Joystick::J11] = "J11";
            enumNames[Window::Input::Joystick::J12] = "J12";
            enumNames[Window::Input::Joystick::J13] = "J13";
            enumNames[Window::Input::Joystick::J14] = "J14";
            enumNames[Window::Input::Joystick::J15] = "J15";
            enumNames[Window::Input::Joystick::J16] = "J16";

            runOnce = true;
        }

        auto it = enumNames.find(joystick);
        if (it != enumNames.end()) return it->second;

        return "Unknown joystick";
    }

    std::string to_string(const Window::Input::Gamepad::Button& gamepadButton)
    {
        static bool runOnce = false;
        static std::unordered_map<Window::Input::Gamepad::Button, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Gamepad::Button::A]            = "A";
            enumNames[Window::Input::Gamepad::Button::B]            = "B";
            enumNames[Window::Input::Gamepad::Button::X]            = "X";
            enumNames[Window::Input::Gamepad::Button::Y]            = "Y";
            enumNames[Window::Input::Gamepad::Button::LeftBumper]   = "LeftBumper";
            enumNames[Window::Input::Gamepad::Button::RightBumper]  = "RightBumper";
            enumNames[Window::Input::Gamepad::Button::Back]         = "Back";
            enumNames[Window::Input::Gamepad::Button::Start]        = "Start";
            enumNames[Window::Input::Gamepad::Button::Guide]        = "Guide";
            enumNames[Window::Input::Gamepad::Button::LeftThumb]    = "LeftThumb";
            enumNames[Window::Input::Gamepad::Button::RightThumb]   = "RightThumb";
            enumNames[Window::Input::Gamepad::Button::DPadUp]       = "DPadUp";
            enumNames[Window::Input::Gamepad::Button::DPadRight]    = "DPadRight";
            enumNames[Window::Input::Gamepad::Button::DPadDown]     = "DPadDown";
            enumNames[Window::Input::Gamepad::Button::DPadLeft]     = "DPadLeft";

            runOnce = true;
        }

        auto it = enumNames.find(gamepadButton);
        if (it != enumNames.end()) return it->second;

        return "Unknown gamepad button";
    }

    std::string to_string(const Window::Input::Gamepad::Axis& gamepadAxis)
    {
        static bool runOnce = false;
        static std::unordered_map<Window::Input::Gamepad::Axis, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[Window::Input::Gamepad::Axis::LeftX]          = "LeftX";
            enumNames[Window::Input::Gamepad::Axis::LeftY]          = "LeftY";
            enumNames[Window::Input::Gamepad::Axis::RightX]         = "RightX";
            enumNames[Window::Input::Gamepad::Axis::RightY]         = "RightY";
            enumNames[Window::Input::Gamepad::Axis::LeftTrigger]    = "LeftTrigger";
            enumNames[Window::Input::Gamepad::Axis::RightTrigger]   = "RightTrigger";

            runOnce = true;
        }

        auto it = enumNames.find(gamepadAxis);
        if (it != enumNames.end()) return it->second;

        return "Unknown gamepad axis";
    }
}