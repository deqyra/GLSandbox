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
            window->setShouldClose(true);
            window->signalExit();
            break;
        case BasicWindowManagerAction::PolygonFill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case BasicWindowManagerAction::PolygonLine:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case BasicWindowManagerAction::PolygonPoint:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
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
    glViewport(0, 0, width, height);
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
        window->criticalEventManager.queueCriticalEvent(GLWindowCriticalEvent::GoFullscreen);
    }
    else
    {
        window->criticalEventManager.queueCriticalEvent(GLWindowCriticalEvent::ExitFullscreen);
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
