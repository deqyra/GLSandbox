#include "gamepad_input_processor.hpp"

namespace Renderboi
{

unsigned int GamepadInputProcessor::_count = 0;

const std::vector<GamepadInputProcessor::EventType> GamepadInputProcessor::EventTypeList = {
    GamepadInputProcessor::EventType::Connect,
    GamepadInputProcessor::EventType::Disconnect,
    GamepadInputProcessor::EventType::Axis,
    GamepadInputProcessor::EventType::Button
};

GamepadInputProcessor::GamepadInputProcessor() :
    id(_count++)
{

}

void GamepadInputProcessor::processConnected(const Joystick slot)
{

}

void GamepadInputProcessor::processDisconnected(const Joystick slot)
{

}

void GamepadInputProcessor::processButton(const Joystick slot, const Window::Input::Gamepad::Button button, const Window::Input::Action action)
{

}

void GamepadInputProcessor::processAxis(const Joystick slot, const Window::Input::Gamepad::Axis axis, const float value)
{

}

}//namespace Renderboi