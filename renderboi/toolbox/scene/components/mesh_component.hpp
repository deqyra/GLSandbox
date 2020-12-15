#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include <string>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/shader/shader_program.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

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
        MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param shader Shader program which the mesh should be rendred by.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, ShaderProgram shader);

        /// @param sceneObject Pointer to the scene object which will be parent
        /// to this component.
        /// @param mesh Pointer to the mesh which the component should use.
        /// @param material Material which the mesh should be rendered with.
        /// @param shader Shader program which the mesh should be rendred by.
        ///
        /// @exception If the provided mesh pointer is null, the constructor
        /// will throw a std::runtime_error.
        MeshComponent(SceneObjectPtr sceneObject, MeshPtr mesh, Material material, ShaderProgram shader);

        virtual ~MeshComponent();

        /// @brief Get a pointer to the mesh used by the component.
        ///
        /// @return A pointer to the mesh used by the component.
        MeshPtr getMesh();

        /// @brief Set the mesh used by the component.
        ///
        /// @param mesh The new mesh to be used by the component.
        ///
        /// @exception If the provided mesh pointer is null, the function will
        /// throw a std::runtime_error.
        void setMesh(MeshPtr mesh);

        /// @brief Get the material used by the component.
        ///
        /// @return The material used by the component.
        Material getMaterial();

        /// @brief Set the material used by the component.
        ///
        /// @param material The new material to be used by the component.
        void setMaterial(Material material);

        /// @brief Get the shader used by the component.
        ///
        /// @return The shader used by the component.
        ShaderProgram getShader();

        /// @brief Set the shader used by the component.
        ///
        /// @param shader The new shader to be used by the component.
        void setShader(ShaderProgram shader);

        /// @brief Get a raw pointer to a new component instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @param newParent Pointer the scene object which will be parent to
        /// the cloned component instance.
        ///
        /// @return A raw pointer to the component instance cloned from this 
        /// one.
        virtual MeshComponent* clone(SceneObjectPtr newParent);
};

template<>
ComponentType Component::componentType<MeshComponent>();

template<>
std::string Component::componentTypeString<MeshComponent>();

template<>
bool Component::multipleInstancesAllowed<MeshComponent>();

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP