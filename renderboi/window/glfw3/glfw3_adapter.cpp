#include "glfw3_adapter.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"

namespace Renderboi::Window::GLFW3Adapter
{
    namespace EnumMaps
    {
        const std::unordered_map<unsigned int, Window::OpenGLProfile>& GlProfileEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::OpenGLProfile> map;

            if (!runOnce)
            {
                map[GLFW_OPENGL_ANY_PROFILE] = Window::OpenGLProfile::Any;
                map[GLFW_OPENGL_COMPAT_PROFILE] = Window::OpenGLProfile::Core;
                map[GLFW_OPENGL_CORE_PROFILE] = Window::OpenGLProfile::Compatibility;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Mode::Target>& InputModeTargetEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Mode::Target> map;

            if (!runOnce)
            {
                map[GLFW_CURSOR] = Window::Input::Mode::Target::Cursor;
                map[GLFW_STICKY_KEYS] = Window::Input::Mode::Target::StickyKeys;
                map[GLFW_STICKY_MOUSE_BUTTONS] = Window::Input::Mode::Target::StickyMouseButtons;
                map[GLFW_LOCK_KEY_MODS] = Window::Input::Mode::Target::LockKeyMods;
                map[GLFW_RAW_MOUSE_MOTION] = Window::Input::Mode::Target::RawMouseMotion;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Mode::Value>& InputModeValueEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Mode::Value> map;

            if (!runOnce)
            {
                map[GLFW_TRUE] = Window::Input::Mode::Value::True;
                map[GLFW_TRUE] = Window::Input::Mode::Value::False;
                map[GLFW_CURSOR_NORMAL] = Window::Input::Mode::Value::NormalCursor;
                map[GLFW_CURSOR_HIDDEN] = Window::Input::Mode::Value::HiddenCursor;
                map[GLFW_CURSOR_DISABLED] = Window::Input::Mode::Value::DisabledCursor;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Action>& ActionEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Action> map;

            if (!runOnce)
            {
                map[GLFW_RELEASE] = Window::Input::Action::Release;
                map[GLFW_PRESS] = Window::Input::Action::Press;
                // map[GLFW_REPEAT] = Window::Input::Action::Repeat;        // FIX ME IF REPEAT KEYS MUST BE HANDLED

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Key>& KeyEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Key> map;

            if (!runOnce)
            {
                map[GLFW_KEY_UNKNOWN] = Window::Input::Key::Unknown;
                map[GLFW_KEY_SPACE] = Window::Input::Key::Space;
                map[GLFW_KEY_APOSTROPHE] = Window::Input::Key::Apostrophe;
                map[GLFW_KEY_COMMA] = Window::Input::Key::Comma;
                map[GLFW_KEY_MINUS] = Window::Input::Key::Minus;
                map[GLFW_KEY_PERIOD] = Window::Input::Key::Period;
                map[GLFW_KEY_SLASH] = Window::Input::Key::Slash;
                map[GLFW_KEY_0] = Window::Input::Key::Key0;
                map[GLFW_KEY_1] = Window::Input::Key::Key1;
                map[GLFW_KEY_2] = Window::Input::Key::Key2;
                map[GLFW_KEY_3] = Window::Input::Key::Key3;
                map[GLFW_KEY_4] = Window::Input::Key::Key4;
                map[GLFW_KEY_5] = Window::Input::Key::Key5;
                map[GLFW_KEY_6] = Window::Input::Key::Key6;
                map[GLFW_KEY_7] = Window::Input::Key::Key7;
                map[GLFW_KEY_8] = Window::Input::Key::Key8;
                map[GLFW_KEY_9] = Window::Input::Key::Key9;
                map[GLFW_KEY_SEMICOLON] = Window::Input::Key::Semicolon;
                map[GLFW_KEY_EQUAL] = Window::Input::Key::Equal;
                map[GLFW_KEY_A] = Window::Input::Key::A;
                map[GLFW_KEY_B] = Window::Input::Key::B;
                map[GLFW_KEY_C] = Window::Input::Key::C;
                map[GLFW_KEY_D] = Window::Input::Key::D;
                map[GLFW_KEY_E] = Window::Input::Key::E;
                map[GLFW_KEY_F] = Window::Input::Key::F;
                map[GLFW_KEY_G] = Window::Input::Key::G;
                map[GLFW_KEY_H] = Window::Input::Key::H;
                map[GLFW_KEY_I] = Window::Input::Key::I;
                map[GLFW_KEY_J] = Window::Input::Key::J;
                map[GLFW_KEY_K] = Window::Input::Key::K;
                map[GLFW_KEY_L] = Window::Input::Key::L;
                map[GLFW_KEY_M] = Window::Input::Key::M;
                map[GLFW_KEY_N] = Window::Input::Key::N;
                map[GLFW_KEY_O] = Window::Input::Key::O;
                map[GLFW_KEY_P] = Window::Input::Key::P;
                map[GLFW_KEY_Q] = Window::Input::Key::Q;
                map[GLFW_KEY_R] = Window::Input::Key::R;
                map[GLFW_KEY_S] = Window::Input::Key::S;
                map[GLFW_KEY_T] = Window::Input::Key::T;
                map[GLFW_KEY_U] = Window::Input::Key::U;
                map[GLFW_KEY_V] = Window::Input::Key::V;
                map[GLFW_KEY_W] = Window::Input::Key::W;
                map[GLFW_KEY_X] = Window::Input::Key::X;
                map[GLFW_KEY_Y] = Window::Input::Key::Y;
                map[GLFW_KEY_Z] = Window::Input::Key::Z;
                map[GLFW_KEY_LEFT_BRACKET] = Window::Input::Key::LeftBracket;
                map[GLFW_KEY_BACKSLASH] = Window::Input::Key::Backslash;
                map[GLFW_KEY_RIGHT_BRACKET] = Window::Input::Key::RightBracket;
                map[GLFW_KEY_GRAVE_ACCENT] = Window::Input::Key::GraveAccent;
                map[GLFW_KEY_WORLD_1] = Window::Input::Key::World1;
                map[GLFW_KEY_WORLD_2] = Window::Input::Key::World2;
                map[GLFW_KEY_ESCAPE] = Window::Input::Key::Escape;
                map[GLFW_KEY_ENTER] = Window::Input::Key::Enter;
                map[GLFW_KEY_TAB] = Window::Input::Key::Tab;
                map[GLFW_KEY_BACKSPACE] = Window::Input::Key::Backspace;
                map[GLFW_KEY_INSERT] = Window::Input::Key::Insert;
                map[GLFW_KEY_DELETE] = Window::Input::Key::Delete;
                map[GLFW_KEY_RIGHT] = Window::Input::Key::Right;
                map[GLFW_KEY_LEFT] = Window::Input::Key::Left;
                map[GLFW_KEY_DOWN] = Window::Input::Key::Down;
                map[GLFW_KEY_UP] = Window::Input::Key::Up;
                map[GLFW_KEY_PAGE_UP] = Window::Input::Key::PageUp;
                map[GLFW_KEY_PAGE_DOWN] = Window::Input::Key::PageDown;
                map[GLFW_KEY_HOME] = Window::Input::Key::Home;
                map[GLFW_KEY_END] = Window::Input::Key::End;
                map[GLFW_KEY_CAPS_LOCK] = Window::Input::Key::CapsLock;
                map[GLFW_KEY_SCROLL_LOCK] = Window::Input::Key::ScrollLock;
                map[GLFW_KEY_NUM_LOCK] = Window::Input::Key::NumLock;
                map[GLFW_KEY_PRINT_SCREEN] = Window::Input::Key::PrintScreen;
                map[GLFW_KEY_PAUSE] = Window::Input::Key::Pause;
                map[GLFW_KEY_F1] = Window::Input::Key::F1;
                map[GLFW_KEY_F2] = Window::Input::Key::F2;
                map[GLFW_KEY_F3] = Window::Input::Key::F3;
                map[GLFW_KEY_F4] = Window::Input::Key::F4;
                map[GLFW_KEY_F5] = Window::Input::Key::F5;
                map[GLFW_KEY_F6] = Window::Input::Key::F6;
                map[GLFW_KEY_F7] = Window::Input::Key::F7;
                map[GLFW_KEY_F8] = Window::Input::Key::F8;
                map[GLFW_KEY_F9] = Window::Input::Key::F9;
                map[GLFW_KEY_F10] = Window::Input::Key::F10;
                map[GLFW_KEY_F11] = Window::Input::Key::F11;
                map[GLFW_KEY_F12] = Window::Input::Key::F12;
                map[GLFW_KEY_F13] = Window::Input::Key::F13;
                map[GLFW_KEY_F14] = Window::Input::Key::F14;
                map[GLFW_KEY_F15] = Window::Input::Key::F15;
                map[GLFW_KEY_F16] = Window::Input::Key::F16;
                map[GLFW_KEY_F17] = Window::Input::Key::F17;
                map[GLFW_KEY_F18] = Window::Input::Key::F18;
                map[GLFW_KEY_F19] = Window::Input::Key::F19;
                map[GLFW_KEY_F20] = Window::Input::Key::F20;
                map[GLFW_KEY_F21] = Window::Input::Key::F21;
                map[GLFW_KEY_F22] = Window::Input::Key::F22;
                map[GLFW_KEY_F23] = Window::Input::Key::F23;
                map[GLFW_KEY_F24] = Window::Input::Key::F24;
                map[GLFW_KEY_F25] = Window::Input::Key::F25;
                map[GLFW_KEY_KP_0] = Window::Input::Key::Keypad0;
                map[GLFW_KEY_KP_1] = Window::Input::Key::Keypad1;
                map[GLFW_KEY_KP_2] = Window::Input::Key::Keypad2;
                map[GLFW_KEY_KP_3] = Window::Input::Key::Keypad3;
                map[GLFW_KEY_KP_4] = Window::Input::Key::Keypad4;
                map[GLFW_KEY_KP_5] = Window::Input::Key::Keypad5;
                map[GLFW_KEY_KP_6] = Window::Input::Key::Keypad6;
                map[GLFW_KEY_KP_7] = Window::Input::Key::Keypad7;
                map[GLFW_KEY_KP_8] = Window::Input::Key::Keypad8;
                map[GLFW_KEY_KP_9] = Window::Input::Key::Keypad9;
                map[GLFW_KEY_KP_DECIMAL] = Window::Input::Key::KeypadDecimal;
                map[GLFW_KEY_KP_DIVIDE] = Window::Input::Key::KeypadDivide;
                map[GLFW_KEY_KP_MULTIPLY] = Window::Input::Key::KeypadMultiply;
                map[GLFW_KEY_KP_SUBTRACT] = Window::Input::Key::KeypadSubtract;
                map[GLFW_KEY_KP_ADD] = Window::Input::Key::KeypadAdd;
                map[GLFW_KEY_KP_ENTER] = Window::Input::Key::KeypadEnter;
                map[GLFW_KEY_KP_EQUAL] = Window::Input::Key::KeypadEqual;
                map[GLFW_KEY_LEFT_SHIFT] = Window::Input::Key::LeftShift;
                map[GLFW_KEY_LEFT_CONTROL] = Window::Input::Key::LeftControl;
                map[GLFW_KEY_LEFT_ALT] = Window::Input::Key::LeftAlt;
                map[GLFW_KEY_LEFT_SUPER] = Window::Input::Key::LeftSuper;
                map[GLFW_KEY_RIGHT_SHIFT] = Window::Input::Key::RightShift;
                map[GLFW_KEY_RIGHT_CONTROL] = Window::Input::Key::RightControl;
                map[GLFW_KEY_RIGHT_ALT] = Window::Input::Key::RightAlt;
                map[GLFW_KEY_RIGHT_SUPER] = Window::Input::Key::RightSuper;
                map[GLFW_KEY_MENU] = Window::Input::Key::Menu;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Modifier>& KeyModifierEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Modifier> map;

            if (!runOnce)
            {
                map[GLFW_MOD_SHIFT] = Window::Input::Modifier::Shift;
                map[GLFW_MOD_CONTROL] = Window::Input::Modifier::Control;
                map[GLFW_MOD_ALT] = Window::Input::Modifier::Alt;
                map[GLFW_MOD_SUPER] = Window::Input::Modifier::Super;
                map[GLFW_MOD_CAPS_LOCK] = Window::Input::Modifier::CapsLock;
                map[GLFW_MOD_NUM_LOCK] = Window::Input::Modifier::NumLock;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::MouseButton>& MouseButtonEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::MouseButton> map;

            if (!runOnce)
            {
                map[GLFW_MOUSE_BUTTON_1] = Window::Input::MouseButton::B1;
                map[GLFW_MOUSE_BUTTON_2] = Window::Input::MouseButton::B2;
                map[GLFW_MOUSE_BUTTON_3] = Window::Input::MouseButton::B3;
                map[GLFW_MOUSE_BUTTON_4] = Window::Input::MouseButton::B4;
                map[GLFW_MOUSE_BUTTON_5] = Window::Input::MouseButton::B5;
                map[GLFW_MOUSE_BUTTON_6] = Window::Input::MouseButton::B6;
                map[GLFW_MOUSE_BUTTON_7] = Window::Input::MouseButton::B7;
                map[GLFW_MOUSE_BUTTON_8] = Window::Input::MouseButton::B8;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Joystick>& JoystickEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Joystick> map;

            if (!runOnce)
            {
                map[GLFW_JOYSTICK_1] = Window::Input::Joystick::J1;
                map[GLFW_JOYSTICK_2] = Window::Input::Joystick::J2;
                map[GLFW_JOYSTICK_3] = Window::Input::Joystick::J3;
                map[GLFW_JOYSTICK_4] = Window::Input::Joystick::J4;
                map[GLFW_JOYSTICK_5] = Window::Input::Joystick::J5;
                map[GLFW_JOYSTICK_6] = Window::Input::Joystick::J6;
                map[GLFW_JOYSTICK_7] = Window::Input::Joystick::J7;
                map[GLFW_JOYSTICK_8] = Window::Input::Joystick::J8;
                map[GLFW_JOYSTICK_9] = Window::Input::Joystick::J9;
                map[GLFW_JOYSTICK_10] = Window::Input::Joystick::J10;
                map[GLFW_JOYSTICK_11] = Window::Input::Joystick::J11;
                map[GLFW_JOYSTICK_12] = Window::Input::Joystick::J12;
                map[GLFW_JOYSTICK_13] = Window::Input::Joystick::J13;
                map[GLFW_JOYSTICK_14] = Window::Input::Joystick::J14;
                map[GLFW_JOYSTICK_15] = Window::Input::Joystick::J15;
                map[GLFW_JOYSTICK_16] = Window::Input::Joystick::J16;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Gamepad::Button>& GamepadButtonEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Gamepad::Button> map;

            if (!runOnce)
            {
                map[GLFW_GAMEPAD_BUTTON_A] = Window::Input::Gamepad::Button::A;
                map[GLFW_GAMEPAD_BUTTON_B] = Window::Input::Gamepad::Button::B;
                map[GLFW_GAMEPAD_BUTTON_X] = Window::Input::Gamepad::Button::X;
                map[GLFW_GAMEPAD_BUTTON_Y] = Window::Input::Gamepad::Button::Y;
                map[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] = Window::Input::Gamepad::Button::LeftBumper;
                map[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = Window::Input::Gamepad::Button::RightBumper;
                map[GLFW_GAMEPAD_BUTTON_BACK] = Window::Input::Gamepad::Button::Select;
                map[GLFW_GAMEPAD_BUTTON_START] = Window::Input::Gamepad::Button::Start;
                map[GLFW_GAMEPAD_BUTTON_GUIDE] = Window::Input::Gamepad::Button::Home;
                map[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] = Window::Input::Gamepad::Button::LeftThumb;
                map[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] = Window::Input::Gamepad::Button::RightThumb;
                map[GLFW_GAMEPAD_BUTTON_DPAD_UP] = Window::Input::Gamepad::Button::DPadUp;
                map[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] = Window::Input::Gamepad::Button::DPadRight;
                map[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] = Window::Input::Gamepad::Button::DPadDown;
                map[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] = Window::Input::Gamepad::Button::DPadLeft;

                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<unsigned int, Window::Input::Gamepad::Axis>& GamepadAxisEnums()
        {
            static bool runOnce = false;
            static std::unordered_map<unsigned int, Window::Input::Gamepad::Axis> map;

            if (!runOnce)
            {
                map[GLFW_GAMEPAD_AXIS_LEFT_X] = Window::Input::Gamepad::Axis::LeftX;
                map[GLFW_GAMEPAD_AXIS_LEFT_Y] = Window::Input::Gamepad::Axis::LeftY;
                map[GLFW_GAMEPAD_AXIS_RIGHT_X] = Window::Input::Gamepad::Axis::RightX;
                map[GLFW_GAMEPAD_AXIS_RIGHT_Y] = Window::Input::Gamepad::Axis::RightY;
                map[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = Window::Input::Gamepad::Axis::LeftTrigger;
                map[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = Window::Input::Gamepad::Axis::RightTrigger;

                runOnce = true;
            }

            return map;
        }

    }// namespace EnumMaps

    namespace ValueMaps
    {
        const std::unordered_map<Window::OpenGLProfile, unsigned int>& GlProfileValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::OpenGLProfile, unsigned int> map;

            if (!runOnce)
            {
                map[Window::OpenGLProfile::Any] = GLFW_OPENGL_ANY_PROFILE;
                map[Window::OpenGLProfile::Core] = GLFW_OPENGL_COMPAT_PROFILE;
                map[Window::OpenGLProfile::Compatibility] = GLFW_OPENGL_CORE_PROFILE;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Mode::Target, unsigned int>& InputModeTargetValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Mode::Target, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Mode::Target::Cursor] = GLFW_CURSOR;
                map[Window::Input::Mode::Target::StickyKeys] = GLFW_STICKY_KEYS;
                map[Window::Input::Mode::Target::StickyMouseButtons] = GLFW_STICKY_MOUSE_BUTTONS;
                map[Window::Input::Mode::Target::LockKeyMods] = GLFW_LOCK_KEY_MODS;
                map[Window::Input::Mode::Target::RawMouseMotion] = GLFW_RAW_MOUSE_MOTION;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Mode::Value, unsigned int>& InputModeValueValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Mode::Value, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Mode::Value::True] = GLFW_TRUE;
                map[Window::Input::Mode::Value::False] = GLFW_TRUE;
                map[Window::Input::Mode::Value::NormalCursor] = GLFW_CURSOR_NORMAL;
                map[Window::Input::Mode::Value::HiddenCursor] = GLFW_CURSOR_HIDDEN;
                map[Window::Input::Mode::Value::DisabledCursor] = GLFW_CURSOR_DISABLED;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Action, unsigned int>& ActionValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Action, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Action::Release] = GLFW_RELEASE;
                map[Window::Input::Action::Press] = GLFW_PRESS;
                // map[Window::Input::Action::Repeat] = GLFW_REPEAT;        // FIX ME IF REPEAT KEYS MUST BE HANDLED
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Key, unsigned int>& KeyValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Key, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Key::Unknown] = GLFW_KEY_UNKNOWN;
                map[Window::Input::Key::Space] = GLFW_KEY_SPACE;
                map[Window::Input::Key::Apostrophe] = GLFW_KEY_APOSTROPHE;
                map[Window::Input::Key::Comma] = GLFW_KEY_COMMA;
                map[Window::Input::Key::Minus] = GLFW_KEY_MINUS;
                map[Window::Input::Key::Period] = GLFW_KEY_PERIOD;
                map[Window::Input::Key::Slash] = GLFW_KEY_SLASH;
                map[Window::Input::Key::Key0] = GLFW_KEY_0;
                map[Window::Input::Key::Key1] = GLFW_KEY_1;
                map[Window::Input::Key::Key2] = GLFW_KEY_2;
                map[Window::Input::Key::Key3] = GLFW_KEY_3;
                map[Window::Input::Key::Key4] = GLFW_KEY_4;
                map[Window::Input::Key::Key5] = GLFW_KEY_5;
                map[Window::Input::Key::Key6] = GLFW_KEY_6;
                map[Window::Input::Key::Key7] = GLFW_KEY_7;
                map[Window::Input::Key::Key8] = GLFW_KEY_8;
                map[Window::Input::Key::Key9] = GLFW_KEY_9;
                map[Window::Input::Key::Semicolon] = GLFW_KEY_SEMICOLON;
                map[Window::Input::Key::Equal] = GLFW_KEY_EQUAL;
                map[Window::Input::Key::A] = GLFW_KEY_A;
                map[Window::Input::Key::B] = GLFW_KEY_B;
                map[Window::Input::Key::C] = GLFW_KEY_C;
                map[Window::Input::Key::D] = GLFW_KEY_D;
                map[Window::Input::Key::E] = GLFW_KEY_E;
                map[Window::Input::Key::F] = GLFW_KEY_F;
                map[Window::Input::Key::G] = GLFW_KEY_G;
                map[Window::Input::Key::H] = GLFW_KEY_H;
                map[Window::Input::Key::I] = GLFW_KEY_I;
                map[Window::Input::Key::J] = GLFW_KEY_J;
                map[Window::Input::Key::K] = GLFW_KEY_K;
                map[Window::Input::Key::L] = GLFW_KEY_L;
                map[Window::Input::Key::M] = GLFW_KEY_M;
                map[Window::Input::Key::N] = GLFW_KEY_N;
                map[Window::Input::Key::O] = GLFW_KEY_O;
                map[Window::Input::Key::P] = GLFW_KEY_P;
                map[Window::Input::Key::Q] = GLFW_KEY_Q;
                map[Window::Input::Key::R] = GLFW_KEY_R;
                map[Window::Input::Key::S] = GLFW_KEY_S;
                map[Window::Input::Key::T] = GLFW_KEY_T;
                map[Window::Input::Key::U] = GLFW_KEY_U;
                map[Window::Input::Key::V] = GLFW_KEY_V;
                map[Window::Input::Key::W] = GLFW_KEY_W;
                map[Window::Input::Key::X] = GLFW_KEY_X;
                map[Window::Input::Key::Y] = GLFW_KEY_Y;
                map[Window::Input::Key::Z] = GLFW_KEY_Z;
                map[Window::Input::Key::LeftBracket] = GLFW_KEY_LEFT_BRACKET;
                map[Window::Input::Key::Backslash] = GLFW_KEY_BACKSLASH;
                map[Window::Input::Key::RightBracket] = GLFW_KEY_RIGHT_BRACKET;
                map[Window::Input::Key::GraveAccent] = GLFW_KEY_GRAVE_ACCENT;
                map[Window::Input::Key::World1] = GLFW_KEY_WORLD_1;
                map[Window::Input::Key::World2] = GLFW_KEY_WORLD_2;
                map[Window::Input::Key::Escape] = GLFW_KEY_ESCAPE;
                map[Window::Input::Key::Enter] = GLFW_KEY_ENTER;
                map[Window::Input::Key::Tab] = GLFW_KEY_TAB;
                map[Window::Input::Key::Backspace] = GLFW_KEY_BACKSPACE;
                map[Window::Input::Key::Insert] = GLFW_KEY_INSERT;
                map[Window::Input::Key::Delete] = GLFW_KEY_DELETE;
                map[Window::Input::Key::Right] = GLFW_KEY_RIGHT;
                map[Window::Input::Key::Left] = GLFW_KEY_LEFT;
                map[Window::Input::Key::Down] = GLFW_KEY_DOWN;
                map[Window::Input::Key::Up] = GLFW_KEY_UP;
                map[Window::Input::Key::PageUp] = GLFW_KEY_PAGE_UP;
                map[Window::Input::Key::PageDown] = GLFW_KEY_PAGE_DOWN;
                map[Window::Input::Key::Home] = GLFW_KEY_HOME;
                map[Window::Input::Key::End] = GLFW_KEY_END;
                map[Window::Input::Key::CapsLock] = GLFW_KEY_CAPS_LOCK;
                map[Window::Input::Key::ScrollLock] = GLFW_KEY_SCROLL_LOCK;
                map[Window::Input::Key::NumLock] = GLFW_KEY_NUM_LOCK;
                map[Window::Input::Key::PrintScreen] = GLFW_KEY_PRINT_SCREEN;
                map[Window::Input::Key::Pause] = GLFW_KEY_PAUSE;
                map[Window::Input::Key::F1] = GLFW_KEY_F1;
                map[Window::Input::Key::F2] = GLFW_KEY_F2;
                map[Window::Input::Key::F3] = GLFW_KEY_F3;
                map[Window::Input::Key::F4] = GLFW_KEY_F4;
                map[Window::Input::Key::F5] = GLFW_KEY_F5;
                map[Window::Input::Key::F6] = GLFW_KEY_F6;
                map[Window::Input::Key::F7] = GLFW_KEY_F7;
                map[Window::Input::Key::F8] = GLFW_KEY_F8;
                map[Window::Input::Key::F9] = GLFW_KEY_F9;
                map[Window::Input::Key::F10] = GLFW_KEY_F10;
                map[Window::Input::Key::F11] = GLFW_KEY_F11;
                map[Window::Input::Key::F12] = GLFW_KEY_F12;
                map[Window::Input::Key::F13] = GLFW_KEY_F13;
                map[Window::Input::Key::F14] = GLFW_KEY_F14;
                map[Window::Input::Key::F15] = GLFW_KEY_F15;
                map[Window::Input::Key::F16] = GLFW_KEY_F16;
                map[Window::Input::Key::F17] = GLFW_KEY_F17;
                map[Window::Input::Key::F18] = GLFW_KEY_F18;
                map[Window::Input::Key::F19] = GLFW_KEY_F19;
                map[Window::Input::Key::F20] = GLFW_KEY_F20;
                map[Window::Input::Key::F21] = GLFW_KEY_F21;
                map[Window::Input::Key::F22] = GLFW_KEY_F22;
                map[Window::Input::Key::F23] = GLFW_KEY_F23;
                map[Window::Input::Key::F24] = GLFW_KEY_F24;
                map[Window::Input::Key::F25] = GLFW_KEY_F25;
                map[Window::Input::Key::Keypad0] = GLFW_KEY_KP_0;
                map[Window::Input::Key::Keypad1] = GLFW_KEY_KP_1;
                map[Window::Input::Key::Keypad2] = GLFW_KEY_KP_2;
                map[Window::Input::Key::Keypad3] = GLFW_KEY_KP_3;
                map[Window::Input::Key::Keypad4] = GLFW_KEY_KP_4;
                map[Window::Input::Key::Keypad5] = GLFW_KEY_KP_5;
                map[Window::Input::Key::Keypad6] = GLFW_KEY_KP_6;
                map[Window::Input::Key::Keypad7] = GLFW_KEY_KP_7;
                map[Window::Input::Key::Keypad8] = GLFW_KEY_KP_8;
                map[Window::Input::Key::Keypad9] = GLFW_KEY_KP_9;
                map[Window::Input::Key::KeypadDecimal] = GLFW_KEY_KP_DECIMAL;
                map[Window::Input::Key::KeypadDivide] = GLFW_KEY_KP_DIVIDE;
                map[Window::Input::Key::KeypadMultiply] = GLFW_KEY_KP_MULTIPLY;
                map[Window::Input::Key::KeypadSubtract] = GLFW_KEY_KP_SUBTRACT;
                map[Window::Input::Key::KeypadAdd] = GLFW_KEY_KP_ADD;
                map[Window::Input::Key::KeypadEnter] = GLFW_KEY_KP_ENTER;
                map[Window::Input::Key::KeypadEqual] = GLFW_KEY_KP_EQUAL;
                map[Window::Input::Key::LeftShift] = GLFW_KEY_LEFT_SHIFT;
                map[Window::Input::Key::LeftControl] = GLFW_KEY_LEFT_CONTROL;
                map[Window::Input::Key::LeftAlt] = GLFW_KEY_LEFT_ALT;
                map[Window::Input::Key::LeftSuper] = GLFW_KEY_LEFT_SUPER;
                map[Window::Input::Key::RightShift] = GLFW_KEY_RIGHT_SHIFT;
                map[Window::Input::Key::RightControl] = GLFW_KEY_RIGHT_CONTROL;
                map[Window::Input::Key::RightAlt] = GLFW_KEY_RIGHT_ALT;
                map[Window::Input::Key::RightSuper] = GLFW_KEY_RIGHT_SUPER;
                map[Window::Input::Key::Menu] = GLFW_KEY_MENU;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Modifier, unsigned int>& KeyModifierValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Modifier, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Modifier::Shift] = GLFW_MOD_SHIFT;
                map[Window::Input::Modifier::Control] = GLFW_MOD_CONTROL;
                map[Window::Input::Modifier::Alt] = GLFW_MOD_ALT;
                map[Window::Input::Modifier::Super] = GLFW_MOD_SUPER;
                map[Window::Input::Modifier::CapsLock] = GLFW_MOD_CAPS_LOCK;
                map[Window::Input::Modifier::NumLock] = GLFW_MOD_NUM_LOCK;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::MouseButton, unsigned int>& MouseButtonValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::MouseButton, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::MouseButton::B1] = GLFW_MOUSE_BUTTON_1;
                map[Window::Input::MouseButton::B2] = GLFW_MOUSE_BUTTON_2;
                map[Window::Input::MouseButton::B3] = GLFW_MOUSE_BUTTON_3;
                map[Window::Input::MouseButton::B4] = GLFW_MOUSE_BUTTON_4;
                map[Window::Input::MouseButton::B5] = GLFW_MOUSE_BUTTON_5;
                map[Window::Input::MouseButton::B6] = GLFW_MOUSE_BUTTON_6;
                map[Window::Input::MouseButton::B7] = GLFW_MOUSE_BUTTON_7;
                map[Window::Input::MouseButton::B8] = GLFW_MOUSE_BUTTON_8;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Joystick, unsigned int>& JoystickValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Joystick, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Joystick::J1] = GLFW_JOYSTICK_1;
                map[Window::Input::Joystick::J2] = GLFW_JOYSTICK_2;
                map[Window::Input::Joystick::J3] = GLFW_JOYSTICK_3;
                map[Window::Input::Joystick::J4] = GLFW_JOYSTICK_4;
                map[Window::Input::Joystick::J5] = GLFW_JOYSTICK_5;
                map[Window::Input::Joystick::J6] = GLFW_JOYSTICK_6;
                map[Window::Input::Joystick::J7] = GLFW_JOYSTICK_7;
                map[Window::Input::Joystick::J8] = GLFW_JOYSTICK_8;
                map[Window::Input::Joystick::J9] = GLFW_JOYSTICK_9;
                map[Window::Input::Joystick::J10] = GLFW_JOYSTICK_10;
                map[Window::Input::Joystick::J11] = GLFW_JOYSTICK_11;
                map[Window::Input::Joystick::J12] = GLFW_JOYSTICK_12;
                map[Window::Input::Joystick::J13] = GLFW_JOYSTICK_13;
                map[Window::Input::Joystick::J14] = GLFW_JOYSTICK_14;
                map[Window::Input::Joystick::J15] = GLFW_JOYSTICK_15;
                map[Window::Input::Joystick::J16] = GLFW_JOYSTICK_16;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Gamepad::Button, unsigned int>& GamepadButtonValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Gamepad::Button, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Gamepad::Button::A] = GLFW_GAMEPAD_BUTTON_A;
                map[Window::Input::Gamepad::Button::B] = GLFW_GAMEPAD_BUTTON_B;
                map[Window::Input::Gamepad::Button::X] = GLFW_GAMEPAD_BUTTON_X;
                map[Window::Input::Gamepad::Button::Y] = GLFW_GAMEPAD_BUTTON_Y;
                map[Window::Input::Gamepad::Button::LeftBumper] = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
                map[Window::Input::Gamepad::Button::RightBumper] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
                map[Window::Input::Gamepad::Button::Select] = GLFW_GAMEPAD_BUTTON_BACK;
                map[Window::Input::Gamepad::Button::Start] = GLFW_GAMEPAD_BUTTON_START;
                map[Window::Input::Gamepad::Button::Home] = GLFW_GAMEPAD_BUTTON_GUIDE;
                map[Window::Input::Gamepad::Button::LeftThumb] = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
                map[Window::Input::Gamepad::Button::RightThumb] = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
                map[Window::Input::Gamepad::Button::DPadUp] = GLFW_GAMEPAD_BUTTON_DPAD_UP;
                map[Window::Input::Gamepad::Button::DPadRight] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
                map[Window::Input::Gamepad::Button::DPadDown] = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
                map[Window::Input::Gamepad::Button::DPadLeft] = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
            
                runOnce = true;
            }

            return map;
        }

        const std::unordered_map<Window::Input::Gamepad::Axis, unsigned int>& GamepadAxisValues()
        {
            static bool runOnce = false;
            static std::unordered_map<Window::Input::Gamepad::Axis, unsigned int> map;

            if (!runOnce)
            {
                map[Window::Input::Gamepad::Axis::LeftX] = GLFW_GAMEPAD_AXIS_LEFT_X;
                map[Window::Input::Gamepad::Axis::LeftY] = GLFW_GAMEPAD_AXIS_LEFT_Y;
                map[Window::Input::Gamepad::Axis::RightX] = GLFW_GAMEPAD_AXIS_RIGHT_X;
                map[Window::Input::Gamepad::Axis::RightY] = GLFW_GAMEPAD_AXIS_RIGHT_Y;
                map[Window::Input::Gamepad::Axis::LeftTrigger] = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
                map[Window::Input::Gamepad::Axis::RightTrigger] = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
            
                runOnce = true;
            }

            return map;
        }

    }// namespace ValueMaps

    template<>
    Input::Mode::Target getEnum(int constant)
    {
        return EnumMaps::InputModeTargetEnums().at(constant);
    }

    template<>
    Input::Mode::Value getEnum(int constant)
    {
        return EnumMaps::InputModeValueEnums().at(constant);
    }

    template<>
    Input::Action getEnum(int constant)
    {
        return EnumMaps::ActionEnums().at(constant);
    }

    template<>
    Input::Key getEnum(int constant)
    {
        return EnumMaps::KeyEnums().at(constant);
    }

    template<>
    Input::Modifier getEnum(int constant)
    {
        return EnumMaps::KeyModifierEnums().at(constant);
    }

    template<>
    Input::MouseButton getEnum(int constant)
    {
        return EnumMaps::MouseButtonEnums().at(constant);
    }

    template<>
    Input::Joystick getEnum(int constant)
    {
        return EnumMaps::JoystickEnums().at(constant);
    }

    template<>
    Input::Gamepad::Button getEnum(int constant)
    {
        return EnumMaps::GamepadButtonEnums().at(constant);
    }

    template<>
    Input::Gamepad::Axis getEnum(int constant)
    {
        return EnumMaps::GamepadAxisEnums().at(constant);
    }

    template<>
    OpenGLProfile getEnum(int constant)
    {
        return EnumMaps::GlProfileEnums().at(constant);
    }

    template<>
    int getValue(Input::Mode::Target literal)
    {
        return ValueMaps::InputModeTargetValues().at(literal);
    }


    template<>
    int getValue(Input::Mode::Value literal)
    {
        return ValueMaps::InputModeValueValues().at(literal);
    }


    template<>
    int getValue(Input::Action literal)
    {
        return ValueMaps::ActionValues().at(literal);
    }


    template<>
    int getValue(Input::Key literal)
    {
        return ValueMaps::KeyValues().at(literal);
    }


    template<>
    int getValue(Input::Modifier literal)
    {
        return ValueMaps::KeyModifierValues().at(literal);
    }


    template<>
    int getValue(Input::MouseButton literal)
    {
        return ValueMaps::MouseButtonValues().at(literal);
    }


    template<>
    int getValue(Input::Joystick literal)
    {
        return ValueMaps::JoystickValues().at(literal);
    }


    template<>
    int getValue(Input::Gamepad::Button literal)
    {
        return ValueMaps::GamepadButtonValues().at(literal);
    }


    template<>
    int getValue(Input::Gamepad::Axis literal)
    {
        return ValueMaps::GamepadAxisValues().at(literal);
    }


    template<>
    int getValue(OpenGLProfile literal)
    {
        return ValueMaps::GlProfileValues().at(literal);
    }
}//namespace Renderboi::Window::GLFW3Adapter
