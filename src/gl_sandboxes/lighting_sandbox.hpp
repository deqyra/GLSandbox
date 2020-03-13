#ifndef LIGHTING_EXAMPLE_HPP
#define LIGHTING_EXAMPLE_HPP

#include <string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"


#include "../gl_sandbox.hpp"
#include "../core/shader.hpp"
#include "../core/texture_2d.hpp"
#include "../core/mesh_generator.hpp"
#include "../core/scene/scene_object.hpp"
#include "../core/scene/input_processing_script.hpp"
#include "../tools/input_processor.hpp"

// Display lit moving objects
class LightingSandbox : public GLSandbox
{
    private:
        // Generate a mesh using the given generator and attach it to the given scene. The generated mesh will be rendered using the given material and shader.
        std::shared_ptr<SceneObject> generateSceneMesh(std::shared_ptr<Scene> scene, std::shared_ptr<MeshGenerator> generator, Material mat = Material(), Shader shader = Shader());

    public:
        LightingSandbox();
        virtual ~LightingSandbox();

        // Run the scene in the provided GLFW window
        virtual void run(GLFWwindow* window);
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
        // Whether objects should move
        bool _autoRotate;
        // How fast objects should move
        float _speedFactor;

    public:
        LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr bigTorusObj, SceneObjectPtr smallTorusObj, SceneObjectPtr tetrahedronObj);

        // Update the transforms of the different scene objects
        virtual void update(float timeElapsed);
        // Process keyboard input
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif//LIGHTING_EXAMPLE_HPP