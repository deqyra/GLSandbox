#ifndef RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP
#define RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP

#include "../gl_window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

namespace Renderboi
{

// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLFW3Window : public GLWindow
{
    private:
        /// @brief Pointer to the managed GLFW window.
        GLFWwindow* _w;

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

        /// @brief Set the input mode of a certain target in the window.
        ///
        /// @param target Literal describing which aspect of the window whose
        /// input mode should be set.
        /// @param value Literal describing which input to set the target to.
        void setInputMode(Window::Input::Mode::Target target, Window::Input::Mode::Value value) override;

        /// @brief Set the title of the window.
        ///
        /// @return The title of the window.
        void setTitle(std::string title) override;
        
        /// @brief Get the aspect ratio of the framebuffer used by the window.
        ///
        /// @return The aspect ratio of the framebuffer used by the window.
        float getAspectRatio() const override;

        /// @brief Get position of the mouse cursor in the window.
        ///
        /// @param x [Output parameter] The X coordinate of the cursor.
        /// @param y [Output parameter] The Y coordinate of the cursor.
        void getCursorPos(double& x, double& y) const override;
};

using GLFW3WindowPtr = std::shared_ptr<GLFW3Window>;

}//namespace Renderboi

#endif//RENDERBOI__WINDOW__GLFW3__GLFW3_WINDOW_HPP
