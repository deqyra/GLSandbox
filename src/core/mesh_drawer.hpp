/**
    GLTest, mesh_drawer.hpp
    Purpose: Define a class to draw any mesh.

    @author François Brachais (deqyra)
    @version 1.0 07/02/2020
 */
#ifndef MESH_DRAWER_HPP
#define MESH_DRAWER_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "view_provider.hpp"
#include "light_type.hpp"
#include "light.hpp"
#include "lights/point_light.hpp"
#include "ubo/matrix_ubo.hpp"
#include "ubo/light_ubo.hpp"

#define MAX_POINT_LIGHTS 128

class MeshDrawer
{
    private:
        std::unordered_map<unsigned int, std::shared_ptr<Mesh>> _meshes;
        std::unordered_map<unsigned int, Shader> _shaders;
        std::unordered_map<unsigned int, bool> _meshesEnabled;

        std::unordered_map<unsigned int, std::shared_ptr<Light>> _lights;
        std::unordered_map<unsigned int, bool> _lightsEnabled;

        std::shared_ptr<ViewProvider> _camera;
        glm::mat4 _projection;

        MatrixUBO _matrixUbo;
        LightUBO _lightUbo;

        void drawMesh(unsigned int id);
        void sendLightData();

    public:
        MeshDrawer();
        MeshDrawer(std::shared_ptr<ViewProvider> camera, glm::mat4 projection);

        void registerMesh(Mesh* mesh, Shader shader = Shader());
        void registerMesh(std::shared_ptr<Mesh> mesh, Shader shader = Shader());

        std::shared_ptr<Mesh> getMesh(unsigned int id);
        bool hasMesh(unsigned int id);
        void removeMesh(unsigned int id);

        Shader getShader(unsigned int id);
        void setShader(unsigned int id, Shader shader);

        void enableMesh(unsigned int id);
        void disableMesh(unsigned int id);
        void setMeshEnabled(unsigned int id, bool enabled);
        bool isMeshEnabled(unsigned int id);

        void registerLight(Light* light);
        void registerLight(std::shared_ptr<Light> light);

        std::shared_ptr<Light> getLight(unsigned int id);
        bool hasLight(unsigned int id);
        void removeLight(unsigned int id);

        void enableLight(unsigned int id);
        void disableLight(unsigned int id);
        void setLightEnabled(unsigned int id, bool enabled);
        bool isLightEnabled(unsigned int id);

        void setCamera(ViewProvider* camera);
        void setCamera(std::shared_ptr<ViewProvider> camera);
        std::shared_ptr<ViewProvider> getCamera();

        glm::mat4 getProjection();
        void setProjection(glm::mat4 projection);

        void renderFrame();
};

#endif//MESH_DRAWER_HPP