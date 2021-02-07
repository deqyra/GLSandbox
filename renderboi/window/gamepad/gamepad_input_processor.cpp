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

void GamepadInputProcessor::processConnected(const GamepadPtr gamepad)
{

}

void GamepadInputProcessor::processDisconnected(const GamepadPtr gamepad)
{

}

void GamepadInputProcessor::processButton(const GamepadPtr gamepad, const Window::Input::Gamepad::Button button, const Window::Input::Action action)
{

}

void GamepadInputProcessor::processAxis(const GamepadPtr gamepad, const Window::Input::Gamepad::Axis axis, const float value)
{

}

}//namespace Renderboi