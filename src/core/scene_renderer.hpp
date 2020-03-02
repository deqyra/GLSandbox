#ifndef SCENE_RENDERER_HPP
#define SCENE_RENDERER_HPP

#include <vector>
#include <memory>

#include "scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/scene_object_component.hpp"

#include "view_provider.hpp"

#include "light.hpp"
#include "ubo/matrix_ubo.hpp"
#include "ubo/light_ubo.hpp"

#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"

class SceneRenderer
{
    public:
        using ObjPtr = Scene::ObjPtr;
        using WeakObjPtr = Scene::WeakObjPtr;

    private:
        MatrixUBO _matrixUbo;
        LightUBO _lightUbo;

        void sendLightData(std::vector<LightPtr>& lights, std::vector<glm::mat4>& modelMats, glm::mat4 view);
        void drawMesh(MeshPtr mesh, glm::mat4 model, glm::mat4 view, Material mat, Shader shader);

    public:
        SceneRenderer();
        void renderScene(std::weak_ptr<Scene> wScene);
};

#endif//SCENE_RENDERER_HPP