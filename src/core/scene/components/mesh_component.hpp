#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../../mesh.hpp"
#include "../../material.hpp"
#include "../../shader.hpp"

#include <string>

class MeshComponent : public SceneObjectComponent
{
    public:
        MeshComponent(Mesh mesh);
        MeshComponent(Mesh mesh, Material material);
        MeshComponent(Mesh mesh, Shader shader);
        MeshComponent(Mesh mesh, Material material, Shader shader);

        Mesh mesh;
        Material material;
        Shader shader;
};

template<>
SceneObjectComponentType SceneObjectComponent::componentType<MeshComponent>();

template<>
std::string SceneObjectComponent::componentTypeString<MeshComponent>();

#endif//MESH_COMPONENT_HPP