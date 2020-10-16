#ifndef CORE__SCENE__SCRIPT_HPP
#define CORE__SCENE__SCRIPT_HPP

#include <memory>

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

/// @brief Abstract class to autonomously update scene object on a per-frame 
/// basis.
class Script
{
    private:
        Script(const Script& other) = delete;
        Script& operator=(const Script& other) = delete;

        // Keeps track of how many script instances were created (used as a 
        /// unique ID system).
        static unsigned int _count;

    protected:
        /// @brief Pointer to a scene object which the script might be attached
        /// to.
        SceneObjectWPtr _sceneObject;

    public:
        Script();

        virtual ~Script();

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(float timeElapsed) = 0;

        /// @brief Get a pointer to the parent scene object, if any.
        ///
        /// @return A pointer to the parent scene object, if any.
        virtual SceneObjectWPtr getSceneObject();

        /// @brief Set the parent scene object of the script.
        ///
        /// @param object A pointer to scene object which should be parent to 
        /// the script.
        virtual void setSceneObject(SceneObjectWPtr object);

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual Script* clone() = 0;

        /// @brief Unique ID of the script.
        const unsigned int id;
};

using ScriptPtr = std::shared_ptr<Script>;
using ScriptWPtr = std::weak_ptr<Script>;

#endif//CORE__SCENE__SCRIPT_HPP