#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../window_backend.hpp"
#include "../gl_window.hpp"

namespace Renderboi::Window
{

template<WindowBackend W>
class WindowFactory;

// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLFW3Window : public GLWindow
{
private:
    using GLFW3WindowFactory = WindowFactory<WindowBackend::GLFW3>;
    friend class GLFW3WindowFactory;

    /// @brief Pointer to the managed GLFW window.
    GLFWwindow* _w;

    /// @brief Pointer to the monitor on which the window is fullscreen, or nullptr.
    GLFWmonitor* _fullscreenMonitor;

    /// @brief Width of the window before fullscreen was requested.
    int _widthBeforeFullscreen;

    /// @brief Height of the window before fullscreen was requested.
    int _heightBeforeFullscreen;

    /// @brief X position of the window before fullscreen was requested.
    int _xPosBeforeFullscreen;

    /// @brief Y position of the window before fullscreen was requested.
    int _yPosBeforeFullscreen;

public:
    /// @param window Raw pointer to the GLFW window struct.
    /// @param title Title to give the window.
    GLFW3Window(GLFWwindow* window, std::string title);
    ~GLFW3Window();

    /// @brief Get a raw pointer to the managed window.
    ///
    /// @return A raw pointer to the managed window.
    GLFWwindow* getWindow() const;

    ////////////////////////////////////////
    ///                                  ///
    /// Methods overridden from GLWindow ///
    ///                                  ///
    ////////////////////////////////////////

    /// @brief Set the title of the window.
    ///
    /// @return The title of the window.
    void setTitle(std::string title) override;

    /// @brief Set the input mode of a certain target in the window.
    ///
    /// @param target Literal describing which aspect of the window whose
    /// input mode should be set.
    /// @param value Literal describing which input to set the target to.
    void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value) override;

    /// @brief Hide the window. May be called only from the main thread.
    void hide() override;

    /// @brief Show the window. May be called only from the main thread.
    void show() override;

    /// @brief Whether the window is visible. May be called only from the main 
    /// thread.
    ///
    /// @return Whether the window is visible.
    bool isVisible() const override;

    /// @brief Bring focus to the window. May be called only from the main 
    /// thread.
    void focus() override;

    /// @brief Whether the window is visible. May be called only from the main 
    /// thread.
    ///
    /// @return Whether the window is visible.
    bool isFocused() const override;

    /// @brief Maximize the window. May be called only from the main thread.
    void maximize() override;

    /// @brief Whether or not the window is maximized. May be called only from 
    /// the main thread.
    ///
    /// @return Whether or not the window is maximized.
    bool isMaximized() const override;

    /// @brief Minimize the window. May be called only from the main thread.
    void minimize() override;

    /// @brief Whether or not the window is minimized. May be called only from 
    /// the main thread.
    ///
    /// @return Whether or not the window is minimized.
    bool isMinimized() const override;

    /// @brief Retrieve the width and height of the window in screen coordinates.
    ///
    /// @param[out] width Will receive the width of the window.
    /// @param[out] height Will receive the height of the window.
    void getSize(int& width, int& height) const override;

    /// @brief Retrieve the width and height of the framebuffer in pixels.
    ///
    /// @param[out] width Will receive the width of the framebuffer.
    /// @param[out] height Will receive the height of the framebuffer.
    void getFramebufferSize(int& width, int& height) const override;

    /// @brief Display the window in fullscreen on the specified monitor. The
    /// primary monitor will be used if none is provided. May be called only
    /// from the main thread.
    ///
    /// @param monitor Pointer to the monitor on which to make the window go
    /// fullscreen.
    /// @param borderless Whether or not to go borderless fullscreen. When 
    /// enabled, this makes the window fit exactly the video mode of the monitor
    /// it is on. Otherwise, the attributes of the window are left untouched.
    void goFullscreen(MonitorPtr monitor = nullptr, bool borderless = false) override;

    /// @brief Display the window in fullscreen on the specified monitor. The
    /// primary monitor will be used if none is provided. The window will switch
    /// to the video mode which is the closest to the indicated parameters. May
    /// be called only from the main thread.
    ///
    /// @param monitor Pointer to the monitor on which to make the window go
    /// fullscreen.
    /// @param width Desired width of the video mode.
    /// @param height Desired height of the video mode.
    /// @param refreshRate Desired refresh rate.
    void goFullscreen(
        MonitorPtr monitor = nullptr,
        int width = -1,
        int height = -1,
        int refreshRate = -1
    ) override;

    /// @brief Whether or not the window is displayed in fullscreen mode. May be
    /// called only from the main thread.
    ///
    /// @return Whether or not the window is displayed in fullscreen mode.
    bool isFullscreen() const override;

    /// @brief Display the window in windowed mode. May be called only from the
    /// main thread.
    void exitFullscreen() override;

    /// @brief Set the refresh rate of a fullscreen window. Has no effect on a
    /// window displayed in windowed mode. May be called only from the main 
    /// thread.
    void setRefreshRate(int rate) override;

    /// @brief Whether the window was flagged for closing.
    ///
    /// @return Whether or not the window was flagged for closing.
    bool shouldClose() const override;

    /// @brief Set the window closing flag.
    ///
    /// @param value Whether or not the window should be flagged for closing.
    void setShouldClose(bool value) override;

    /// @brief Swap the front and back buffers of the window.
    void swapBuffers() override;

    /// @brief Poll events recorded by the window.
    void pollEvents() const override;
    
    /// @brief Get the aspect ratio of the framebuffer used by the window.
    ///
    /// @return The aspect ratio of the framebuffer used by the window.
    float getAspectRatio() const override;

    /// @brief Get position of the mouse cursor in the window.
    ///
    /// @param x [Output parameter] The X coordinate of the cursor.
    /// @param y [Output parameter] The Y coordinate of the cursor.
    void getCursorPos(double& x, double& y) const override;

    /// @brief Make the GL context current for the calling thread. May be called
    /// from any thread.
    void makeContextCurrent(GLContextClientPtr context) override;

    /// @brief Make the GL context non- current for the calling thread. May be 
    /// called from any thread.
    void releaseContext() override;

    /// @brief Tell whether the GL context supports a certain extension. A GL 
    /// context must be current on the calling thread.
    ///
    /// @param extName String containing the name of the extension to query.
    bool extensionSupported(std::string extName);
};

using GLFW3WindowPtr = std::shared_ptr<GLFW3Window>;

}//namespace Renderboi::Window

#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP
