#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <memory>

#include "scene_types_decl.hpp"

class Script
{
    protected:
        SceneObjectWPtr _sceneObject;

    public:
        Script();
        virtual ~Script();

        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        virtual void update(float currentTime, float timeElapsed) = 0;
};

using ScriptPtr = std::shared_ptr<Script>;

#endif//SCRIPT_HPP