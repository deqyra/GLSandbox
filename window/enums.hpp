#ifndef WINDOW__ENUMS_HPP
#define WINDOW__ENUMS_HPP

#include <unordered_map>

namespace Window
{
namespace Input
{    
    namespace Mode
    {
        enum class Target
        {
            Cursor,
            StickyKeys,
            StickyMouseButtons,
            LockKeyMods,
            RawMouseMotion
        };

        enum class Value
        {
            True,
            False,
            NormalCursor,
            HiddenCursor,
            DisabledCursor
        };
    }

    enum class Action
    {
        Release,
        Press,
        Repeat
    };

    enum class Key
    {
        Unknown,
        Space,
        Apostrophe,     /* ' */
        Comma,          /* , */
        Minus,          /* - */
        Period,         /* . */
        Slash,          /* / */
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        Semicolon,      /* ; */
        Equal,          /* = */
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,    /* [ */
        Backslash,      /* \ */
        RightBracket,   /* ] */
        GraveAccent,    /* ` */
        World1,         /* non-US #1 */
        World2,         /* non-US #2 */
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu
    };

    enum class Modifier : unsigned int
    {
        None        = 0x0000,
        Shift       = 0x0001,
        Control     = 0x0002,
        Alt         = 0x0004,
        Super       = 0x0008,
        CapsLock    = 0x0010,
        NumLock     = 0x0020
    };

    enum class MouseButton
    {
        B1,
        B2,
        B3,
        B4,
        B5,
        B6,
        B7,
        B8,
        Last = B8,
        Left = B1,
        Right = B2,
        Middle = B3
    };

    enum class Joystick
    {
        J1,
        J2,
        J3,
        J4,
        J5,
        J6,
        J7,
        J8,
        J9,
        J10,
        J11,
        J12,
        J13,
        J14,
        J15,
        J16
    };

    namespace Gamepad
    {
        enum class Button
        {
            A,
            B,
            X,
            Y,
            LeftBumper,
            RightBumper,
            Back,
            Start,
            Guide,
            LeftThumb,
            RightThumb,
            DPadUp,
            DPadRight,
            DPadDown,
            DPadLeft,
            Cross = A,
            Circle = B,
            Square = X,
            Triangle = Y
        };

        enum class Axis
        {
            LeftX,
            LeftY,
            RightX,
            RightY,
            LeftTrigger,
            RightTrigger
        };
    }
}

enum class OpenGLProfile
{
    Core,
    Compatibility,
    Any
};
}//namespace Window

unsigned int operator&(unsigned int left, Window::Input::Modifier right);
unsigned int operator|(unsigned int left, Window::Input::Modifier right);
bool any(Window::Input::Modifier value);

#endif//WINDOW__ENUMS_HPP