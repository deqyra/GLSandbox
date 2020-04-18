#ifndef CORE__SCRIPTS__LAMBDA_SCRIPT_HPP
#define CORE__SCRIPTS__LAMBDA_SCRIPT_HPP

#include "../scene/input_processing_script.hpp"

#include <functional>

// Provide event callbacks which can be set on the fly using lambdas
class LambdaScript : public InputProcessingScript
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Scripts are meant to be used only through pointers
        LambdaScript(const LambdaScript& other) = delete;
        LambdaScript& operator=(const LambdaScript& other) = delete;

    public:
        using Update_CB = std::function<void(float)>;
        using FramebufferResize_CB = std::function<void(GLWindowPtr, int, int)>;
        using Keyboard_CB = std::function<void(GLWindowPtr, Window::Input::Key, int, Window::Input::Action, int)>;
        using MouseButton_CB = std::function<void(GLWindowPtr, Window::Input::MouseButton, Window::Input::Action, int)>;
        using MouseCursor_CB = std::function<void(GLWindowPtr, double, double)>;

        LambdaScript();

        // Lambda to call upon a frame update
        Update_CB updateCallback;
        // Lambda to call upon a framebuffer resize event
        FramebufferResize_CB framebufferResizeCallback;
        // Lambda to call upon a keyboard event
        Keyboard_CB keyboardCallback;
        // Lambda to call upon a mouse button event
        MouseButton_CB mouseButtonCallback;
        // Lambda to call upon a mouse cursor event
        MouseCursor_CB mouseCursorCallback;

        // To be called upon a frame update; forwards the call to the custom callback
        virtual void update(float timeElapsed);
        // To be called upon a framebuffer resize event; forwards the call to the custom callback
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);
        // To be called upon a keyboard event; forwards the call to the custom callback
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // To be called upon a mouse button event; forwards the call to the custom callback
        virtual void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // To be called upon a mouse cursor event; forwards the call to the custom callback
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        virtual LambdaScript* clone();
};

#endif//CORE__SCRIPTS__LAMBDA_SCRIPT_HPP