#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "scene_types_decl.hpp"
#include "scene_object_component_type.hpp"

#include <memory>
#include <string>

class SceneObjectComponent
{
    private:
        SceneObjectComponent() = delete;

    protected:
        SceneObjectComponent(SceneObjectComponentType type, SceneObjectWPtr sceneObject = SceneObjectWPtr());
        SceneObjectWPtr _sceneObject;

    public:
        virtual ~SceneObjectComponent();

        const SceneObjectComponentType type;

        virtual SceneObjectWPtr getSceneObject();
        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        template<class T>
        static SceneObjectComponentType componentType();

        template<class T>
        static std::string componentTypeString();
};

template<class T>
SceneObjectComponentType SceneObjectComponent::componentType()
{
    return SceneObjectComponentType::Unknown;
}

template<class T>
std::string SceneObjectComponent::componentTypeString()
{
    return "Unknown";
}

/* INHERITING FROM SceneObjectComponent
 * ====================================
 * (waiting for a better solution)
 *
 * - Write new component
 * - Add enum value to SceneObjectComponentType
 * - Include the following functions in the header :
 *     template<>
 *     SceneObjectComponentType SceneObjectComponent::componentType<NewComponent>();
 *
 *     template<>
 *     std::string SceneObjectComponent::componentTypeString<NewComponent>();
 * - Implement them in the source file
 * - Add new template specialization for the component in tools.hpp
 * - Add condition branch in cloneComponent in tools.cpp
 */

#endif//COMPONENT_HPP