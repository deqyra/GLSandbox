/**
    GLTest, lighting_cube.hpp
    Purpose: Define an example class to display a textured cube with some lighting.

    @author François Brachais (deqyra)
    @version 1.0 06/02/2020
 */
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

class LightingSandbox : public GLSandbox
{
    private:
        std::shared_ptr<SceneObject> generateSceneMesh(std::shared_ptr<Scene> scene, std::shared_ptr<MeshGenerator> generator, Material mat = Material(), Shader shader = Shader());

    public:
        LightingSandbox();
        virtual ~LightingSandbox();
        virtual void run(GLFWwindow* window);
};

class LightingSandboxScript : public InputProcessingScript
{
    private:
        SceneObjectPtr _cubeObj;
        SceneObjectPtr _torusObj;
        bool _autoRotate;
        float _speedFactor;

    public:
        LightingSandboxScript(SceneObjectPtr cubeObj, SceneObjectPtr torusObj);

        virtual void update(float timeElapsed);
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif//LIGHTING_EXAMPLE_HPP