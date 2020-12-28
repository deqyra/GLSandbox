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

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from GLSandbox ///
        ///                                   ///
        /////////////////////////////////////////

        /// @brief Run something in the provided GL window.
		///
		/// @param window Pointer to the window to run stuff in.
        void run(const GLWindowPtr window) override;
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

        //////////////////////////////////////
        ///                                ///
        /// Methods overridden from Script ///
        ///                                ///
        //////////////////////////////////////

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        void update(float timeElapsed) override;

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        ShadowSandboxScript* clone() const override;

        //////////////////////////////////////////////
        ///                                        ///
        /// Methods overridden from InputProcessor ///
        ///                                        ///
        //////////////////////////////////////////////

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        void processKeyboard(
            const GLWindowPtr window,
            const Window::Input::Key key,
            const int scancode,
            const Window::Input::Action action,
            const int mods
        ) override;

};

#endif//RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP