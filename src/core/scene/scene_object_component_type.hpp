#ifndef SCENE_OBJECT_COMPONENT_TYPE_HPP
#define SCENE_OBJECT_COMPONENT_TYPE_HPP

class MeshComponent;
class LightComponent;
class CameraComponent;

enum class SceneObjectComponentType
{
    Unknown,
    Mesh,
    Light,
    Camera
};

#endif//SCENE_OBJECT_COMPONENT_TYPE_HPP