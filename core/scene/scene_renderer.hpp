#ifndef CORE__SCENE__SCENE_RENDERER_HPP
#define CORE__SCENE__SCENE_RENDERER_HPP

#include <memory>
#include <vector>

#include "scene.hpp"
#include "scene_object.hpp"
#include "component.hpp"

#include "../lights/light.hpp"
#include "../ubo/matrix_ubo.hpp"
#include "../ubo/light_ubo.hpp"

#include "../mesh.hpp"
#include "../material.hpp"
#include "../shader.hpp"

// Render a scene
class SceneRenderer
{
    private:
        // Handle to a UBO for matrices on the GPU
        MatrixUBO _matrixUbo;
        // Handle to a UBO for lights on the GPU
        LightUBO _lightUbo;

        // Send the scene lights to the GPU
        void sendLightData(std::vector<LightPtr>& lights, std::vector<glm::mat4>& modelMats, glm::mat4 view);
        // Issue draw commands for a single mesh
        void drawMesh(MeshPtr mesh, glm::mat4 model, glm::mat4 view, Material mat, Shader shader);

    public:
        SceneRenderer();
        // Render the provided scene
        void renderScene(SceneWPtr wScene);
};

#endif//CORE__SCENE__SCENE_RENDERER_HPP