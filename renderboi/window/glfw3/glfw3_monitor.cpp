#include "glfw3_monitor.hpp"

#include "../window_backend.hpp"

#include <cstring>

namespace Renderboi::Window
{

GLFW3Monitor::GLFW3Monitor(GLFWmonitor* monitor) :
    Monitor(),
    _m(monitor)
{
    glfwSetMonitorUserPointer(_m, (void*)this);
}

GLFW3Monitor::~GLFW3Monitor()
{
    glfwSetMonitorUserPointer(_m, nullptr);
}

GLFWmonitor* GLFW3Monitor::getGlfwMonitorPointer() const
{
    return _m;
}

Monitor::VideoMode GLFW3Monitor::getCurrentVideoMode() const
{
    const GLFWvidmode* mode = glfwGetVideoMode(_m);

    return Monitor::VideoMode{
        mode->width,
        mode->height,
        mode->redBits,
        mode->greenBits,
        mode->blueBits,
        mode->refreshRate
    };
}

std::vector<Monitor::VideoMode> GLFW3Monitor::getVideoModes() const
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(_m, &count);

    std::vector<Monitor::VideoMode> videoModes = std::vector<Monitor::VideoMode>(count);
    for (int i = 0; i < count; i++)
    {
        videoModes.push_back(Monitor::VideoMode{
            modes[i].width,
            modes[i].height,
            modes[i].redBits,
            modes[i].greenBits,
            modes[i].blueBits,
            modes[i].refreshRate
        });
    }

    return videoModes;
}

void GLFW3Monitor::getPhysicalSize(int& width_mm, int& height_mm) const
{
    glfwGetMonitorPhysicalSize(_m, &width_mm, &height_mm);
}

void GLFW3Monitor::getContentScale(float& xscale, float& yscale) const
{
    glfwGetMonitorContentScale(_m, &xscale, &yscale);
}

void GLFW3Monitor::getPosition(int& xpos, int& ypos) const
{
    glfwGetMonitorPos(_m, &xpos, &ypos);
}

void GLFW3Monitor::getWorkArea(int& xpos, int& ypos, int& width, int& height) const
{
    glfwGetMonitorWorkarea(_m, &xpos, &ypos, &width, &height);
}

std::string GLFW3Monitor::getName() const
{
    return std::string(glfwGetMonitorName(_m));
}

Monitor::GammaRamp GLFW3Monitor::getGammaRamp() const
{
    const GLFWgammaramp* ramp = glfwGetGammaRamp(_m);

    std::vector<unsigned short> red = std::vector<unsigned short>(ramp->size);
    std::vector<unsigned short> green = std::vector<unsigned short>(ramp->size);
    std::vector<unsigned short> blue = std::vector<unsigned short>(ramp->size);

    std::memcpy(red.data(), ramp->red, ramp->size);
    std::memcpy(green.data(), ramp->green, ramp->size);
    std::memcpy(blue.data(), ramp->blue, ramp->size);

    return Monitor::GammaRamp(ramp->size, red, green, blue);
}

void GLFW3Monitor::setGammaRamp(const GammaRamp& gammaRamp) const
{
    GLFWgammaramp ramp;
    ramp.size = gammaRamp.getSize();

    // const_cast to avoid a useless copy and because GLFW guarantees not to 
    // modify the values.
    ramp.red = const_cast<unsigned short*>(gammaRamp.getRed().data());
    ramp.green = const_cast<unsigned short*>(gammaRamp.getGreen().data());
    ramp.blue = const_cast<unsigned short*>(gammaRamp.getBlue().data());

    glfwSetGammaRamp(_m, &ramp);
}

Monitor::VideoMode GLFW3Monitor::getLargestVideoMode() const
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(_m, &count);

    int maxWidth = 0;
    int maxHeight = 0;
    int maxRefreshRate = 0;
    int maxColorBits[3] = {0, 0, 0};    
    for (int i = 0; i < count; i++)
    {
        if (modes[i].width > maxWidth)
            maxWidth = modes[i].width;
        if (modes[i].height > maxHeight)
            maxHeight = modes[i].height;
        if (modes[i].refreshRate > maxRefreshRate)
            maxRefreshRate = modes[i].refreshRate;
        if (modes[i].redBits > maxColorBits[0])
            maxColorBits[0] = modes[i].redBits;
        if (modes[i].greenBits > maxColorBits[1])
            maxColorBits[1] = modes[i].greenBits;
        if (modes[i].blueBits > maxColorBits[2])
            maxColorBits[2] = modes[i].blueBits;
    }

    Monitor::VideoMode v = {
        .width = maxWidth,
        .height = maxHeight,
        .redBits = maxColorBits[0],
        .greenBits = maxColorBits[1],
        .blueBits = maxColorBits[2],
        .refreshRate = maxRefreshRate
    };

    return v;
}

}//namespace Renderboi::Window