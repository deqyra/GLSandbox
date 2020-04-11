#ifndef CORE__SCENE__SCRIPT_HPP
#define CORE__SCENE__SCRIPT_HPP

#include <memory>

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

// Abstract class for a script referencing objects from a scene and updating them on a per-frame basis
class Script
{
    private:
        // Disallow copy-constructor and copy-assignment operator as Scripts are meant to be used only through pointers
        Script(const Script& other) = delete;
        Script& operator=(const Script& other) = delete;

        // Keeps track of how many Script objects were created (used as an ID system)
        static unsigned int _count;

    protected:
        // A SceneObject which the Script might be attached to
        SceneObjectWPtr _sceneObject;

    public:
        Script();
        virtual ~Script();

        // To be called once per frame
        virtual void update(float timeElapsed) = 0;

        // Get the parent scene object if any
        virtual SceneObjectWPtr getSceneObject();
        // Set the parent scene object
        virtual void setSceneObject(SceneObjectWPtr object);

        // Get a raw pointer to a new Script instance cloned from this. Ownership and responsibility for the allocated resources are fully transferred to the caller.
        virtual Script* clone() = 0;

        // Unique ID of the script
        const unsigned int id;
};

using ScriptPtr = std::shared_ptr<Script>;
using ScriptWPtr = std::weak_ptr<Script>;

#endif//CORE__SCENE__SCRIPT_HPP