#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <memory>

#include "scene_types_decl.hpp"

class Script
{
    private:
        SceneObjectWPtr _sceneObject;

    public:
        Script();
        virtual ~Script();

        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        virtual void update() = 0;
};

using ScriptPtr = std::shared_ptr<Script>;

#endif//SCRIPT_HPP