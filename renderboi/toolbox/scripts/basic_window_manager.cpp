#include "basic_window_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

#include <cpptools/enum_map.hpp>

BasicWindowManager::BasicWindowManager()
{

}

void BasicWindowManager::triggerAction(GLWindowPtr window, const BasicWindowManagerAction& action)
{
    switch (action)
    {
        case BasicWindowManagerAction::Terminate:
            window->setShouldClose(true);
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
    }
}

void BasicWindowManager::stopAction(GLWindowPtr window, const BasicWindowManagerAction& action)
{

}

void BasicWindowManager::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    glViewport(0, 0, width, height);
}

ControlSchemeManagerPtr<BasicWindowManagerAction> BasicWindowManager::getDefaultControlScheme()
{
    using Window::Input::Key;
    ControlSchemeManagerPtr<BasicWindowManagerAction> schemeManager = std::make_shared<ControlSchemeManager<BasicWindowManagerAction>>();

    schemeManager->bindControl(Control(Key::Escape), BasicWindowManagerAction::Terminate);
    schemeManager->bindControl(Control(Key::Space), BasicWindowManagerAction::Terminate);
    schemeManager->bindControl(Control(Key::F1), BasicWindowManagerAction::PolygonFill);
    schemeManager->bindControl(Control(Key::F2), BasicWindowManagerAction::PolygonLine);
    schemeManager->bindControl(Control(Key::F3), BasicWindowManagerAction::PolygonPoint);

    return schemeManager;
}

namespace std
{
    string to_string(const BasicWindowManagerAction& action)
    {
        static bool runOnce = false;
        static unordered_enum_map<BasicWindowManagerAction, std::string> enumNames;

        if (!runOnce)
        {
            enumNames[BasicWindowManagerAction::Terminate]      = "Terminate";
            enumNames[BasicWindowManagerAction::PolygonFill]    = "PolygonFill";
            enumNames[BasicWindowManagerAction::PolygonLine]    = "PolygonLine";
            enumNames[BasicWindowManagerAction::PolygonPoint]   = "PolygonPoint";

            runOnce = true;
        }

        auto it = enumNames.find(action);
        if (it != enumNames.end()) return it->second;

        return "Unknown";
    }
}
