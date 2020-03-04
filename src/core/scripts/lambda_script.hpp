#ifndef LAMBDA_SCRIPT_HPP
#define LAMBDA_SCRIPT_HPP

#include "../scene/input_processing_script.hpp"

#include <functional>

class LambdaScript : public InputProcessingScript
{
    public:
        using Up_Callback = std::function<void(float)>;
        using FB_Callback = std::function<void(GLFWwindow*, int, int)>;
        using KB_Callback = std::function<void(GLFWwindow*, int, int, int, int)>;
        using MB_Callback = std::function<void(GLFWwindow*, int, int, int)>;
        using Cur_Callback = std::function<void(GLFWwindow*, double, double)>;

        LambdaScript();

        Up_Callback updateCallback;
        FB_Callback framebufferResizeCallback;
        KB_Callback keyboardCallback;
        MB_Callback mouseButtonCallback;
        Cur_Callback mouseCursorCallback;

        virtual void update(float timeElapsed);
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);
};

#endif//LAMBDA_SCRIPT_HPP