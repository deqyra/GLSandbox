#include "basic_input_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

BasicInputManager::BasicInputManager()
{

}

void BasicInputManager::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void BasicInputManager::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    if ((key == Key::Escape || key == Key::Space) && action == Action::Press)
    {
        window->setShouldClose(true);
    }
    else if (key == Key::Key1 && action == Action::Press)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      // Normal draw mode
    }
    else if (key == Key::Key2 && action == Action::Press)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Wireframe draw mode
    }
    else if (key == Key::Key3 && action == Action::Press)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);     // Vertices only draw mode
    }

    // cout << "Key event: " << glfwKeyName(key) << " " << glfwActionName(action) << endl;
}

void BasicInputManager::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    double xpos, ypos;
    window->getCursorPos(&xpos, &ypos);

    std::cout << "x: " << xpos << "; y: " << ypos << "; event: mouse " << glfwButtonName(button) << " " << glfwActionName(action) << std::endl;
}

void BasicInputManager::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{

}

BasicInputManager* BasicInputManager::clone()
{
    return new BasicInputManager();
}

std::string glfwActionName(Window::Input::Action action)
{
    return "???";
}

std::string glfwKeyName(Window::Input::Key key)
{
    return "???";
}

std::string glfwButtonName(Window::Input::MouseButton button)
{
    return "???";
}
