#ifndef CORE__SCENE__SCENE_HPP
#define CORE__SCENE__SCENE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../../tools/tree.hpp"
#include "../input_processor.hpp"

#include "scene_object.hpp"
#include "script.hpp"
#include "input_processing_script.hpp"

// A 3D scene containing 3D objects organised in a tree structure. Handles scripts and input processing.
class Scene : public InputProcessor, public std::enable_shared_from_this<Scene>
{
    public:
        using ObjTree = Tree<SceneObjectPtr>;
        using MatTree = Tree<glm::mat4>;

    private:
        // Scene graph, containing all objects in the scene in a tree
        ObjTree _graph;
        // Matrix graph: tree whose structure is identical to that of the scene graph, containing world model matrices of corresponding scene objects
        MatTree _modelMatrices;
        // Map scene object IDs to tree node IDs (both scene object graph and matrix graph node IDS)
        std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> _objectIdsToNodeIds;
        // Map script IDs to script pointers
        std::unordered_map<unsigned int, ScriptPtr> _scripts;
        // Map input processor IDs to input processor pointers
        std::unordered_map<unsigned int, InputProcessorPtr> _inputProcessors;

        // Last time a scene update was triggered
        std::chrono::time_point<std::chrono::system_clock> _lastTime;

        // Trigger a world model matrix update on the node corresponding to the provided scene object ID, looking for changes in the parent chain and updating it alongside with it
        void processOutdatedTransformsFromNode(unsigned int id);
        // Update the world model matrix of the node corresponding to the provided scene object ID, as well as all of its children's
        void recalculateModelMatrix(unsigned int id);
        // Remove the ID mapping from a provided object ID to the corresponding nodes in the scene graph and matrix graph
        void removeIdMappings(unsigned int id);
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
        // Get the world model matrix of the object with provided ID
        glm::mat4 getWorldModelMatrix(unsigned int id);
        // Get the world position of the object with provided ID
        glm::vec3 getWorldPosition(unsigned int id);

        // Get weak pointers to all enabled scene objects
        std::vector<SceneObjectWPtr> getAllObjects();

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

        // The Scene itself is an input processor, and should be registered to the GLFW window as such.
        // Following methods forward all input events to registered input processors.

        // Forward framebuffer resize event to all input processors
        virtual void processFramebufferResize(GLFWwindow* window, int width, int height);
        // Forward keyboard event to all input processors
        virtual void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        // Forward mouse buttons event to all input processors
        virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods);
        // Forward mouse cursor event to all input processors
        virtual void processMouseCursor(GLFWwindow* window, double xpos, double ypos);

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
    std::function<bool(SceneObjectWPtr)> componentChecker = [this, mustBeEnabled](SceneObjectWPtr obj)
    {
        SceneObjectPtr strongObj = obj.lock();
        // Skip if the object is not enabled or if any of its parents is not enabled
        if (mustBeEnabled && !strongObj->enabled) return false;
        if (mustBeEnabled && hasDisabledParent(strongObj->id)) return false;
        // Return whether the object has the component being asked for
        return strongObj->hasComponent<T>();
    };

    // Copy all object pointers into the result vector using the check function
    std::copy_if(all.begin(), all.end(), std::back_inserter(result), componentChecker);

    return result;
}

using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

#endif//CORE__SCENE__SCENE_HPP