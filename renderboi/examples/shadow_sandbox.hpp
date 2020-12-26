#ifndef RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP

#include <glm/glm.hpp>

#include "gl_sandbox.hpp"

#include <cpptools/sine_generator.hpp>

#include <renderboi/core/camera.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/scene/scene_object.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

class ShadowSandbox : public GLSandbox
{
    public:
        static constexpr float LightBaseRange = 10.f;
        static constexpr Camera::CameraParameters CameraParams = {-45.f, -35.f, 1.f};
        static constexpr glm::vec3 StartingCameraPosition = {15.f, 15.f, 15.f};
        static constexpr unsigned int PlaneTileCount = 10;
        static constexpr float PlaneTileSize = 1.f;
        static constexpr float WallSize = PlaneTileCount * PlaneTileSize;
        static constexpr glm::vec3 LightPosition = {WallSize / 2.f, WallSize / 2.f, WallSize / 2.f};

        // Run the scene in the provided GL window
        virtual void run(GLWindowPtr window);
};

class ShadowSandboxScript : public Script, public InputProcessor
{
    private:
        /// @brief Pointer to the torus object to animate over time.
        SceneObjectPtr _torusObj;

        /// @brief Pointer to the light object to animate over time.
        SceneObjectPtr _lightObj;

        /// @brief Starting position of the light object.
        glm::vec3 _lightStartingPos;

        /// @brief Whether objects should move.
        bool _pause;

        /// @brief How fast objects should move.
        float _speedFactor;

        /// @brief Sine along which to have the light position vary.
        SineGenerator<float> _sine;

    public:
        static constexpr glm::vec3 LightMovementAxis = {0.f, 1.f, 0.f};
        static constexpr float LightMovementAmplitude = 8.f;
        static constexpr float LightMovementFrequency = 1.f;
        static constexpr glm::vec3 TorusRotationAxis = {0.f, 1.f, 0.f};

        ShadowSandboxScript(SceneObjectPtr lightObj, SceneObjectPtr torusObj);

        /// @brief Update the transforms of the different scene objects.
        virtual void update(float timeElapsed);
        
        /// @brief Process keyboard input.
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        virtual ShadowSandboxScript* clone();
};

#endif//RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP