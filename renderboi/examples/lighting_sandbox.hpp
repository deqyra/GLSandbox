#ifndef RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP
#define RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP

#include <string>
#include <glm/glm.hpp>

#include "gl_sandbox.hpp"

#include <renderboi/core/camera.hpp>
#include <renderboi/core/lights/point_light.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/scene/scene_object.hpp>

#include <cpptools/sine_generator.hpp>

namespace Renderboi
{

/// @brief Example class to display lit moving objects.
class LightingSandbox : public GLSandbox
{
    public:
        static constexpr float LightBaseRange = 30.f;
        static constexpr Camera::CameraParameters CameraParams = {-45.f, -40.f, 1.f};
        static constexpr glm::vec3 StartingCameraPosition = {5.f, 6.f, 5.f};
        static constexpr glm::vec3 StartingLightPosition = {-3.f, 3.f, 0.f};

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

// Handle object movement in the scene displayed by LightingSandbox
class LightingSandboxScript : public InputProcessor, public Script
{
    private:
        /// @brief Pointer to the cube of the LightingSandbox.
        SceneObjectPtr _cubeObj;

        /// @brief Pointer to the big torus of the LightingSandbox.
        SceneObjectPtr _bigTorusObj;

        /// @brief Pointer to the small torus of the LightingSandbox.
        SceneObjectPtr _smallTorusObj;

        /// @brief Pointer to the tetrahedron of the LightingSandbox.
        SceneObjectPtr _tetrahedronObj;

        /// @brief Pointer to the camera of the LightingSandbox.
        SceneObjectPtr _cameraObj;

        /// @brief Pointer to the light whose range to vary.
        std::shared_ptr<PointLight> _light;

        /// @brief Whether objects should move.
        bool _autoRotate;

        /// @brief How fast objects should move.
        float _speedFactor;

        /// @brief Sine along which to vary the light range.
        CppTools::SineGenerator<float> _sine;

        /// @brief Base range of the light.
        float _baseRange;
        
    public:
        static constexpr glm::vec3 CubeOrbitAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 BigTorusRotationAxis = {1.f, 0.f, 0.f};
        static constexpr glm::vec3 SmallTorusRotationAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 TetrahedronRotationAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 TetrahedronOrbitAxis = {0.f, 1.f, 0.f};
        static constexpr float LightVariationAmplitude = 50.f;
        static constexpr float LightVariationFrequency = 1.f;

        LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr bigTorusObj, SceneObjectPtr smallTorusObj, SceneObjectPtr tetrahedronObj, SceneObjectPtr cameraObj, std::shared_ptr<PointLight> light, float baseLightRange);

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
        LightingSandboxScript* clone() const override;

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

#endif//RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP