#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <memory>

#include "scene_types_decl.hpp"

class Script
{
    private:
        static unsigned int _count;

    protected:
        SceneObjectWPtr _sceneObject;

    public:
        Script();
        virtual ~Script();

        virtual void setSceneObject(SceneObjectWPtr sceneObject);

        virtual void update(float timeElapsed) = 0;

        const unsigned int id;
};

using ScriptPtr = std::shared_ptr<Script>;
using ScriptWPtr = std::weak_ptr<Script>;

#endif//SCRIPT_HPP