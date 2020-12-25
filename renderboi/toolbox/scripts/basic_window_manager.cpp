#include "basic_window_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

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
