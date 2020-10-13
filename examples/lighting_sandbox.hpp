#ifndef EXAMPLES__LIGHTING_EXAMPLE_HPP
#define EXAMPLES__LIGHTING_EXAMPLE_HPP

#include <string>

#include "gl_sandbox.hpp"

#include "../window/gl_window.hpp"
#include "../tools/sine_generator.hpp"

#include "../core/factory.hpp"
#include "../core/input_processor.hpp"
#include "../core/shader.hpp"
#include "../core/texture_2d.hpp"

#include "../core/mesh_generator.hpp"
#include "../core/mesh_generators/mesh_type.hpp"
#include "../core/mesh_generators/type_to_gen_mapping.hpp"

#include "../core/scene/scene.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/input_processing_script.hpp"

// Display lit moving objects
class LightingSandbox : public GLSandbox
{
    public:
        static constexpr float LightBaseRange = 30.f;
        static constexpr float CameraYaw = -90.f;
        static constexpr float CameraPitch = 0.f;
        
        //static constexpr glm::vec3 StartingCameraPosition = {5.f, 6.f, 5.f};
        static constexpr glm::vec3 StartingCameraPosition = {0.f, 0.f, 6.f};
        static constexpr glm::vec3 StartingLightPosition = {-3.f, 3.f, 0.f};

        LightingSandbox();
        virtual ~LightingSandbox();

        // Run the scene in the provided GLFW window
        virtual void run(GLWindowPtr window);
};

// Handle object movement in the scene displayed by LightingSandbox
class LightingSandboxScript : public InputProcessingScript
{
    private:
        // Pointer to the cube of the LightingSandbox
        SceneObjectPtr _cubeObj;
        // Pointer to the big torus of the LightingSandbox
        SceneObjectPtr _bigTorusObj;
        // Pointer to the small torus of the LightingSandbox
        SceneObjectPtr _smallTorusObj;
        // Pointer to the tetrahedron of the LightingSandbox
        SceneObjectPtr _tetrahedronObj;
        // Pointer to the camera of the LightingSandbox
        SceneObjectPtr _cameraObj;
        // Pointer to the light whose range to vary
        std::shared_ptr<PointLight> _light;
        // Whether objects should move
        bool _autoRotate;
        // How fast objects should move
        float _speedFactor;
        // Sine along which to vary the light range
        SineGenerator<float> _sine;
        // Base range of the light
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

        // Update the transforms of the different scene objects
        virtual void update(float timeElapsed);
        // Process keyboard input
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        virtual LightingSandboxScript* clone();
};

#endif//EXAMPLES__LIGHTING_EXAMPLE_HPP