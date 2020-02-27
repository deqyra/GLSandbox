/**
    GLTest, mesh_drawer.hpp
    Purpose: Define a class to draw any mesh.

    @author François Brachais (deqyra)
    @version 1.0 07/02/2020
 */
#ifndef MESH_DRAWER_HPP
#define MESH_DRAWER_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "view_projection_provider.hpp"

#include "light_type.hpp"
#include "light.hpp"
#include "lights/point_light.hpp"

#include "ubo/matrix_ubo.hpp"
#include "ubo/light_ubo.hpp"

#define MAX_POINT_LIGHTS 128

class MeshDrawer
{
    private:
        std::unordered_map<unsigned int, MeshPtr> _meshes;
        std::unordered_map<unsigned int, Shader> _shaders;
        std::unordered_map<unsigned int, bool> _meshesEnabled;

        std::unordered_map<unsigned int, LightPtr> _lights;
        std::unordered_map<unsigned int, bool> _lightsEnabled;

        std::shared_ptr<ViewProjectionProvider> _camera;

        MatrixUBO _matrixUbo;
        LightUBO _lightUbo;

        void drawMesh(unsigned int id);
        void sendLightData();

    public:
        MeshDrawer(std::shared_ptr<ViewProjectionProvider> camera);

        void registerMesh(MeshPtr mesh, Shader shader = Shader());

        MeshPtr getMesh(unsigned int id);
        bool hasMesh(unsigned int id);
        void removeMesh(unsigned int id);

        Shader getShader(unsigned int id);
        void setShader(unsigned int id, Shader shader);

        void enableMesh(unsigned int id);
        void disableMesh(unsigned int id);
        void setMeshEnabled(unsigned int id, bool enabled);
        bool isMeshEnabled(unsigned int id);

        void registerLight(LightPtr light);

        LightPtr getLight(unsigned int id);
        bool hasLight(unsigned int id);
        void removeLight(unsigned int id);

        void enableLight(unsigned int id);
        void disableLight(unsigned int id);
        void setLightEnabled(unsigned int id, bool enabled);
        bool isLightEnabled(unsigned int id);

        void setCamera(std::shared_ptr<ViewProjectionProvider> camera);
        std::shared_ptr<ViewProjectionProvider> getCamera();

        void renderFrame();
};

#endif//MESH_DRAWER_HPP