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

MeshComponent* MeshComponent::clone()
{
    // By the time this shared pointer is destroyed (end of scope), responsibility will already have been shared with the cloned MeshComponent
    MeshPtr clonedMesh = std::make_shared<Mesh>(*mesh);
    // Material and shader clones are automatically copy-constructed by the following call
    return new MeshComponent(clonedMesh, material, shader);
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
