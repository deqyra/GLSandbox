#ifndef WINDOWING__GL_WINDOW_HPP
#define WINDOWING__GL_WINDOW_HPP

#include <memory>
#include <sstream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <../include/GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../core/input_processor.hpp"
#include "../tools/enum_map.hpp"

// To be attached to a GLFWwindow object, providing event callback functionality through a custom InputProcessor
class GLWindow
{
    public:
        enum class InputModeTarget;
        enum class InputModeValue;

        GLWindow(GLFWwindow* window, std::string title);
        ~GLWindow();

        // Get a pointer to the managed window
        GLFWwindow* getWindow();

        // To be called upon a framebuffer resize event; forwards the call to the custom input processor
        void processFramebufferResize(GLFWwindow* window, int width, int height);
        // To be called upon a keyboard event; forwards the call to the custom input processor
        void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // To be called upon a mouse button event; forwards the call to the custom input processor
        void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // To be called upon a mouse cursor event; forwards the call to the custom input processor
        void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

        // Register new input processor
        void registerInputProcessor(InputProcessorPtr inputProcessor);
        // Discard custom input processor
        void detachInputProcessor();

        // Whether the window was flagged for closing
        bool shouldClose();
        // Set the window closing flaf
        void setShouldClose(bool value);

        // Swap GLFW buffers
        void swapBuffers();
        // Poll GLFW events
        void pollEvents();

        // Set GLFW input mode
        void setInputMode(InputModeTarget target, InputModeValue value);

        // Get title of managed window
        std::string getTitle();
        // Get title of managed window
        void setTitle(std::string title);

    private:
        // The GLFW window being managed
        GLFWwindow* _w;
        // Custom input processor to be registered at any time, providing custom callbacks
        InputProcessorPtr _inputProcessor;
        // Window title
        std::string _title;

        // Maps from enum values to their corresponding unsigned int values in GLFW
        static std::enum_map<InputModeTarget, unsigned int> _enumTargets;
        static std::enum_map<InputModeValue, unsigned int> _enumValues;

        // Whether the enum maps contain data
        static bool _enumMapsPopulated;
        // Populate the enum maps
        static void populateEnumMaps();

    public:
        enum class InputModeTarget
        {
            Cursor,
            StickyKeys,
            StickyMouseButtons,
            LockKeyMods,
            RawMouseMotion
        };

        enum class InputModeValue
        {
            True,
            False,
            NormalCursor,
            HiddenCursor,
            DisabledCursor
        };
};

using GLWindowPtr = std::shared_ptr<GLWindow>;

#endif//WINDOWING__GL_WINDOW_HPP
