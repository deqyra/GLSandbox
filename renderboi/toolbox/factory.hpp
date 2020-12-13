#ifndef RENDERBOI__TOOLBOX__FACTORY_HPP
#define RENDERBOI__TOOLBOX__FACTORY_HPP

#include <renderboi/core/materials.hpp>
#include <renderboi/core/mesh.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

#include "script.hpp"
#include "input_processing_script.hpp"
#include "mesh_generator.hpp"
#include "mesh_generators/mesh_type.hpp"
#include "mesh_generators/type_to_gen_mapping.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/component.hpp"
#include "scene/components/mesh_component.hpp"
#include "scene/components/script_component.hpp"
#include "scene/components/input_processing_script_component.hpp"

/// @brief Provides static methods to facilitate complex object instantiation.
class Factory
{
    public:
        /// @brief Instantiate and initialize a new scene.
        ///
        /// @return Pointer to the instantiated scene.
        static ScenePtr makeScene();

        /// @brief Terminate a scene. Also releases the provided pointer.
        ///
        /// @param scene Pointer to the scene to terminate, which will be 
        /// released once done.
        static void destroyScene(ScenePtr& scene);

        /// @brief Instantiate and initialize a new scene object.
        ///
        /// @param name Name to give to the scene object.
        ///
        /// @return Pointer to the instantiated scene object.
        static SceneObjectPtr makeSceneObject(std::string name = "");

        /// @brief Create a mesh with vertices arranged in a certain shape.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param parameters Parameters to pass to the mesh generator.
        /// Look up generators in core/mesh_generators/ for details.
        ///
        /// @return Pointer to the generated mesh.
        template<MeshType T>
        static MeshPtr makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters);

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<MeshType T>
        static SceneObjectPtr makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat = Material(), ShaderProgram shader = ShaderBuilder::MinimalShaderProgram());

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param name Name to give to the scene object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<MeshType T>
        static SceneObjectPtr makeSceneObjectWithMesh(std::string name, typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat = Materials::Default, ShaderProgram shader = ShaderBuilder::MinimalShaderProgram());

        /// @brief Create a script of any type and attach it to a scene object.
        ///
        /// @tparam T The class of the script to instantiate.
        /// @tparam ArgTypes Types of the arguments which will be used to create
        /// the script.
        ///
        /// @param object Pointer to the scene object to which the newly 
        /// instantiated script should be attached.
        /// @param args Arguments to use to construct the script.
        ///
        /// @return A pointer to the newly instantiated script.
        ///
        /// @exception If the provided scene object does not meet criteria 
        /// required by the script about to be attached to it, exceptions may be
        /// thrown and behavior may be off.
        template<class T, typename... ArgTypes>
        static std::shared_ptr<T> createScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args);

        /// @brief Create an input processing script of any type and attach it
        /// to a scene object.
        ///
        /// @tparam T The class of the input processing script to instantiate.
        /// @tparam ArgTypes Types of the arguments which will be used to create
        /// the input processing script.
        ///
        /// @param object Pointer to the scene object to which the newly 
        /// instantiated script should be attached.
        /// @param args Arguments to use to construct the input processing 
        /// script.
        ///
        /// @return A pointer to the newly instantiated input processing script.
        ///
        /// @exception If the provided scene object does not meet criteria 
        /// required by the input processing script about to be attached to it,
        /// exceptions may be thrown and behavior may be off.
        template<class T, typename... ArgTypes>
        static std::shared_ptr<T> createInputProcessingScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args);
};

template<MeshType T>
MeshPtr Factory::makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters)
{
    using GenType = typename TypeToGenMapping<T>::GenType;
    std::shared_ptr<GenType> gen = std::make_shared<GenType>(parameters);
    return gen->generatePtr();
}

template<MeshType T>
SceneObjectPtr Factory::makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat, ShaderProgram shader)
{
    return makeSceneObjectWithMesh("", parameters, mat, shader);
}

template<MeshType T>
SceneObjectPtr Factory::makeSceneObjectWithMesh(std::string name, typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat, ShaderProgram shader)
{
    MeshPtr mesh = Factory::makeMesh<T>(parameters);
    SceneObjectPtr obj = Factory::makeSceneObject(name);
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}

template<typename T, typename... ArgTypes>
std::shared_ptr<T> Factory::createScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args)
{
    std::shared_ptr<T> script = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    ScriptPtr baseScript = std::static_pointer_cast<Script>(script);
    object->addComponent<ScriptComponent>(baseScript);

    return script;
}

template<typename T, typename... ArgTypes>
std::shared_ptr<T> Factory::createInputProcessingScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args)
{
    std::shared_ptr<T> ipScript = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    InputProcessingScriptPtr baseIpScript = std::static_pointer_cast<InputProcessingScript>(ipScript);
    object->addComponent<InputProcessingScriptComponent>(baseIpScript);

    return ipScript;
}

#endif//RENDERBOI__TOOLBOX__FACTORY_HPP