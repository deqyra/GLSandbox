#ifndef RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP

#include <string>

#include <glm/glm.hpp>

#include "gl_sandbox.hpp"
#include "gl_sandbox_parameters.hpp"

#include <cpptools/math/sine_generator.hpp>

#include <renderboi/core/camera.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/scene_object.hpp>
#include <renderboi/toolbox/scene/scene_renderer.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gamepad/gamepad.hpp>
#include <renderboi/window/gamepad/gamepad_manager.hpp>

namespace Renderboi
{

class ShadowSandbox : public GLSandbox
{
    private:
        /// @brief Used to temporarily store the original title of the window.
        std::string _title;

        /// @brief Pointer to a gamepad instace.
        GamepadPtr _gamepad;
        
        /// @brief Whether or not a gamepad could be retrieved.
        bool _gamepadPresent;

        /// @brief Pointer to the scene.
        ScenePtr _scene;

        /// @brief In charge of rendering the scene.
        SceneRendererPtr _sceneRenderer;

        /// @brief Pointer to the input splitter of the scene.
        InputSplitterPtr _splitter;

    public:
        static constexpr float LightBaseRange = 10.f;
        static constexpr Camera::CameraParameters CameraParams = {-45.f, -35.f, 1.f};
        static constexpr glm::vec3 StartingCameraPosition = {15.f, 15.f, 15.f};
        static constexpr unsigned int PlaneTileCount = 10;
        static constexpr float PlaneTileSize = 1.f;
        static constexpr float WallSize = PlaneTileCount * PlaneTileSize;
        static constexpr glm::vec3 LightPosition = {WallSize / 2.f, WallSize / 2.f, WallSize / 2.f};

		/// @param window Pointer to the window on which the sandbox should run.
        ShadowSandbox(const GLWindowPtr window);

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from GLSandbox ///
        ///                                   ///
        /////////////////////////////////////////

		/// @brief Set up the window prior to running the example. Will be
		/// called from the main thread.
		///
		/// @param window Pointer to the window to initialize.
		virtual void setUp(const GLSandboxParameters& params);

		/// @brief Set up everything needed prior to rendering. Will be called  
		/// from the rendering thread.
		///
		/// @param window Pointer to the window to initialize.
		virtual void renderSetUp(const GLSandboxParameters& params);

        /// @brief Run something in the provided GL window. To be executed by
		/// a separate thread.
		///
		/// @param window Pointer to the window to run stuff in.
		virtual void render(const GLSandboxParameters& params);

		/// @brief Clean up everything that had to be set up for rendering. Will
		/// be called from the rendering thread once run() has returned.
		///
		/// @param window Pointer to the window to detach from.
		virtual void renderTearDown();

		/// @brief Restore the window back to how it was before the example ran.
		/// The contents of this function should be the opposite from those in
		/// setUp(). Will be called from the main thread once run() has returned.
		///
		/// @param window Pointer to the window to detach from.
		virtual void tearDown();
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
        cpptools::SineGenerator<float> _sine;

    public:
        static constexpr glm::vec3 LightMovementAxis = {0.f, 1.f, 0.f};
        static constexpr float LightMovementAmplitude = 8.f;
        static constexpr float LightMovementFrequency = 1.f;
        static constexpr glm::vec3 TorusRotationAxis = {0.f, 1.f, 0.f};

        ShadowSandboxScript(SceneObjectPtr lightObj, SceneObjectPtr torusObj);

        virtual ~ShadowSandboxScript() = default;

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

}//namespace Renderboi

#endif//RENDERBOI__EXAMPLES__SHADOW_SANDBOX_HPP