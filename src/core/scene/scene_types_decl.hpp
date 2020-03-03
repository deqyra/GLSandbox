#ifndef SCENE_OBJECT_DECL_HPP
#define SCENE_OBJECT_DECL_HPP

#include <memory>

class SceneObjectComponent;
using SceneObjectComponentPtr = std::shared_ptr<SceneObjectComponent>;
using SceneObjectComponentWPtr = std::weak_ptr<SceneObjectComponent>;

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

class Scene;
using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

#endif//SCENE_OBJECT_DECL_HPP