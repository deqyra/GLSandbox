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

class MeshDrawer
{
    private:
        std::unordered_map<unsigned int, std::shared_ptr<Mesh>> _meshes;
        std::unordered_map<unsigned int, Shader> _shaders;
        std::unordered_map<unsigned int, bool> _enabled;

        std::shared_ptr<ViewProvider> _camera;
        glm::mat4 _projection;

        void drawMeshUnsafe(unsigned int id);

    public:
        MeshDrawer(std::shared_ptr<ViewProvider> camera, glm::mat4 projection);

        void setCamera(std::shared_ptr<ViewProvider> camera);
        std::shared_ptr<ViewProvider> getCamera();

        void registerMesh(std::shared_ptr<Mesh> mesh, Shader shader = Shader());
        std::shared_ptr<Mesh> getMesh(unsigned int id);
        bool hasMesh(unsigned int id);
        void removeMesh(unsigned int id);

        void enableMesh(unsigned int id);
        void disableMesh(unsigned int id);
        void setEnabled(unsigned int id, bool enabled);
        bool isEnabled(unsigned int id);

        Shader getShader(unsigned int id);
        void setShader(unsigned int id, Shader shader);

        glm::mat4 getProjection();
        void setProjection(glm::mat4 projection);

        void drawMesh(unsigned int id);
        void drawMeshes();
};

#endif//MESH_DRAWER_HPP