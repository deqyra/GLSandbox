#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include <string>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/shader/shader_program.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

namespace Renderboi
{

/// @brief Component allowing to attach a mesh to a scene object.
class MeshComponent : public Component
{
    private:
        MeshComponent(MeshComponent& other) = delete;
        MeshComponent& operator=(const MeshComponent& other) = delete;

        /// @brief Pointer to the mesh data of the object.
        MeshPtr _mesh;

        /// @brief Material to paint the mesh with.
        Material _material;
        
        /// @brief Shader program to render the mesh with.
        ShaderProgram _shader;

    public:
        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh, const Material material);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param shader Shader program which the mesh should be rendred by.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh, const ShaderProgram shader);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        /// @param shader Shader program which the mesh should be rendred by.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(const SceneObjectPtr sceneObject, const MeshPtr mesh, const Material material, const ShaderProgram shader);

        ~MeshComponent();

        /// @brief Get a pointer to the mesh used by the component.
        ///
        /// @return A pointer to the mesh used by the component.
        MeshPtr getMesh() const;

        /// @brief Set the mesh used by the component.
        ///
        /// @param mesh The new mesh to be used by the component.
        ///
        /// @exception If the provided mesh pointer is null, the function will
        /// throw a std::runtime_error.
        void setMesh(const MeshPtr mesh);

        /// @brief Get the material used by the component.
        ///
        /// @return The material used by the component.
        Material getMaterial() const;

        /// @brief Set the material used by the component.
        ///
        /// @param material The new material to be used by the component.
        void setMaterial(const Material material);

        /// @brief Get the shader used by the component.
        ///
        /// @return The shader used by the component.
        ShaderProgram getShader() const;

        /// @brief Set the shader used by the component.
        ///
        /// @param shader The new shader to be used by the component.
        void setShader(const ShaderProgram shader);

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from Component ///
        ///                                   ///
        /////////////////////////////////////////

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @param newParent Pointer the scene object which will be parent to
        /// the cloned component instance.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        MeshComponent* clone(const SceneObjectPtr newParent) const override;
};

template<>
ComponentType Component::componentType<MeshComponent>();

template<>
std::string Component::componentTypeString<MeshComponent>();

template<>
bool Component::multipleInstancesAllowed<MeshComponent>();

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP