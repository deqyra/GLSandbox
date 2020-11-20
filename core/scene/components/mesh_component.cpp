#include "mesh_component.hpp"

#include "../../shader/shader_builder.hpp"
#include "../../materials.hpp"

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh) :
    MeshComponent(sceneObject, mesh, Materials::Default, ShaderBuilder::buildMinimalShaderProgram())
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material) :
    MeshComponent(sceneObject, mesh, material, ShaderBuilder::buildMinimalShaderProgram())
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, ShaderProgram shader) :
    MeshComponent(sceneObject, mesh, Materials::Default, shader)
{

}

MeshComponent::MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material, ShaderProgram shader) :
    Component(ComponentType::Mesh, sceneObject),
    _mesh(mesh),
    _material(material),
    _shader(shader)
{
    if (!mesh)
    {
        throw std::runtime_error("MeshComponent: cannot construct from null mesh pointer.");
    }
}

MeshComponent::~MeshComponent()
{

}

MeshPtr MeshComponent::getMesh()
{
    return _mesh;
}

void MeshComponent::setMesh(MeshPtr mesh)
{
    if (!mesh)
    {
        throw std::runtime_error("MeshComponent: cannot set mesh pointer to null.");
    }

    _mesh = mesh;
}

Material MeshComponent::getMaterial()
{
    return _material;
}

void MeshComponent::setMaterial(Material material)
{
    _material = material;
}

ShaderProgram MeshComponent::getShader()
{
    return _shader;
}

void MeshComponent::setShader(ShaderProgram shader)
{
    _shader = shader;
}

MeshComponent* MeshComponent::clone(SceneObjectPtr newParent)
{
    // By the time this shared pointer is destroyed (end of scope), responsibility will already have been shared with the cloned MeshComponent
    MeshPtr clonedMesh = std::make_shared<Mesh>(*_mesh);
    // Material and shader clones are automatically copy-constructed by the following call
    return new MeshComponent(newParent, clonedMesh, _material, _shader);
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

template<>
bool Component::multipleInstancesAllowed<MeshComponent>()
{
    return false;
}
