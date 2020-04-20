#ifndef WINDOW__GLFW3_ADAPTER_HPP
#define WINDOW__GLFW3_ADAPTER_HPP

#include <unordered_map>

#include "../tools/enum_map.hpp"
#include "enums.hpp"

namespace Window
{
namespace GLFW3Adapter
{
    // Mappings from RenderBoi enums to GLFW3 macros
    namespace ValueMaps
    {
        extern std::unordered_map<Input::Mode::Target, unsigned int> _inputModeTargetValueMap;
        extern std::unordered_map<Input::Mode::Value, unsigned int> _inputModeValueValueMap;
        extern std::unordered_map<Input::Action, unsigned int> _actionValueMap;
        extern std::unordered_map<Input::Key, unsigned int> _keyValueMap;
        extern std::unordered_map<Input::Modifier, unsigned int> _keyModifierValueMap;
        extern std::unordered_map<Input::MouseButton, unsigned int> _mouseButtonValueMap;
        extern std::unordered_map<Input::Joystick, unsigned int> _joystickValueMap;
        extern std::unordered_map<Input::Gamepad::Button, unsigned int> _gamepadButtonValueMap;
        extern std::unordered_map<Input::Gamepad::Axis, unsigned int> _gamepadAxisValueMap;
        extern std::unordered_map<OpenGLProfile, unsigned int> _glProfileValueMap;

        // Whether maps were populated
        extern bool _mapsPopulated;
        // Populate maps
        void populateMaps();
    }

    // Mappings from GLFW3 macros to RenderBoi enums
    namespace EnumMaps
    {
        extern std::unordered_map<unsigned int, Input::Mode::Target> _inputModeTargetEnumMap;
        extern std::unordered_map<unsigned int, Input::Mode::Value> _inputModeValueEnumMap;
        extern std::unordered_map<unsigned int, Input::Action> _actionEnumMap;
        extern std::unordered_map<unsigned int, Input::Key> _keyEnumMap;
        extern std::unordered_map<unsigned int, Input::Modifier> _keyModifierEnumMap;
        extern std::unordered_map<unsigned int, Input::MouseButton> _mouseButtonEnumMap;
        extern std::unordered_map<unsigned int, Input::Joystick> _joystickEnumMap;
        extern std::unordered_map<unsigned int, Input::Gamepad::Button> _gamepadButtonEnumMap;
        extern std::unordered_map<unsigned int, Input::Gamepad::Axis> _gamepadAxisEnumMap;
        extern std::unordered_map<unsigned int, OpenGLProfile> _glProfileEnumMap;

        // Whether maps were populated
        extern bool _mapsPopulated;
        // Populate maps
        void populateMaps();
    }

    // Populate all maps if not done already
    void initialize();

    // Get the RenderBoi enum corresponding to the given GLFW3 macro
    template<typename Enum>
    Enum getEnum(int constant);

    template<>
    Input::Mode::Target getEnum(int litteral);

    template<>
    Input::Mode::Value getEnum(int litteral);

    template<>
    Input::Action getEnum(int litteral);

    template<>
    Input::Key getEnum(int litteral);

    template<>
    Input::Modifier getEnum(int litteral);

    template<>
    Input::MouseButton getEnum(int litteral);

    template<>
    Input::Joystick getEnum(int litteral);

    template<>
    Input::Gamepad::Button getEnum(int litteral);

    template<>
    Input::Gamepad::Axis getEnum(int litteral);

    template<>
    OpenGLProfile getEnum(int litteral);

    // Get the GLFW3 macro corresponding to the given RenderBoi enum
    template<typename Enum>
    int getValue(Enum litteral);

    template<>
    int getValue(Input::Mode::Target litteral);

    template<>
    int getValue(Input::Mode::Value litteral);

    template<>
    int getValue(Input::Action litteral);

    template<>
    int getValue(Input::Key litteral);

    template<>
    int getValue(Input::Modifier litteral);

    template<>
    int getValue(Input::MouseButton litteral);

    template<>
    int getValue(Input::Joystick litteral);

    template<>
    int getValue(Input::Gamepad::Button litteral);

    template<>
    int getValue(Input::Gamepad::Axis litteral);

    template<>
    int getValue(OpenGLProfile litteral);
}
}//namespace Window

#endif//WINDOW__GLFW3_ADAPTER_HPP