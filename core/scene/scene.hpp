#ifndef CORE__SCENE__SCENE_HPP
#define CORE__SCENE__SCENE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../../tools/tree.hpp"
#include "../input_processor.hpp"

#include "scene_object.hpp"
#include "scene_object_metadata.hpp"
#include "script.hpp"
#include "input_processing_script.hpp"

class Factory;

/// @brief A scene containing 3D objects organised in a tree structure. Handles
/// self-updating scripts and processes input from the application. Use Factory
/// to instantiate and terminate.
class Scene : public InputProcessor, public std::enable_shared_from_this<Scene>
{
    public:
        using ObjectTree = Tree<SceneObjectPtr>;
        using TransformTree = Tree<Transform>;
        using BoolTree = Tree<bool>;

    private:
        /// @brief Scene graph. Contains all objects in the scene, 
        /// hierarchically organised in a tree.
        ObjectTree _objects;

        /// @brief Tree replicating the structure of the scene graph, containing
        /// the world transforms of the mirrored scene objects.
        TransformTree _transforms;

        /// @brief Tree replicating the structure of the scene graph, telling 
        /// whether the transforms of the mirrored scene objects need an update.
        BoolTree _updateMarkers;

        /// @brief Tells whether all transforms in the scene are up to date.
        unsigned int _outdatedTransformNodes;

        /// @brief Map scene object IDs to object metadata structs.
        std::unordered_map<unsigned int, SceneObjectMetadata> _objectMetadata;

        /// @brief Map script IDs to script pointers.
        std::unordered_map<unsigned int, ScriptPtr> _scripts;

        /// @brief Map input processor IDs to input processor pointers.
        std::unordered_map<unsigned int, InputProcessorPtr> _inputProcessors;

        /// @brief Last time a scene update was triggered.
        std::chrono::time_point<std::chrono::system_clock> _lastTime;

        friend Factory;

        /// @brief Initialize the root of object tree in the scene, as well as
        /// the first scene object metadata entry.
        void init();

        /// @brief Prepares the scene for destruction, by releasing strong 
        /// references in its scene objects, and delting the contents of all
        /// trees.
        void terminate();

        /// @brief Callback linked to the transform notifier of every object in 
        /// the scene.
        ///
        /// @param id ID of the scene object whose transform emitted the 
        /// notification.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        void objectTransformModified(const unsigned int& id);

        /// @brief Mark the world transform of an object for update in the
        /// update tree.
        ///
        /// @param id ID of the scene object whose world transform needs 
        /// updating.
        void markForUpdate(unsigned int id);

        /// @brief Get the metadata entry for the object of provided ID, or 
        /// throw an exception.
        ///
        /// @param id ID of the object whose metadata to find.
        ///
        /// @return The metadata entry of the object of provided ID, if found.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// registered in the scene, the function will throw a 
        /// std::runtime_error.
        SceneObjectMetadata findObjectMetaOrThrow(unsigned int id);

        /// @brief Check that the provided pointer to a scene object is not 
        /// null, or throw.
        ///
        /// @param object Pointer to check.
        ///
        /// @exception If the provided pointer is null, the function will throw
        /// a std::runtime_error.
        void checkNotNullOrThrow(SceneObjectPtr object);

        /// @brief Check that the provided scene object has no parent scene, or
        /// throw an exception.
        ///
        /// @param object Pointer to the object to check.
        ///
        /// @exception If the provided scene object already has a parent scene, 
        /// the function will throw a std::runtime_error.
        void verifyNoParentSceneOrThrow(SceneObjectPtr object);

        /// @brief Perform the actual scene object registration. Call after all
        /// sanity checks have passed.
        ///
        /// @param object Pointer to the object to register in the scene.
        /// @param parentMeta Metadata entry for the scene object which should 
        /// be parent the newly registered scene object.
        void performObjectRegistration(SceneObjectPtr object, SceneObjectMetadata& parentMeta);

        /// @brief Recursively update world transforms in a DFS traversal of the
        /// transform tree.
        ///
        /// @param startingId ID of the object whose transform should be the 
        /// starting point of the recursive update, going downwards in the tree.
        void worldTransformDFSUpdate(unsigned int startingId);

        /// @brief For an object with given ID, find the topmost parent node 
        /// whose update marker is set, and return the parent ID chain to it.
        ///
        /// @param id ID of the object from which to start searching.
        ///
        /// @return An array filled with the IDs of the successive parents of 
        /// the object, up to the topmost one whose world transform was marked
        /// as outdated.
        std::vector<unsigned int> findLongestOutdatedParentChain(unsigned int id);

        /// @brief Update the world transform of the scene object with provided 
        /// ID, as well as the world transforms of all of its children.
        ///
        /// @param id ID of the object whose transform should be the starting
        /// point of the recursive update, goind downwards in the tree.
        void worldTransformCascadeUpdate(unsigned int id);

        /// @brief Update the world transform of the scene object with provided
        /// ID, but do not reverberate the update to the children transforms.
        ///
        /// @param id ID of the object whose world transform should be updated.
        void worldTransformUpdateNoCascade(unsigned int id);

        /// @brief Whether an object has a disabled parent in the scene graph.
        ///
        /// @param id ID of the object from which to start searching.
        ///
        /// @return Whether the object with provided ID has a disabled parent.
        bool hasDisabledParent(unsigned int id);

        /// @brief Maximum value an unsigned int can hold.
        static constexpr unsigned int MaxUInt = std::numeric_limits<unsigned int>::max();

    public:
        Scene();

        /// @brief Get a pointer to the object of provided ID.
        ///
        /// @param id ID of the object whose pointer to get.
        ///
        /// @return A pointer to the object of provided ID.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        SceneObjectPtr operator[](unsigned int id);

        /// @brief Create a new object and attach it to the graph as a root 
        /// child.
        ///
        /// @param name Name to give to the scene object.
        ///
        /// @return A pointer to the newly created object.
        SceneObjectPtr newObject(std::string name = "");

        /// @brief Create a new object and attach it to the graph as a child of
        /// the object with provided ID.
        ///
        /// @param parentId ID of the object which should be parent to the newly
        /// created object.
        ///
        /// @return A pointer to the newly created object.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        SceneObjectPtr newObject(unsigned int parentId);

        /// @brief Create a new object and attach it to the graph as a child of
        /// the object with provided ID.
        ///
        /// @param name Name to give to the scene object.
        /// @param parentId ID of the object which should be parent to the newly
        /// created object.
        ///
        /// @return A pointer to the newly created object.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        SceneObjectPtr newObject(std::string name, unsigned int parentId);

        /// @brief Register an object in the scene as a root child.
        ///
        /// @param object Poitner to the object to register.
        ///
        /// @exception If the scene object is already registered to a scene, 
        /// whether it is this scene or another one, the function will throw a
        /// std::runtime_error.
        void registerObject(SceneObjectPtr object);

        /// @brief Register an object in the scene as a child of the object
        /// with provided ID.
        ///
        /// @param object Poitner to the object to register.
        /// @param parentId ID of the object which should be parent to the newly
        /// registered object.
        ///
        /// @exception If the scene object is already registered to a scene, 
        /// whether it is this scene or another one, the function will throw a
        /// std::runtime_error. If the provided ID does not match that of an 
        /// object present in the scene, the function will throw a 
        /// std::runtime_error.
        void registerObject(SceneObjectPtr object, unsigned int parentId);

        /// @brief Remove the object with provided ID from the scene, as well as
        /// all of its children.
        ///
        /// @param id ID of the object to remove from the scene.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        void removeObject(unsigned int id);

        /// @brief Move the object with provided ID (as well as all of its 
        /// children) in the tree so that its new parent is the object of 
        /// second provided ID.
        ///
        /// @param id ID of the object to move in the tree.
        /// @param newParentId ID of the object which should be the new parent
        /// to the moved object.
        /// @param worldPositionStays Whether or not the transform of the moved
        /// object should be updated so that its world transform remains the 
        /// as before being moved.
        ///
        /// @exception If the provided IDs do not match that of objects present 
        /// in the scene, the function will throw a std::runtime_error.
        void moveObject(unsigned int id, unsigned int newParentId, bool worldPositionStays = true);

        /// @brief Get the ID of the object which is parent to the object with 
        /// provided ID in the scene.
        ///
        /// @param id ID of the object whose parent ID to find.
        ///
        /// @return The ID of the object which is parent to the object with 
        /// provided ID.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        unsigned int getParentId(unsigned int id);

        /// @brief Get a pointer to the object which is parent to the object 
        /// with provided ID in the scene.
        ///
        /// @param id ID of the object whose parent to find.
        ///
        /// @return A pointer to the object which is parent to the object 
        /// with provided ID in the scene.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        SceneObjectPtr getParent(unsigned int id);
        
        /// @brief Update all world transforms of objects marked for update.
        void updateAllTransforms();

        /// @brief Get the world transform of the object with provided ID.
        ///
        /// @param id ID of the object whose world transform to get.
        /// @param cascadeUpdate In case the world transform of the object needs
        /// to be recalculated, whether or not to reverberate the update to the
        /// world transforms of the children of that object.
        ///
        /// @exception If the provided ID does not match that of an object 
        /// present in the scene, the function will throw a std::runtime_error.
        Transform getWorldTransform(unsigned int id, bool cascadeUpdate = true);

        /// @brief Get pointers to all scene objects.
        ///
        /// @param mustBeEnabled Whether or not to filter the objects depending
        /// on their enabled state.
        ///
        /// @return An array filled with pointers to all the objects in the 
        /// scene which meet the criteria.
        std::vector<SceneObjectPtr> getAllObjects(bool mustBeEnabled = true);

        /// @brief Register a script in the scene. It will then receive update
        /// signals from the scene.
        ///
        /// @param script Pointer to the script to register in the scene.
        ///
        /// @exception If the provided script pointer is null, the function will
        /// throw a std::runtime_error.
        void registerScript(ScriptPtr script);

        /// @brief Detach a script from the scene so that it no longer receives 
        /// update signals from it.
        ///
        /// @param id ID of the script to detach from the scene.
        void detachScript(unsigned int id);

        /// @brief Trigger a scene update, which will send an update signal to 
        /// all registered scripts.
        void triggerUpdate();

        /// @brief Register an input processor in the scene. Input events 
        /// received by the scene will then be forwarded to the input processor.
        ///
        /// @param inputProcessor Pointer to the input processor to register in
        /// the scene.
        ///
        /// @exception If the provided input processor pointer is null, the 
        /// function will throw a std::runtime_error.
        void registerInputProcessor(InputProcessorPtr inputProcessor);

        /// @brief Detach an input processor from the scene so that it no longer
        /// receives forwarded input from it.
        ///
        /// @param id ID of the input processor to detach from the scene.
        void detachInputProcessor(unsigned int id);

        /// @brief Register an input processing script in the scene. The script
        /// will receive both update signals and forwarded input from the scene.
        ///
        /// @param script Pointer to the script to register in the scene.
        ///
        /// @exception If the provided script pointer is null, the function will
        /// throw a std::runtime_error.
        void registerInputProcessingScript(InputProcessingScriptPtr script);

        /// @brief Detach a script from the scene so that it no longer receives 
        /// update signals nor forwarded input from it.
        ///
        /// @param script Pointer to the script to detach from the scene.
        ///
        /// @exception If the provided script pointer is null, the function will
        /// throw a std::runtime_error.
        void detachInputProcessingScript(InputProcessingScriptPtr script);

        // The Scene itself is an input processor, and should be registered to the GL window as such.
        // Following methods forward all input events to registered input processors.

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse button event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param button Literal describing which button triggered the
        /// event.
        /// @param action Literal describing what action was performed on
        /// the button which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the button event (Ctrl, Shift, etc).
        virtual void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        /// @brief Get pointers to all scene objects which have a certain 
        /// component.
        ///
        /// @tparam T Type of the concrete component to test for.
        ///
        /// @param mustBeEnabled Whether or not to filter the objects depending
        /// on their enabled state.
        ///
        /// @return An array filled with pointers to all the objects in the 
        /// scene which meet the criteria.
        template<class T>
        std::vector<SceneObjectPtr> getObjectsWithComponent(bool mustBeEnabled = true);
};

template<class T>
std::vector<SceneObjectPtr> Scene::getObjectsWithComponent(bool mustBeEnabled)
{
    std::vector<SceneObjectPtr> all = getAllObjects();
    std::vector<SceneObjectPtr> result;

    // Tells whether an object whose weak pointer is provided should be added to the result vector
    std::function<bool(SceneObjectPtr)> componentChecker = [this, mustBeEnabled](SceneObjectPtr obj) -> bool
    {
        // Skip if the object is not enabled or if any of its parents is not enabled
        if (mustBeEnabled && !obj->enabled) return false;
        if (mustBeEnabled && hasDisabledParent(obj->id)) return false;
        // Return whether the object has the component being asked for
        return obj->hasComponent<T>();
    };

    // Copy all object pointers into the result vector using the check function
    std::copy_if(all.begin(), all.end(), std::back_inserter(result), componentChecker);

    return result;
}

using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

#endif//CORE__SCENE__SCENE_HPP