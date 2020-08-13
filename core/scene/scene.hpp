#ifndef CORE__SCENE__SCENE_HPP
#define CORE__SCENE__SCENE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
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

// A 3D scene containing 3D objects organised in a tree structure. Handles scripts and input processing.
class Scene : public InputProcessor, public std::enable_shared_from_this<Scene>
{
    public:
        using ObjectTree = Tree<SceneObjectPtr>;
        using TransformTree = Tree<Transform>;
        using BoolTree = Tree<bool>;

    private:
        // Scene graph, containing all objects in the scene organised in a tree
        ObjectTree _objects;
        // Transform graph: tree replicating the structure of the scene graph, containing world transforms of corresponding scene objects
        TransformTree _transforms;
        // Update graph: tree replicating the structure of the scene graph, telling whether the transforms of the corresponding scene objects were modified
        BoolTree _updateMarkers;
        // Tells whether all transforms in the scene are up to date
        bool _transformsUpToDate;

        // Map scene object IDs to object metadata structs
        std::unordered_map<unsigned int, SceneObjectMetadata> _objectMetadata;
        // Map script IDs to script pointers
        std::unordered_map<unsigned int, ScriptPtr> _scripts;
        // Map input processor IDs to input processor pointers
        std::unordered_map<unsigned int, InputProcessorPtr> _inputProcessors;

        // Last time a scene update was triggered
        std::chrono::time_point<std::chrono::system_clock> _lastTime;

        // Gets called every time an object in the scene sees his transform modified
        void objectTransformModified(const unsigned int& id);
        // Mark an object for update in the update tree
        void markForUpdate(unsigned int id);

        // Recursively update world transforms in DFS order, starting with that of the object with provided ID
        void updateWorldTransformDFS(unsigned int startingId);

        // For an object with given ID, find the topmost parent node whose update marker is set
        unsigned int findFurthestOutdatedParent(unsigned int id);
        // Update the world transform of the node corresponding to the provided scene object ID, as well as all of its children's
        void cascadeWorldTransformUpdate(unsigned int id);
        // Whether the object with the provided ID has a disabled parent in the scene graph (and thus should be processed or not)
        bool hasDisabledParent(unsigned int id);

    public:
        Scene();
        // Initialize the scene and return the root scene object ID; to be called once (and only once) immediately after the scene is created
        unsigned int init();

        // Get a weak pointer to the object of provided ID
        SceneObjectWPtr operator[](unsigned int id);
        // Create a new object and attach it to the graph as a root child
        SceneObjectWPtr newObject();
        // Create a new object and attach it to the graph as a child of the object with provided ID
        SceneObjectWPtr newObject(unsigned int parentId);
        // Register an object in the scene as a root child
        void registerObject(SceneObjectPtr object);
        // Register an object in the scene as a child of the object with provided ID
        void registerObject(SceneObjectPtr object, unsigned int parentId);
        // Remove the object with provided ID from the scene, as well as all of its children
        void removeObject(unsigned int id);
        // Move the object with provided ID (as well as all of its children) in the tree so that its new parent is the object of second provided ID
        void moveObject(unsigned int id, unsigned int newParentId);
        
        // Update all transforms marked for update in DFS order
        void updateAllTransforms();
        // Get the world transform of the object with provided ID
        Transform getWorldTransform(unsigned int id);

        // Get weak pointers to all enabled scene objects
        std::vector<SceneObjectPtr> getAllObjects(bool mustBeEnabled = true);

        // Register a script in the scene, to be triggered at each updates
        void registerScript(ScriptPtr script);
        // Detach a script from the scene so that an update no longer affects it
        void detachScript(ScriptPtr script);
        // Detach the script with provided ID from the scene
        void detachScript(unsigned int id);
        // Trigger a scene update (update all registered scripts)
        void triggerUpdate();

        // Register an input processor in the scene; input events forwarded to the scene will also be forwarded to it
        void registerInputProcessor(InputProcessorPtr inputProcessor);
        // Detach an input processor from the scene so that input events are no longer forwarded to it
        void detachInputProcessor(InputProcessorPtr inputProcessor);
        // Detach the input processor with provided ID from the scene
        void detachInputProcessor(unsigned int id);

        // Register an input processing script in the scene (behaves as both a script and an input processor)
        void registerInputProcessingScript(InputProcessingScriptPtr script);
        // Detach an input processing script from the scene
        void detachInputProcessingScript(InputProcessingScriptPtr script);

        // The Scene itself is an input processor, and should be registered to the GL window as such.
        // Following methods forward all input events to registered input processors.

        // Forward framebuffer resize event to all input processors
        virtual void processFramebufferResize(GLWindowPtr window, int width, int height);
        // Forward keyboard event to all input processors
        virtual void processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods);
        // Forward mouse buttons event to all input processors
        virtual void processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods);
        // Forward mouse cursor event to all input processors
        virtual void processMouseCursor(GLWindowPtr window, double xpos, double ypos);

        // Get weak pointers to all scene objects which have a certain component
        template<class T>
        std::vector<SceneObjectWPtr> getObjectsWithComponent(bool mustBeEnabled = true);
};

template<class T>
std::vector<SceneObjectWPtr> Scene::getObjectsWithComponent(bool mustBeEnabled)
{
    std::vector<SceneObjectWPtr> all = getAllObjects();
    std::vector<SceneObjectWPtr> result;

    // Tells whether an object whose weak pointer is provided should be added to the result vector
    std::function<bool(SceneObjectWPtr)> componentChecker = [this, mustBeEnabled](SceneObjectPtr obj)
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