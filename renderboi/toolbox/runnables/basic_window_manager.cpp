#include "basic_window_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

namespace Renderboi
{

BasicWindowManager::BasicWindowManager()
{

}

void BasicWindowManager::triggerAction(const GLWindowPtr window, const BasicWindowManagerAction& action)
{
    switch (action)
    {
    case BasicWindowManagerAction::Terminate:
        window->signalExit();
        break;

    case BasicWindowManagerAction::PolygonFill:
        _setPolygonMode(window, PolygonMode::Fill);
        break;

    case BasicWindowManagerAction::PolygonLine:
        _setPolygonMode(window, PolygonMode::Line);
        break;

    case BasicWindowManagerAction::PolygonPoint:
        _setPolygonMode(window, PolygonMode::Point);
        break;
        
    case BasicWindowManagerAction::ToggleFullscreen:
        _toggleFullscreen(window);
    }
}

void BasicWindowManager::stopAction(const GLWindowPtr window, const BasicWindowManagerAction& action)
{

}

void BasicWindowManager::processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height)
{
    window->getGlContextClient()->eventManager->queueEvent(Window::GLContextEvent::FitFramebufferToWindow);
}

ControlSchemeManagerPtr<BasicWindowManagerAction> BasicWindowManager::getDefaultControlScheme() const
{
    ControlSchemeManagerPtr<BasicWindowManagerAction>
    schemeManager = std::make_shared<ControlSchemeManager<BasicWindowManagerAction>>();

    using Window::Input::Key;
    schemeManager->bindControl(Control(Key::Escape), BasicWindowManagerAction::Terminate);
    schemeManager->bindControl(Control(Key::Space), BasicWindowManagerAction::Terminate);
    schemeManager->bindControl(Control(Key::F1), BasicWindowManagerAction::PolygonFill);
    schemeManager->bindControl(Control(Key::F2), BasicWindowManagerAction::PolygonLine);
    schemeManager->bindControl(Control(Key::F3), BasicWindowManagerAction::PolygonPoint);
    schemeManager->bindControl(Control(Key::F11), BasicWindowManagerAction::ToggleFullscreen);

    return schemeManager;
}

void BasicWindowManager::_toggleFullscreen(const GLWindowPtr window) const
{
    using Window::GLWindowCriticalEvent;
    
    if (window->isFullscreen())
    {
        window->criticalEventManager.queueEvent(GLWindowCriticalEvent::ExitFullscreen);
    }
    else
    {
        window->criticalEventManager.queueEvent(GLWindowCriticalEvent::GoFullscreen);
    }
}

void BasicWindowManager::_setPolygonMode(const GLWindowPtr window, const PolygonMode mode) const
{
    using Window::GLContextEvent;

    switch (mode)
    {
    case PolygonMode::Fill:
        window->getGlContextClient()->eventManager->queueEvent(GLContextEvent::PolygonModeFill);
        break;

    case PolygonMode::Line:
        window->getGlContextClient()->eventManager->queueEvent(GLContextEvent::PolygonModeLine);
        break;

    case PolygonMode::Point:
        window->getGlContextClient()->eventManager->queueEvent(GLContextEvent::PolygonModePoint);
        break;
    }
}

std::string to_string(const BasicWindowManagerAction& action)
{
    static bool runOnce = false;
    static std::unordered_map<BasicWindowManagerAction, std::string> enumNames;

    if (!runOnce)
    {
        enumNames[BasicWindowManagerAction::Terminate]          = "Terminate";
        enumNames[BasicWindowManagerAction::PolygonFill]        = "PolygonFill";
        enumNames[BasicWindowManagerAction::PolygonLine]        = "PolygonLine";
        enumNames[BasicWindowManagerAction::PolygonPoint]       = "PolygonPoint";
        enumNames[BasicWindowManagerAction::ToggleFullscreen]   = "ToggleFullscreen";

        runOnce = true;
    }

    auto it = enumNames.find(action);
    if (it != enumNames.end()) return it->second;

    return "Unknown";
}

}//namespace Renderboi
