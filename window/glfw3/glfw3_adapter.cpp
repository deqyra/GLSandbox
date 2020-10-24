#include "glfw3_adapter.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

std::unordered_map<unsigned int, Window::Input::Mode::Target>       Window::GLFW3Adapter::EnumMaps::_inputModeTargetEnumMap = std::unordered_map<unsigned int, Window::Input::Mode::Target>();
std::unordered_map<unsigned int, Window::Input::Mode::Value>        Window::GLFW3Adapter::EnumMaps::_inputModeValueEnumMap  = std::unordered_map<unsigned int, Window::Input::Mode::Value>();
std::unordered_map<unsigned int, Window::OpenGLProfile>             Window::GLFW3Adapter::EnumMaps::_glProfileEnumMap       = std::unordered_map<unsigned int, Window::OpenGLProfile>();
std::unordered_map<unsigned int, Window::Input::Action>             Window::GLFW3Adapter::EnumMaps::_actionEnumMap          = std::unordered_map<unsigned int, Window::Input::Action>();
std::unordered_map<unsigned int, Window::Input::Key>                Window::GLFW3Adapter::EnumMaps::_keyEnumMap             = std::unordered_map<unsigned int, Window::Input::Key>();
std::unordered_map<unsigned int, Window::Input::Modifier>           Window::GLFW3Adapter::EnumMaps::_keyModifierEnumMap     = std::unordered_map<unsigned int, Window::Input::Modifier>();
std::unordered_map<unsigned int, Window::Input::MouseButton>        Window::GLFW3Adapter::EnumMaps::_mouseButtonEnumMap     = std::unordered_map<unsigned int, Window::Input::MouseButton>();
std::unordered_map<unsigned int, Window::Input::Joystick>           Window::GLFW3Adapter::EnumMaps::_joystickEnumMap        = std::unordered_map<unsigned int, Window::Input::Joystick>();
std::unordered_map<unsigned int, Window::Input::Gamepad::Button>    Window::GLFW3Adapter::EnumMaps::_gamepadButtonEnumMap   = std::unordered_map<unsigned int, Window::Input::Gamepad::Button>();
std::unordered_map<unsigned int, Window::Input::Gamepad::Axis>      Window::GLFW3Adapter::EnumMaps::_gamepadAxisEnumMap     = std::unordered_map<unsigned int, Window::Input::Gamepad::Axis>();

std::unordered_map<Window::Input::Mode::Target, unsigned int>       Window::GLFW3Adapter::ValueMaps::_inputModeTargetValueMap = std::unordered_map<Window::Input::Mode::Target, unsigned int>();
std::unordered_map<Window::Input::Mode::Value, unsigned int>        Window::GLFW3Adapter::ValueMaps::_inputModeValueValueMap  = std::unordered_map<Window::Input::Mode::Value, unsigned int>();
std::unordered_map<Window::OpenGLProfile, unsigned int>             Window::GLFW3Adapter::ValueMaps::_glProfileValueMap       = std::unordered_map<Window::OpenGLProfile, unsigned int>();
std::unordered_map<Window::Input::Action, unsigned int>             Window::GLFW3Adapter::ValueMaps::_actionValueMap          = std::unordered_map<Window::Input::Action, unsigned int>();
std::unordered_map<Window::Input::Key, unsigned int>                Window::GLFW3Adapter::ValueMaps::_keyValueMap             = std::unordered_map<Window::Input::Key, unsigned int>();
std::unordered_map<Window::Input::Modifier, unsigned int>           Window::GLFW3Adapter::ValueMaps::_keyModifierValueMap     = std::unordered_map<Window::Input::Modifier, unsigned int>();
std::unordered_map<Window::Input::MouseButton, unsigned int>        Window::GLFW3Adapter::ValueMaps::_mouseButtonValueMap     = std::unordered_map<Window::Input::MouseButton, unsigned int>();
std::unordered_map<Window::Input::Joystick, unsigned int>           Window::GLFW3Adapter::ValueMaps::_joystickValueMap        = std::unordered_map<Window::Input::Joystick, unsigned int>();
std::unordered_map<Window::Input::Gamepad::Button, unsigned int>    Window::GLFW3Adapter::ValueMaps::_gamepadButtonValueMap   = std::unordered_map<Window::Input::Gamepad::Button, unsigned int>();
std::unordered_map<Window::Input::Gamepad::Axis, unsigned int>      Window::GLFW3Adapter::ValueMaps::_gamepadAxisValueMap     = std::unordered_map<Window::Input::Gamepad::Axis, unsigned int>();

bool Window::GLFW3Adapter::EnumMaps::_mapsPopulated = false;
bool Window::GLFW3Adapter::ValueMaps::_mapsPopulated = false;
bool Window::GLFW3Adapter::_initialized = Window::GLFW3Adapter::_initialize();

namespace Window
{
namespace GLFW3Adapter
{
    namespace EnumMaps
    {
        void populateMaps()
        {
            if (_mapsPopulated) return;

            _glProfileEnumMap[GLFW_OPENGL_ANY_PROFILE] = Window::OpenGLProfile::Any;
            _glProfileEnumMap[GLFW_OPENGL_COMPAT_PROFILE] = Window::OpenGLProfile::Core;
            _glProfileEnumMap[GLFW_OPENGL_CORE_PROFILE] = Window::OpenGLProfile::Compatibility;

            _inputModeTargetEnumMap[GLFW_CURSOR] = Window::Input::Mode::Target::Cursor;
            _inputModeTargetEnumMap[GLFW_STICKY_KEYS] = Window::Input::Mode::Target::StickyKeys;
            _inputModeTargetEnumMap[GLFW_STICKY_MOUSE_BUTTONS] = Window::Input::Mode::Target::StickyMouseButtons;
            _inputModeTargetEnumMap[GLFW_LOCK_KEY_MODS] = Window::Input::Mode::Target::LockKeyMods;
            _inputModeTargetEnumMap[GLFW_RAW_MOUSE_MOTION] = Window::Input::Mode::Target::RawMouseMotion;

            _inputModeValueEnumMap[GLFW_TRUE] = Window::Input::Mode::Value::True;
            _inputModeValueEnumMap[GLFW_TRUE] = Window::Input::Mode::Value::False;
            _inputModeValueEnumMap[GLFW_CURSOR_NORMAL] = Window::Input::Mode::Value::NormalCursor;
            _inputModeValueEnumMap[GLFW_CURSOR_HIDDEN] = Window::Input::Mode::Value::HiddenCursor;
            _inputModeValueEnumMap[GLFW_CURSOR_DISABLED] = Window::Input::Mode::Value::DisabledCursor;

            _actionEnumMap[GLFW_RELEASE] = Window::Input::Action::Release;
            _actionEnumMap[GLFW_PRESS] = Window::Input::Action::Press;
            _actionEnumMap[GLFW_REPEAT] = Window::Input::Action::Repeat;

            _keyEnumMap[GLFW_KEY_UNKNOWN] = Window::Input::Key::Unknown;
            _keyEnumMap[GLFW_KEY_SPACE] = Window::Input::Key::Space;
            _keyEnumMap[GLFW_KEY_APOSTROPHE] = Window::Input::Key::Apostrophe;
            _keyEnumMap[GLFW_KEY_COMMA] = Window::Input::Key::Comma;
            _keyEnumMap[GLFW_KEY_MINUS] = Window::Input::Key::Minus;
            _keyEnumMap[GLFW_KEY_PERIOD] = Window::Input::Key::Period;
            _keyEnumMap[GLFW_KEY_SLASH] = Window::Input::Key::Slash;
            _keyEnumMap[GLFW_KEY_0] = Window::Input::Key::Key0;
            _keyEnumMap[GLFW_KEY_1] = Window::Input::Key::Key1;
            _keyEnumMap[GLFW_KEY_2] = Window::Input::Key::Key2;
            _keyEnumMap[GLFW_KEY_3] = Window::Input::Key::Key3;
            _keyEnumMap[GLFW_KEY_4] = Window::Input::Key::Key4;
            _keyEnumMap[GLFW_KEY_5] = Window::Input::Key::Key5;
            _keyEnumMap[GLFW_KEY_6] = Window::Input::Key::Key6;
            _keyEnumMap[GLFW_KEY_7] = Window::Input::Key::Key7;
            _keyEnumMap[GLFW_KEY_8] = Window::Input::Key::Key8;
            _keyEnumMap[GLFW_KEY_9] = Window::Input::Key::Key9;
            _keyEnumMap[GLFW_KEY_SEMICOLON] = Window::Input::Key::Semicolon;
            _keyEnumMap[GLFW_KEY_EQUAL] = Window::Input::Key::Equal;
            _keyEnumMap[GLFW_KEY_A] = Window::Input::Key::A;
            _keyEnumMap[GLFW_KEY_B] = Window::Input::Key::B;
            _keyEnumMap[GLFW_KEY_C] = Window::Input::Key::C;
            _keyEnumMap[GLFW_KEY_D] = Window::Input::Key::D;
            _keyEnumMap[GLFW_KEY_E] = Window::Input::Key::E;
            _keyEnumMap[GLFW_KEY_F] = Window::Input::Key::F;
            _keyEnumMap[GLFW_KEY_G] = Window::Input::Key::G;
            _keyEnumMap[GLFW_KEY_H] = Window::Input::Key::H;
            _keyEnumMap[GLFW_KEY_I] = Window::Input::Key::I;
            _keyEnumMap[GLFW_KEY_J] = Window::Input::Key::J;
            _keyEnumMap[GLFW_KEY_K] = Window::Input::Key::K;
            _keyEnumMap[GLFW_KEY_L] = Window::Input::Key::L;
            _keyEnumMap[GLFW_KEY_M] = Window::Input::Key::M;
            _keyEnumMap[GLFW_KEY_N] = Window::Input::Key::N;
            _keyEnumMap[GLFW_KEY_O] = Window::Input::Key::O;
            _keyEnumMap[GLFW_KEY_P] = Window::Input::Key::P;
            _keyEnumMap[GLFW_KEY_Q] = Window::Input::Key::Q;
            _keyEnumMap[GLFW_KEY_R] = Window::Input::Key::R;
            _keyEnumMap[GLFW_KEY_S] = Window::Input::Key::S;
            _keyEnumMap[GLFW_KEY_T] = Window::Input::Key::T;
            _keyEnumMap[GLFW_KEY_U] = Window::Input::Key::U;
            _keyEnumMap[GLFW_KEY_V] = Window::Input::Key::V;
            _keyEnumMap[GLFW_KEY_W] = Window::Input::Key::W;
            _keyEnumMap[GLFW_KEY_X] = Window::Input::Key::X;
            _keyEnumMap[GLFW_KEY_Y] = Window::Input::Key::Y;
            _keyEnumMap[GLFW_KEY_Z] = Window::Input::Key::Z;
            _keyEnumMap[GLFW_KEY_LEFT_BRACKET] = Window::Input::Key::LeftBracket;
            _keyEnumMap[GLFW_KEY_BACKSLASH] = Window::Input::Key::Backslash;
            _keyEnumMap[GLFW_KEY_RIGHT_BRACKET] = Window::Input::Key::RightBracket;
            _keyEnumMap[GLFW_KEY_GRAVE_ACCENT] = Window::Input::Key::GraveAccent;
            _keyEnumMap[GLFW_KEY_WORLD_1] = Window::Input::Key::World1;
            _keyEnumMap[GLFW_KEY_WORLD_2] = Window::Input::Key::World2;
            _keyEnumMap[GLFW_KEY_ESCAPE] = Window::Input::Key::Escape;
            _keyEnumMap[GLFW_KEY_ENTER] = Window::Input::Key::Enter;
            _keyEnumMap[GLFW_KEY_TAB] = Window::Input::Key::Tab;
            _keyEnumMap[GLFW_KEY_BACKSPACE] = Window::Input::Key::Backspace;
            _keyEnumMap[GLFW_KEY_INSERT] = Window::Input::Key::Insert;
            _keyEnumMap[GLFW_KEY_DELETE] = Window::Input::Key::Delete;
            _keyEnumMap[GLFW_KEY_RIGHT] = Window::Input::Key::Right;
            _keyEnumMap[GLFW_KEY_LEFT] = Window::Input::Key::Left;
            _keyEnumMap[GLFW_KEY_DOWN] = Window::Input::Key::Down;
            _keyEnumMap[GLFW_KEY_UP] = Window::Input::Key::Up;
            _keyEnumMap[GLFW_KEY_PAGE_UP] = Window::Input::Key::PageUp;
            _keyEnumMap[GLFW_KEY_PAGE_DOWN] = Window::Input::Key::PageDown;
            _keyEnumMap[GLFW_KEY_HOME] = Window::Input::Key::Home;
            _keyEnumMap[GLFW_KEY_END] = Window::Input::Key::End;
            _keyEnumMap[GLFW_KEY_CAPS_LOCK] = Window::Input::Key::CapsLock;
            _keyEnumMap[GLFW_KEY_SCROLL_LOCK] = Window::Input::Key::ScrollLock;
            _keyEnumMap[GLFW_KEY_NUM_LOCK] = Window::Input::Key::NumLock;
            _keyEnumMap[GLFW_KEY_PRINT_SCREEN] = Window::Input::Key::PrintScreen;
            _keyEnumMap[GLFW_KEY_PAUSE] = Window::Input::Key::Pause;
            _keyEnumMap[GLFW_KEY_F1] = Window::Input::Key::F1;
            _keyEnumMap[GLFW_KEY_F2] = Window::Input::Key::F2;
            _keyEnumMap[GLFW_KEY_F3] = Window::Input::Key::F3;
            _keyEnumMap[GLFW_KEY_F4] = Window::Input::Key::F4;
            _keyEnumMap[GLFW_KEY_F5] = Window::Input::Key::F5;
            _keyEnumMap[GLFW_KEY_F6] = Window::Input::Key::F6;
            _keyEnumMap[GLFW_KEY_F7] = Window::Input::Key::F7;
            _keyEnumMap[GLFW_KEY_F8] = Window::Input::Key::F8;
            _keyEnumMap[GLFW_KEY_F9] = Window::Input::Key::F9;
            _keyEnumMap[GLFW_KEY_F10] = Window::Input::Key::F10;
            _keyEnumMap[GLFW_KEY_F11] = Window::Input::Key::F11;
            _keyEnumMap[GLFW_KEY_F12] = Window::Input::Key::F12;
            _keyEnumMap[GLFW_KEY_F13] = Window::Input::Key::F13;
            _keyEnumMap[GLFW_KEY_F14] = Window::Input::Key::F14;
            _keyEnumMap[GLFW_KEY_F15] = Window::Input::Key::F15;
            _keyEnumMap[GLFW_KEY_F16] = Window::Input::Key::F16;
            _keyEnumMap[GLFW_KEY_F17] = Window::Input::Key::F17;
            _keyEnumMap[GLFW_KEY_F18] = Window::Input::Key::F18;
            _keyEnumMap[GLFW_KEY_F19] = Window::Input::Key::F19;
            _keyEnumMap[GLFW_KEY_F20] = Window::Input::Key::F20;
            _keyEnumMap[GLFW_KEY_F21] = Window::Input::Key::F21;
            _keyEnumMap[GLFW_KEY_F22] = Window::Input::Key::F22;
            _keyEnumMap[GLFW_KEY_F23] = Window::Input::Key::F23;
            _keyEnumMap[GLFW_KEY_F24] = Window::Input::Key::F24;
            _keyEnumMap[GLFW_KEY_F25] = Window::Input::Key::F25;
            _keyEnumMap[GLFW_KEY_KP_0] = Window::Input::Key::Keypad0;
            _keyEnumMap[GLFW_KEY_KP_1] = Window::Input::Key::Keypad1;
            _keyEnumMap[GLFW_KEY_KP_2] = Window::Input::Key::Keypad2;
            _keyEnumMap[GLFW_KEY_KP_3] = Window::Input::Key::Keypad3;
            _keyEnumMap[GLFW_KEY_KP_4] = Window::Input::Key::Keypad4;
            _keyEnumMap[GLFW_KEY_KP_5] = Window::Input::Key::Keypad5;
            _keyEnumMap[GLFW_KEY_KP_6] = Window::Input::Key::Keypad6;
            _keyEnumMap[GLFW_KEY_KP_7] = Window::Input::Key::Keypad7;
            _keyEnumMap[GLFW_KEY_KP_8] = Window::Input::Key::Keypad8;
            _keyEnumMap[GLFW_KEY_KP_9] = Window::Input::Key::Keypad9;
            _keyEnumMap[GLFW_KEY_KP_DECIMAL] = Window::Input::Key::KeypadDecimal;
            _keyEnumMap[GLFW_KEY_KP_DIVIDE] = Window::Input::Key::KeypadDivide;
            _keyEnumMap[GLFW_KEY_KP_MULTIPLY] = Window::Input::Key::KeypadMultiply;
            _keyEnumMap[GLFW_KEY_KP_SUBTRACT] = Window::Input::Key::KeypadSubtract;
            _keyEnumMap[GLFW_KEY_KP_ADD] = Window::Input::Key::KeypadAdd;
            _keyEnumMap[GLFW_KEY_KP_ENTER] = Window::Input::Key::KeypadEnter;
            _keyEnumMap[GLFW_KEY_KP_EQUAL] = Window::Input::Key::KeypadEqual;
            _keyEnumMap[GLFW_KEY_LEFT_SHIFT] = Window::Input::Key::LeftShift;
            _keyEnumMap[GLFW_KEY_LEFT_CONTROL] = Window::Input::Key::LeftControl;
            _keyEnumMap[GLFW_KEY_LEFT_ALT] = Window::Input::Key::LeftAlt;
            _keyEnumMap[GLFW_KEY_LEFT_SUPER] = Window::Input::Key::LeftSuper;
            _keyEnumMap[GLFW_KEY_RIGHT_SHIFT] = Window::Input::Key::RightShift;
            _keyEnumMap[GLFW_KEY_RIGHT_CONTROL] = Window::Input::Key::RightControl;
            _keyEnumMap[GLFW_KEY_RIGHT_ALT] = Window::Input::Key::RightAlt;
            _keyEnumMap[GLFW_KEY_RIGHT_SUPER] = Window::Input::Key::RightSuper;
            _keyEnumMap[GLFW_KEY_MENU] = Window::Input::Key::Menu;

            _keyModifierEnumMap[GLFW_MOD_SHIFT] = Window::Input::Modifier::Shift;
            _keyModifierEnumMap[GLFW_MOD_CONTROL] = Window::Input::Modifier::Control;
            _keyModifierEnumMap[GLFW_MOD_ALT] = Window::Input::Modifier::Alt;
            _keyModifierEnumMap[GLFW_MOD_SUPER] = Window::Input::Modifier::Super;
            _keyModifierEnumMap[GLFW_MOD_CAPS_LOCK] = Window::Input::Modifier::CapsLock;
            _keyModifierEnumMap[GLFW_MOD_NUM_LOCK] = Window::Input::Modifier::NumLock;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_1] = Window::Input::MouseButton::B1;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_2] = Window::Input::MouseButton::B2;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_3] = Window::Input::MouseButton::B3;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_4] = Window::Input::MouseButton::B4;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_5] = Window::Input::MouseButton::B5;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_6] = Window::Input::MouseButton::B6;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_7] = Window::Input::MouseButton::B7;
            _mouseButtonEnumMap[GLFW_MOUSE_BUTTON_8] = Window::Input::MouseButton::B8;

            _joystickEnumMap[GLFW_JOYSTICK_1] = Window::Input::Joystick::J1;
            _joystickEnumMap[GLFW_JOYSTICK_2] = Window::Input::Joystick::J2;
            _joystickEnumMap[GLFW_JOYSTICK_3] = Window::Input::Joystick::J3;
            _joystickEnumMap[GLFW_JOYSTICK_4] = Window::Input::Joystick::J4;
            _joystickEnumMap[GLFW_JOYSTICK_5] = Window::Input::Joystick::J5;
            _joystickEnumMap[GLFW_JOYSTICK_6] = Window::Input::Joystick::J6;
            _joystickEnumMap[GLFW_JOYSTICK_7] = Window::Input::Joystick::J7;
            _joystickEnumMap[GLFW_JOYSTICK_8] = Window::Input::Joystick::J8;
            _joystickEnumMap[GLFW_JOYSTICK_9] = Window::Input::Joystick::J9;
            _joystickEnumMap[GLFW_JOYSTICK_10] = Window::Input::Joystick::J10;
            _joystickEnumMap[GLFW_JOYSTICK_11] = Window::Input::Joystick::J11;
            _joystickEnumMap[GLFW_JOYSTICK_12] = Window::Input::Joystick::J12;
            _joystickEnumMap[GLFW_JOYSTICK_13] = Window::Input::Joystick::J13;
            _joystickEnumMap[GLFW_JOYSTICK_14] = Window::Input::Joystick::J14;
            _joystickEnumMap[GLFW_JOYSTICK_15] = Window::Input::Joystick::J15;
            _joystickEnumMap[GLFW_JOYSTICK_16] = Window::Input::Joystick::J16;

            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_A] = Window::Input::Gamepad::Button::A;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_B] = Window::Input::Gamepad::Button::B;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_X] = Window::Input::Gamepad::Button::X;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_Y] = Window::Input::Gamepad::Button::Y;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] = Window::Input::Gamepad::Button::LeftBumper;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = Window::Input::Gamepad::Button::RightBumper;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_BACK] = Window::Input::Gamepad::Button::Back;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_START] = Window::Input::Gamepad::Button::Start;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_GUIDE] = Window::Input::Gamepad::Button::Guide;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] = Window::Input::Gamepad::Button::LeftThumb;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] = Window::Input::Gamepad::Button::RightThumb;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_DPAD_UP] = Window::Input::Gamepad::Button::DPadUp;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] = Window::Input::Gamepad::Button::DPadRight;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] = Window::Input::Gamepad::Button::DPadDown;
            _gamepadButtonEnumMap[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] = Window::Input::Gamepad::Button::DPadLeft;

            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_LEFT_X] = Window::Input::Gamepad::Axis::LeftX;
            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_LEFT_Y] = Window::Input::Gamepad::Axis::LeftY;
            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_RIGHT_X] = Window::Input::Gamepad::Axis::RightX;
            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_RIGHT_Y] = Window::Input::Gamepad::Axis::RightY;
            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = Window::Input::Gamepad::Axis::LeftTrigger;
            _gamepadAxisEnumMap[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = Window::Input::Gamepad::Axis::RightTrigger;

            _mapsPopulated = true;
        }
    }// namespace EnumMaps

    namespace ValueMaps
    {
        void populateMaps()
        {
            if (_mapsPopulated) return;

            _glProfileValueMap[Window::OpenGLProfile::Any] = GLFW_OPENGL_ANY_PROFILE;
            _glProfileValueMap[Window::OpenGLProfile::Core] = GLFW_OPENGL_COMPAT_PROFILE;
            _glProfileValueMap[Window::OpenGLProfile::Compatibility] = GLFW_OPENGL_CORE_PROFILE;

            _inputModeTargetValueMap[Window::Input::Mode::Target::Cursor] = GLFW_CURSOR;
            _inputModeTargetValueMap[Window::Input::Mode::Target::StickyKeys] = GLFW_STICKY_KEYS;
            _inputModeTargetValueMap[Window::Input::Mode::Target::StickyMouseButtons] = GLFW_STICKY_MOUSE_BUTTONS;
            _inputModeTargetValueMap[Window::Input::Mode::Target::LockKeyMods] = GLFW_LOCK_KEY_MODS;
            _inputModeTargetValueMap[Window::Input::Mode::Target::RawMouseMotion] = GLFW_RAW_MOUSE_MOTION;

            _inputModeValueValueMap[Window::Input::Mode::Value::True] = GLFW_TRUE;
            _inputModeValueValueMap[Window::Input::Mode::Value::False] = GLFW_TRUE;
            _inputModeValueValueMap[Window::Input::Mode::Value::NormalCursor] = GLFW_CURSOR_NORMAL;
            _inputModeValueValueMap[Window::Input::Mode::Value::HiddenCursor] = GLFW_CURSOR_HIDDEN;
            _inputModeValueValueMap[Window::Input::Mode::Value::DisabledCursor] = GLFW_CURSOR_DISABLED;

            _actionValueMap[Window::Input::Action::Release] = GLFW_RELEASE;
            _actionValueMap[Window::Input::Action::Press] = GLFW_PRESS;
            _actionValueMap[Window::Input::Action::Repeat] = GLFW_REPEAT;

            _keyValueMap[Window::Input::Key::Unknown] = GLFW_KEY_UNKNOWN;
            _keyValueMap[Window::Input::Key::Space] = GLFW_KEY_SPACE;
            _keyValueMap[Window::Input::Key::Apostrophe] = GLFW_KEY_APOSTROPHE;
            _keyValueMap[Window::Input::Key::Comma] = GLFW_KEY_COMMA;
            _keyValueMap[Window::Input::Key::Minus] = GLFW_KEY_MINUS;
            _keyValueMap[Window::Input::Key::Period] = GLFW_KEY_PERIOD;
            _keyValueMap[Window::Input::Key::Slash] = GLFW_KEY_SLASH;
            _keyValueMap[Window::Input::Key::Key0] = GLFW_KEY_0;
            _keyValueMap[Window::Input::Key::Key1] = GLFW_KEY_1;
            _keyValueMap[Window::Input::Key::Key2] = GLFW_KEY_2;
            _keyValueMap[Window::Input::Key::Key3] = GLFW_KEY_3;
            _keyValueMap[Window::Input::Key::Key4] = GLFW_KEY_4;
            _keyValueMap[Window::Input::Key::Key5] = GLFW_KEY_5;
            _keyValueMap[Window::Input::Key::Key6] = GLFW_KEY_6;
            _keyValueMap[Window::Input::Key::Key7] = GLFW_KEY_7;
            _keyValueMap[Window::Input::Key::Key8] = GLFW_KEY_8;
            _keyValueMap[Window::Input::Key::Key9] = GLFW_KEY_9;
            _keyValueMap[Window::Input::Key::Semicolon] = GLFW_KEY_SEMICOLON;
            _keyValueMap[Window::Input::Key::Equal] = GLFW_KEY_EQUAL;
            _keyValueMap[Window::Input::Key::A] = GLFW_KEY_A;
            _keyValueMap[Window::Input::Key::B] = GLFW_KEY_B;
            _keyValueMap[Window::Input::Key::C] = GLFW_KEY_C;
            _keyValueMap[Window::Input::Key::D] = GLFW_KEY_D;
            _keyValueMap[Window::Input::Key::E] = GLFW_KEY_E;
            _keyValueMap[Window::Input::Key::F] = GLFW_KEY_F;
            _keyValueMap[Window::Input::Key::G] = GLFW_KEY_G;
            _keyValueMap[Window::Input::Key::H] = GLFW_KEY_H;
            _keyValueMap[Window::Input::Key::I] = GLFW_KEY_I;
            _keyValueMap[Window::Input::Key::J] = GLFW_KEY_J;
            _keyValueMap[Window::Input::Key::K] = GLFW_KEY_K;
            _keyValueMap[Window::Input::Key::L] = GLFW_KEY_L;
            _keyValueMap[Window::Input::Key::M] = GLFW_KEY_M;
            _keyValueMap[Window::Input::Key::N] = GLFW_KEY_N;
            _keyValueMap[Window::Input::Key::O] = GLFW_KEY_O;
            _keyValueMap[Window::Input::Key::P] = GLFW_KEY_P;
            _keyValueMap[Window::Input::Key::Q] = GLFW_KEY_Q;
            _keyValueMap[Window::Input::Key::R] = GLFW_KEY_R;
            _keyValueMap[Window::Input::Key::S] = GLFW_KEY_S;
            _keyValueMap[Window::Input::Key::T] = GLFW_KEY_T;
            _keyValueMap[Window::Input::Key::U] = GLFW_KEY_U;
            _keyValueMap[Window::Input::Key::V] = GLFW_KEY_V;
            _keyValueMap[Window::Input::Key::W] = GLFW_KEY_W;
            _keyValueMap[Window::Input::Key::X] = GLFW_KEY_X;
            _keyValueMap[Window::Input::Key::Y] = GLFW_KEY_Y;
            _keyValueMap[Window::Input::Key::Z] = GLFW_KEY_Z;
            _keyValueMap[Window::Input::Key::LeftBracket] = GLFW_KEY_LEFT_BRACKET;
            _keyValueMap[Window::Input::Key::Backslash] = GLFW_KEY_BACKSLASH;
            _keyValueMap[Window::Input::Key::RightBracket] = GLFW_KEY_RIGHT_BRACKET;
            _keyValueMap[Window::Input::Key::GraveAccent] = GLFW_KEY_GRAVE_ACCENT;
            _keyValueMap[Window::Input::Key::World1] = GLFW_KEY_WORLD_1;
            _keyValueMap[Window::Input::Key::World2] = GLFW_KEY_WORLD_2;
            _keyValueMap[Window::Input::Key::Escape] = GLFW_KEY_ESCAPE;
            _keyValueMap[Window::Input::Key::Enter] = GLFW_KEY_ENTER;
            _keyValueMap[Window::Input::Key::Tab] = GLFW_KEY_TAB;
            _keyValueMap[Window::Input::Key::Backspace] = GLFW_KEY_BACKSPACE;
            _keyValueMap[Window::Input::Key::Insert] = GLFW_KEY_INSERT;
            _keyValueMap[Window::Input::Key::Delete] = GLFW_KEY_DELETE;
            _keyValueMap[Window::Input::Key::Right] = GLFW_KEY_RIGHT;
            _keyValueMap[Window::Input::Key::Left] = GLFW_KEY_LEFT;
            _keyValueMap[Window::Input::Key::Down] = GLFW_KEY_DOWN;
            _keyValueMap[Window::Input::Key::Up] = GLFW_KEY_UP;
            _keyValueMap[Window::Input::Key::PageUp] = GLFW_KEY_PAGE_UP;
            _keyValueMap[Window::Input::Key::PageDown] = GLFW_KEY_PAGE_DOWN;
            _keyValueMap[Window::Input::Key::Home] = GLFW_KEY_HOME;
            _keyValueMap[Window::Input::Key::End] = GLFW_KEY_END;
            _keyValueMap[Window::Input::Key::CapsLock] = GLFW_KEY_CAPS_LOCK;
            _keyValueMap[Window::Input::Key::ScrollLock] = GLFW_KEY_SCROLL_LOCK;
            _keyValueMap[Window::Input::Key::NumLock] = GLFW_KEY_NUM_LOCK;
            _keyValueMap[Window::Input::Key::PrintScreen] = GLFW_KEY_PRINT_SCREEN;
            _keyValueMap[Window::Input::Key::Pause] = GLFW_KEY_PAUSE;
            _keyValueMap[Window::Input::Key::F1] = GLFW_KEY_F1;
            _keyValueMap[Window::Input::Key::F2] = GLFW_KEY_F2;
            _keyValueMap[Window::Input::Key::F3] = GLFW_KEY_F3;
            _keyValueMap[Window::Input::Key::F4] = GLFW_KEY_F4;
            _keyValueMap[Window::Input::Key::F5] = GLFW_KEY_F5;
            _keyValueMap[Window::Input::Key::F6] = GLFW_KEY_F6;
            _keyValueMap[Window::Input::Key::F7] = GLFW_KEY_F7;
            _keyValueMap[Window::Input::Key::F8] = GLFW_KEY_F8;
            _keyValueMap[Window::Input::Key::F9] = GLFW_KEY_F9;
            _keyValueMap[Window::Input::Key::F10] = GLFW_KEY_F10;
            _keyValueMap[Window::Input::Key::F11] = GLFW_KEY_F11;
            _keyValueMap[Window::Input::Key::F12] = GLFW_KEY_F12;
            _keyValueMap[Window::Input::Key::F13] = GLFW_KEY_F13;
            _keyValueMap[Window::Input::Key::F14] = GLFW_KEY_F14;
            _keyValueMap[Window::Input::Key::F15] = GLFW_KEY_F15;
            _keyValueMap[Window::Input::Key::F16] = GLFW_KEY_F16;
            _keyValueMap[Window::Input::Key::F17] = GLFW_KEY_F17;
            _keyValueMap[Window::Input::Key::F18] = GLFW_KEY_F18;
            _keyValueMap[Window::Input::Key::F19] = GLFW_KEY_F19;
            _keyValueMap[Window::Input::Key::F20] = GLFW_KEY_F20;
            _keyValueMap[Window::Input::Key::F21] = GLFW_KEY_F21;
            _keyValueMap[Window::Input::Key::F22] = GLFW_KEY_F22;
            _keyValueMap[Window::Input::Key::F23] = GLFW_KEY_F23;
            _keyValueMap[Window::Input::Key::F24] = GLFW_KEY_F24;
            _keyValueMap[Window::Input::Key::F25] = GLFW_KEY_F25;
            _keyValueMap[Window::Input::Key::Keypad0] = GLFW_KEY_KP_0;
            _keyValueMap[Window::Input::Key::Keypad1] = GLFW_KEY_KP_1;
            _keyValueMap[Window::Input::Key::Keypad2] = GLFW_KEY_KP_2;
            _keyValueMap[Window::Input::Key::Keypad3] = GLFW_KEY_KP_3;
            _keyValueMap[Window::Input::Key::Keypad4] = GLFW_KEY_KP_4;
            _keyValueMap[Window::Input::Key::Keypad5] = GLFW_KEY_KP_5;
            _keyValueMap[Window::Input::Key::Keypad6] = GLFW_KEY_KP_6;
            _keyValueMap[Window::Input::Key::Keypad7] = GLFW_KEY_KP_7;
            _keyValueMap[Window::Input::Key::Keypad8] = GLFW_KEY_KP_8;
            _keyValueMap[Window::Input::Key::Keypad9] = GLFW_KEY_KP_9;
            _keyValueMap[Window::Input::Key::KeypadDecimal] = GLFW_KEY_KP_DECIMAL;
            _keyValueMap[Window::Input::Key::KeypadDivide] = GLFW_KEY_KP_DIVIDE;
            _keyValueMap[Window::Input::Key::KeypadMultiply] = GLFW_KEY_KP_MULTIPLY;
            _keyValueMap[Window::Input::Key::KeypadSubtract] = GLFW_KEY_KP_SUBTRACT;
            _keyValueMap[Window::Input::Key::KeypadAdd] = GLFW_KEY_KP_ADD;
            _keyValueMap[Window::Input::Key::KeypadEnter] = GLFW_KEY_KP_ENTER;
            _keyValueMap[Window::Input::Key::KeypadEqual] = GLFW_KEY_KP_EQUAL;
            _keyValueMap[Window::Input::Key::LeftShift] = GLFW_KEY_LEFT_SHIFT;
            _keyValueMap[Window::Input::Key::LeftControl] = GLFW_KEY_LEFT_CONTROL;
            _keyValueMap[Window::Input::Key::LeftAlt] = GLFW_KEY_LEFT_ALT;
            _keyValueMap[Window::Input::Key::LeftSuper] = GLFW_KEY_LEFT_SUPER;
            _keyValueMap[Window::Input::Key::RightShift] = GLFW_KEY_RIGHT_SHIFT;
            _keyValueMap[Window::Input::Key::RightControl] = GLFW_KEY_RIGHT_CONTROL;
            _keyValueMap[Window::Input::Key::RightAlt] = GLFW_KEY_RIGHT_ALT;
            _keyValueMap[Window::Input::Key::RightSuper] = GLFW_KEY_RIGHT_SUPER;
            _keyValueMap[Window::Input::Key::Menu] = GLFW_KEY_MENU;

            _keyModifierValueMap[Window::Input::Modifier::Shift] = GLFW_MOD_SHIFT;
            _keyModifierValueMap[Window::Input::Modifier::Control] = GLFW_MOD_CONTROL;
            _keyModifierValueMap[Window::Input::Modifier::Alt] = GLFW_MOD_ALT;
            _keyModifierValueMap[Window::Input::Modifier::Super] = GLFW_MOD_SUPER;
            _keyModifierValueMap[Window::Input::Modifier::CapsLock] = GLFW_MOD_CAPS_LOCK;
            _keyModifierValueMap[Window::Input::Modifier::NumLock] = GLFW_MOD_NUM_LOCK;
            _mouseButtonValueMap[Window::Input::MouseButton::B1] = GLFW_MOUSE_BUTTON_1;
            _mouseButtonValueMap[Window::Input::MouseButton::B2] = GLFW_MOUSE_BUTTON_2;
            _mouseButtonValueMap[Window::Input::MouseButton::B3] = GLFW_MOUSE_BUTTON_3;
            _mouseButtonValueMap[Window::Input::MouseButton::B4] = GLFW_MOUSE_BUTTON_4;
            _mouseButtonValueMap[Window::Input::MouseButton::B5] = GLFW_MOUSE_BUTTON_5;
            _mouseButtonValueMap[Window::Input::MouseButton::B6] = GLFW_MOUSE_BUTTON_6;
            _mouseButtonValueMap[Window::Input::MouseButton::B7] = GLFW_MOUSE_BUTTON_7;
            _mouseButtonValueMap[Window::Input::MouseButton::B8] = GLFW_MOUSE_BUTTON_8;

            _joystickValueMap[Window::Input::Joystick::J1] = GLFW_JOYSTICK_1;
            _joystickValueMap[Window::Input::Joystick::J2] = GLFW_JOYSTICK_2;
            _joystickValueMap[Window::Input::Joystick::J3] = GLFW_JOYSTICK_3;
            _joystickValueMap[Window::Input::Joystick::J4] = GLFW_JOYSTICK_4;
            _joystickValueMap[Window::Input::Joystick::J5] = GLFW_JOYSTICK_5;
            _joystickValueMap[Window::Input::Joystick::J6] = GLFW_JOYSTICK_6;
            _joystickValueMap[Window::Input::Joystick::J7] = GLFW_JOYSTICK_7;
            _joystickValueMap[Window::Input::Joystick::J8] = GLFW_JOYSTICK_8;
            _joystickValueMap[Window::Input::Joystick::J9] = GLFW_JOYSTICK_9;
            _joystickValueMap[Window::Input::Joystick::J10] = GLFW_JOYSTICK_10;
            _joystickValueMap[Window::Input::Joystick::J11] = GLFW_JOYSTICK_11;
            _joystickValueMap[Window::Input::Joystick::J12] = GLFW_JOYSTICK_12;
            _joystickValueMap[Window::Input::Joystick::J13] = GLFW_JOYSTICK_13;
            _joystickValueMap[Window::Input::Joystick::J14] = GLFW_JOYSTICK_14;
            _joystickValueMap[Window::Input::Joystick::J15] = GLFW_JOYSTICK_15;
            _joystickValueMap[Window::Input::Joystick::J16] = GLFW_JOYSTICK_16;

            _gamepadButtonValueMap[Window::Input::Gamepad::Button::A] = GLFW_GAMEPAD_BUTTON_A;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::B] = GLFW_GAMEPAD_BUTTON_B;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::X] = GLFW_GAMEPAD_BUTTON_X;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::Y] = GLFW_GAMEPAD_BUTTON_Y;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::LeftBumper] = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::RightBumper] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::Back] = GLFW_GAMEPAD_BUTTON_BACK;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::Start] = GLFW_GAMEPAD_BUTTON_START;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::Guide] = GLFW_GAMEPAD_BUTTON_GUIDE;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::LeftThumb] = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::RightThumb] = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::DPadUp] = GLFW_GAMEPAD_BUTTON_DPAD_UP;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::DPadRight] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::DPadDown] = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
            _gamepadButtonValueMap[Window::Input::Gamepad::Button::DPadLeft] = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;

            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::LeftX] = GLFW_GAMEPAD_AXIS_LEFT_X;
            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::LeftY] = GLFW_GAMEPAD_AXIS_LEFT_Y;
            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::RightX] = GLFW_GAMEPAD_AXIS_RIGHT_X;
            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::RightY] = GLFW_GAMEPAD_AXIS_RIGHT_Y;
            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::LeftTrigger] = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
            _gamepadAxisValueMap[Window::Input::Gamepad::Axis::RightTrigger] = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;

            _mapsPopulated = true;
        }
    }// namespace ValueMaps

    bool _initialize()
    {
        if (!EnumMaps::_mapsPopulated)
        {
            EnumMaps::populateMaps();
        }
        if (!ValueMaps::_mapsPopulated)
        {
            ValueMaps::populateMaps();
        }

        return EnumMaps::_mapsPopulated && ValueMaps::_mapsPopulated;
    }

    template<>
    Input::Mode::Target getEnum(int constant)
    {
        return EnumMaps::_inputModeTargetEnumMap[constant];
    }

    template<>
    Input::Mode::Value getEnum(int constant)
    {
        return EnumMaps::_inputModeValueEnumMap[constant];
    }

    template<>
    Input::Action getEnum(int constant)
    {
        return EnumMaps::_actionEnumMap[constant];
    }

    template<>
    Input::Key getEnum(int constant)
    {
        return EnumMaps::_keyEnumMap[constant];
    }

    template<>
    Input::Modifier getEnum(int constant)
    {
        return EnumMaps::_keyModifierEnumMap[constant];
    }

    template<>
    Input::MouseButton getEnum(int constant)
    {
        return EnumMaps::_mouseButtonEnumMap[constant];
    }

    template<>
    Input::Joystick getEnum(int constant)
    {
        return EnumMaps::_joystickEnumMap[constant];
    }

    template<>
    Input::Gamepad::Button getEnum(int constant)
    {
        return EnumMaps::_gamepadButtonEnumMap[constant];
    }

    template<>
    Input::Gamepad::Axis getEnum(int constant)
    {
        return EnumMaps::_gamepadAxisEnumMap[constant];
    }

    template<>
    OpenGLProfile getEnum(int constant)
    {
        return EnumMaps::_glProfileEnumMap[constant];
    }

    template<>
    int getValue(Input::Mode::Target literal)
    {
        return ValueMaps::_inputModeTargetValueMap[literal];
    }


    template<>
    int getValue(Input::Mode::Value literal)
    {
        return ValueMaps::_inputModeValueValueMap[literal];
    }


    template<>
    int getValue(Input::Action literal)
    {
        return ValueMaps::_actionValueMap[literal];
    }


    template<>
    int getValue(Input::Key literal)
    {
        return ValueMaps::_keyValueMap[literal];
    }


    template<>
    int getValue(Input::Modifier literal)
    {
        return ValueMaps::_keyModifierValueMap[literal];
    }


    template<>
    int getValue(Input::MouseButton literal)
    {
        return ValueMaps::_mouseButtonValueMap[literal];
    }


    template<>
    int getValue(Input::Joystick literal)
    {
        return ValueMaps::_joystickValueMap[literal];
    }


    template<>
    int getValue(Input::Gamepad::Button literal)
    {
        return ValueMaps::_gamepadButtonValueMap[literal];
    }


    template<>
    int getValue(Input::Gamepad::Axis literal)
    {
        return ValueMaps::_gamepadAxisValueMap[literal];
    }


    template<>
    int getValue(OpenGLProfile literal)
    {
        return ValueMaps::_glProfileValueMap[literal];
    }
}// namespace GLFW3Adapter
}// namespace Window
