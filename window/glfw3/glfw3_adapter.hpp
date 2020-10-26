#ifndef WINDOW__GLFW3_ADAPTER_HPP
#define WINDOW__GLFW3_ADAPTER_HPP

#include <unordered_map>

#include "../enums.hpp"

namespace Window
{
/// @brief Contains utilities to translate GLFW constants to RenderBoi enums 
/// and the other way around.
namespace GLFW3Adapter
{
    /// @brief Contains mapping data from RenderBoi enums to GLFW3 macros.
    namespace ValueMaps
    {
        extern const std::unordered_map<Input::Mode::Target, unsigned int> InputModeTargetValues;
        extern const std::unordered_map<Input::Mode::Value, unsigned int> InputModeValueValues;
        extern const std::unordered_map<Input::Action, unsigned int> ActionValues;
        extern const std::unordered_map<Input::Key, unsigned int> KeyValues;
        extern const std::unordered_map<Input::Modifier, unsigned int> KeyModifierValues;
        extern const std::unordered_map<Input::MouseButton, unsigned int> MouseButtonValues;
        extern const std::unordered_map<Input::Joystick, unsigned int> JoystickValues;
        extern const std::unordered_map<Input::Gamepad::Button, unsigned int> GamepadButtonValues;
        extern const std::unordered_map<Input::Gamepad::Axis, unsigned int> GamepadAxisValues;
        extern const std::unordered_map<OpenGLProfile, unsigned int> GlProfileValues;

        std::unordered_map<Input::Mode::Target, unsigned int> getInputModeTargetValues();
        std::unordered_map<Input::Mode::Value, unsigned int> getInputModeValueValues();
        std::unordered_map<Input::Action, unsigned int> getActionValues();
        std::unordered_map<Input::Key, unsigned int> getKeyValues();
        std::unordered_map<Input::Modifier, unsigned int> getKeyModifierValues();
        std::unordered_map<Input::MouseButton, unsigned int> getMouseButtonValues();
        std::unordered_map<Input::Joystick, unsigned int> getJoystickValues();
        std::unordered_map<Input::Gamepad::Button, unsigned int> getGamepadButtonValues();
        std::unordered_map<Input::Gamepad::Axis, unsigned int> getGamepadAxisValues();
        std::unordered_map<OpenGLProfile, unsigned int> getGlProfileValues();
    }

    /// @brief Contains mapping data from GLFW3 macros to RenderBoi enums.
    namespace EnumMaps
    {
        extern const std::unordered_map<unsigned int, Input::Mode::Target> InputModeTargetEnums;
        extern const std::unordered_map<unsigned int, Input::Mode::Value> InputModeValueEnums;
        extern const std::unordered_map<unsigned int, Input::Action> ActionEnums;
        extern const std::unordered_map<unsigned int, Input::Key> KeyEnums;
        extern const std::unordered_map<unsigned int, Input::Modifier> KeyModifierEnums;
        extern const std::unordered_map<unsigned int, Input::MouseButton> MouseButtonEnums;
        extern const std::unordered_map<unsigned int, Input::Joystick> JoystickEnums;
        extern const std::unordered_map<unsigned int, Input::Gamepad::Button> GamepadButtonEnums;
        extern const std::unordered_map<unsigned int, Input::Gamepad::Axis> GamepadAxisEnums;
        extern const std::unordered_map<unsigned int, OpenGLProfile> GlProfileEnums;

        std::unordered_map<unsigned int, Input::Mode::Target> getInputModeTargetEnums();
        std::unordered_map<unsigned int, Input::Mode::Value> getInputModeValueEnums();
        std::unordered_map<unsigned int, Input::Action> getActionEnums();
        std::unordered_map<unsigned int, Input::Key> getKeyEnums();
        std::unordered_map<unsigned int, Input::Modifier> getKeyModifierEnums();
        std::unordered_map<unsigned int, Input::MouseButton> getMouseButtonEnums();
        std::unordered_map<unsigned int, Input::Joystick> getJoystickEnums();
        std::unordered_map<unsigned int, Input::Gamepad::Button> getGamepadButtonEnums();
        std::unordered_map<unsigned int, Input::Gamepad::Axis> getGamepadAxisEnums();
        std::unordered_map<unsigned int, OpenGLProfile> getGlProfileEnums();
    }

    /// @brief Get the RenderBoi enum corresponding to the given GLFW3 macro.
    ///
    /// @tparam E Type of the literal to find.
    ///
    /// @param constant GLFW3 macro to translate into a RenderBoi literal.
    ///
    /// @return A literal of type E which describes the provided GLFW3 macro.
    template<typename E>
    E getEnum(int constant);

    template<>
    Input::Mode::Target getEnum(int macro);

    template<>
    Input::Mode::Value getEnum(int macro);

    template<>
    Input::Action getEnum(int macro);

    template<>
    Input::Key getEnum(int macro);

    template<>
    Input::Modifier getEnum(int macro);

    template<>
    Input::MouseButton getEnum(int macro);

    template<>
    Input::Joystick getEnum(int macro);

    template<>
    Input::Gamepad::Button getEnum(int macro);

    template<>
    Input::Gamepad::Axis getEnum(int macro);

    template<>
    OpenGLProfile getEnum(int macro);

    /// @brief Get the GLFW3 macro corresponding to the given RenderBoi enum.
    ///
    /// @tparam E Type of the literal to translate.
    ///
    /// @param literal The literal to translate.
    ///
    /// @return The GLFW3 macro which corresponds to the given literal.
    template<typename E>
    int getValue(E literal);

    template<>
    int getValue(Input::Mode::Target literal);

    template<>
    int getValue(Input::Mode::Value literal);

    template<>
    int getValue(Input::Action literal);

    template<>
    int getValue(Input::Key literal);

    template<>
    int getValue(Input::Modifier literal);

    template<>
    int getValue(Input::MouseButton literal);

    template<>
    int getValue(Input::Joystick literal);

    template<>
    int getValue(Input::Gamepad::Button literal);

    template<>
    int getValue(Input::Gamepad::Axis literal);

    template<>
    int getValue(OpenGLProfile literal);
}
}//namespace Window

#endif//WINDOW__GLFW3_ADAPTER_HPP