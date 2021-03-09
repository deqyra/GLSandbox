#include "gamepad_manager.hpp"

namespace Renderboi::Window
{

unsigned int GamepadManager::_count = 0;

GamepadManager::GamepadManager() :
    id(_count++)
{

}

void GamepadManager::setGamepadState(GamepadPtr gamepad, GamepadState state)
{
    gamepad->_setNewState(state);
}

void GamepadManager::setGamepadConnected(GamepadPtr gamepad)
{
    gamepad->_processConnected();
}

void GamepadManager::setGamepadDisconnected(GamepadPtr gamepad)
{
    gamepad->_processDisconnected();
}

}//namespace Renderboi::Window