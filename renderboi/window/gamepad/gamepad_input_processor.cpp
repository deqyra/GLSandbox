#include "gamepad_input_processor.hpp"

namespace Renderboi::Window
{

unsigned int GamepadInputProcessor::_count = 0;

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

}//namespace Renderboi::Window