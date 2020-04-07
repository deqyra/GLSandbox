#include "mesh_component.hpp"

using CompType = ComponentType;

MeshComponent::MeshComponent(MeshPtr mesh) :
    MeshComponent(mesh, Material(), Shader())
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Material material) :
    MeshComponent(mesh, material, Shader())
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Shader shader) :
    MeshComponent(mesh, Material(), shader)
{

}

MeshComponent::MeshComponent(MeshPtr mesh, Material material, Shader shader) :
    Component(CompType::Mesh),
    mesh(mesh),
    material(material),
    shader(shader)
{

}

MeshComponent::~MeshComponent()
{

}

template<>
ComponentType Component::componentType<MeshComponent>()
{
    return ComponentType::Mesh;
}

template<>
std::string Component::componentTypeString<MeshComponent>()
{
    return "Mesh";
}
