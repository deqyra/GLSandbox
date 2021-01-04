#ifndef RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP

#include <memory>

#include "scene/scene_object.hpp"

namespace Renderboi
{

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
        SceneObjectPtr _sceneObject;

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
        /// @return A pointer to the parent scene object, or nullptr.
        virtual SceneObjectPtr getSceneObject() const;

        /// @brief Set the parent scene object of the script.
        ///
        /// @param object A pointer to scene object which should be parent to 
        /// the script (can be nullptr).
        virtual void setSceneObject(const SceneObjectPtr object);

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual Script* clone() const = 0;

        /// @brief Unique ID of the script.
        const unsigned int id;
};

using ScriptPtr = std::shared_ptr<Script>;
using ScriptWPtr = std::weak_ptr<Script>;

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP