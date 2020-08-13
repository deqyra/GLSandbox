#include "scene.hpp"

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

// As this->shared_from_this cannot be called until the Scene is fully constructed, the object graph is initialized with a null shared pointer,
// hence why this->init needs to be called immediately thereafter in order to properly initialize the root node with a shared pointer to this.
Scene::Scene() :
    InputProcessor(),
    _objects(SceneObjectPtr()),
    _transforms(Transform()),
    _updateMarkers(false),
    _transformsUpToDate(true),
    _objectMetadata(),
    _scripts(),
    _inputProcessors()
{
    _lastTime = std::chrono::system_clock::now();
}

unsigned int Scene::init()
{
    ObjectTree::NodePtr objectRootNode = _objects.getRoot();
    TransformTree::NodePtr transformRootNode = _transforms.getRoot();
    BoolTree::NodePtr updateRootNode = _updateMarkers.getRoot();

    // Reset the object graph root node pointer to a scene object initialized with a pointer to this Scene.
    objectRootNode->value.reset(new SceneObject(this->shared_from_this()));

    // Set and map metadata
    SceneObjectMetadata meta = {
        objectRootNode->value->id,  // ID of the object this metadata refers to
        objectRootNode->id,         // ID of the graph node containing the object
        transformRootNode->id,      // ID of the graph node containing the world matrix of the object
        updateRootNode->id,         // ID of the graph node containing the update flag of the object
        -1                          // ID of the subscription to the transform notifier of the object
    };
    _objectMetadata[meta.id] = meta;
    
    return meta.id;
}

SceneObjectWPtr Scene::operator[](unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve object metadata
    SceneObjectMetadata meta = it->second;
    // Retrieve the node
    ObjectTree::NodePtr node = _objects[meta.objectNodeId];
    return node->value;
}

SceneObjectWPtr Scene::newObject()
{
    ObjectTree::NodePtr node = _objects.getRoot();
    // Create the new object as a child of the root node
    return newObject(node->value->id);
}

SceneObjectWPtr Scene::newObject(unsigned int parentId)
{
    // Create the new object, initialized with a pointer to this Scene
    SceneObjectPtr obj = std::make_shared<SceneObject>(this->shared_from_this());

    registerObject(obj, parentId);

    return obj;
}

void Scene::registerObject(SceneObjectPtr object)
{
    ObjectTree::NodePtr node = _objects.getRoot();
    // Register object as a root child
    registerObject(object, node->value->id);
}

void Scene::registerObject(SceneObjectPtr object, unsigned int parentId)
{
    auto it = _objectMetadata.find(parentId);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(parentId) + ", cannot register new object as one of its children.";
        throw std::runtime_error(s.c_str());
    }

    ScenePtr objectScene = object->getScene().lock();
    if (objectScene == this->shared_from_this())
    {
        std::string s = "Scene: SceneObject with ID " + std::to_string(object->id) + " cannot be registered to the same scene twice. Use Scene::move instead.";
        throw std::runtime_error(s.c_str());
    }

    if (objectScene != nullptr)
    {
        std::string s = "Scene: SceneObject with ID " + std::to_string(object->id) + " already has a parent Scene, cannot register new object.";
        throw std::runtime_error(s.c_str());
    }

    // Set the scene pointer upon registering the object
    object->setScene(this->weak_from_this());

    // Retrieve parent object metadata and transform
    SceneObjectMetadata parentMeta = it->second;
    Transform parentTransform = _transforms[parentMeta.transformNodeId]->value;
    // Get transform of new object and apply parent transform to it
    Transform newTransform = (Transform)object->transform;
    newTransform = parentTransform.applyTo(newTransform);

    // Create nodes in graphs, using parent node IDs from retrieved metadata
    unsigned int objectNodeId = _objects.addNode(object, parentMeta.objectNodeId);
    unsigned int transformNodeId = _transforms.addNode(newTransform, parentMeta.transformNodeId);
    unsigned int updateNodeId = _updateMarkers.addNode(false, parentMeta.updateNodeId);

    // Hook up the notification receiver of the scene with the notifier of the object transform
    unsigned int transformSubscriberId = object->transform.getNotifier().addSubscriber(std::bind(&Scene::objectTransformModified, this, std::placeholders::_1));

    // Create metadata
    SceneObjectMetadata meta = {
        object->id,             // ID of the object this metadata refers to
        objectNodeId,           // ID of the graph node containing the object
        transformNodeId,        // ID of the graph node containing the world matrix of the object
        updateNodeId,           // ID of the graph node containing the update flag of the object
        transformSubscriberId   // ID of the subscription to the transform notifier of the object
    };
    _objectMetadata[meta.id] = meta;
}

void Scene::removeObject(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot remove object.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve object metadata
    SceneObjectMetadata meta = it->second;

    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    std::vector<ObjectTree::NodeWPtr> children = objectNode->getChildren();

    // Recursively remove all children before carrying on with the object at hand
    for (auto it = children.begin(); it != children.end(); it++)
    {
        ObjectTree::NodePtr child = it->lock();
        removeObject(child->value->id);
    }

    // Remove nodes in graphs
    _objects.removeBranch(meta.objectNodeId);
    _transforms.removeBranch(meta.transformNodeId);
    _updateMarkers.removeBranch(meta.updateNodeId);
    
    // Unsubscribe from transform notifier
    objectNode->value->transform.getNotifier().deleteSubscriber(meta.transformSubscriberId);

    // Remove metadata
    _objectMetadata.erase(meta.id);
}

void Scene::moveObject(unsigned int id, unsigned int newParentId)
{
    auto objectIt = _objectMetadata.find(id);
    if (objectIt == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot move object.";
        throw std::runtime_error(s.c_str());
    }

    auto parentIt = _objectMetadata.find(newParentId);
    if (parentIt == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(newParentId) + ", cannot move object " + std::to_string(id) + " to it.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve IDs of both nodes in all graphs
    SceneObjectMetadata meta = objectIt->second;
    SceneObjectMetadata parentMeta = parentIt->second;

    // Move nodes in all graphs
    _objects.moveBranch(meta.objectNodeId, parentMeta.objectNodeId);
    _transforms.moveBranch(meta.transformNodeId, parentMeta.transformNodeId);
    _updateMarkers.moveBranch(meta.updateNodeId, parentMeta.updateNodeId);

    // Mark object for update
    markForUpdate(id);
}

void Scene::updateAllTransforms()
{
    ObjectTree::NodePtr objectRootNode = _objects.getRoot();
    std::vector<ObjectTree::NodeWPtr> childNodes = objectRootNode->getChildren();

    // Start the DFS routine on each of the root children
    for(auto it = childNodes.begin(); it != childNodes.end(); it++)
    {
        SceneObjectPtr object = it->lock()->value;
        updateWorldTransformDFS(object->id);
    }

    _transformsUpToDate = false;
}

Transform Scene::getWorldTransform(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot retrieve world transform.";
        throw std::runtime_error(s.c_str());
    }

    // Update transform if required
    if (!_transformsUpToDate)
    {
        // Find the topmost outdated parent
        unsigned int outdated = findFurthestOutdatedParent(id);

        // If no outdated parent was found, -1 was returned
        if (outdated == -1)
        {
            SceneObjectMetadata meta = it->second;
            // If the update marker of the considered object is set, update its transform along with all of its children's
            if (_updateMarkers[meta.updateNodeId]) cascadeWorldTransformUpdate(id);

            // If it isn't set, the transform of that is up to date and no action is required
        }
        // If an outdated parent was found, update its transform along with all of its children's
        else cascadeWorldTransformUpdate(outdated);
    }

    // Retrieve matrix graph node ID and return matrix
    SceneObjectMetadata meta = it->second;
    TransformTree::NodePtr node = _transforms[meta.transformNodeId];
    return node->value;
}

std::vector<SceneObjectPtr> Scene::getAllObjects(bool mustBeEnabled)
{
    std::vector<SceneObjectPtr> result;

    for (auto it = _objectMetadata.begin(); it != _objectMetadata.end(); it++)
    {
        SceneObjectMetadata meta = it->second;
        ObjectTree::NodePtr node = _objects[meta.objectNodeId];
        if (!mustBeEnabled || node->value->enabled) result.push_back(node->value);
    }

    return result;
}

void Scene::registerScript(ScriptPtr script)
{
    _scripts[script->id] = script;
}

void Scene::detachScript(ScriptPtr script)
{
    _scripts.erase(script->id);
}

void Scene::detachScript(unsigned int id)
{
    _scripts.erase(id);
}

void Scene::triggerUpdate()
{
    // Get time delta (in seconds) and upate last update time
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> delta = now - _lastTime;
    _lastTime = now;

    // Update all registered scripts
    for (auto it = _scripts.begin(); it != _scripts.end(); it++)
    {
        it->second->update((float)(delta.count()));
    }
}

void Scene::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessors[inputProcessor->id] = inputProcessor;
}

void Scene::detachInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessors.erase(inputProcessor->id);
}

void Scene::detachInputProcessor(unsigned int id)
{
    _inputProcessors.erase(id);
}

void Scene::registerInputProcessingScript(InputProcessingScriptPtr script)
{
    // Register the input processing script both as a script and an input processor
    ScriptPtr baseScriptPtr = std::static_pointer_cast<Script>(script);
    InputProcessorPtr baseIpPtr = std::static_pointer_cast<InputProcessor>(script);
    registerScript(baseScriptPtr);
    registerInputProcessor(baseIpPtr);
}

void Scene::detachInputProcessingScript(InputProcessingScriptPtr script)
{
    _scripts.erase(script->Script::id);
    _inputProcessors.erase(script->InputProcessor::id);
}

void Scene::processFramebufferResize(GLWindowPtr window, int width, int height)
{
    // Forward framebuffer resize event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processFramebufferResize(window, width, height);
    }
}

void Scene::processKeyboard(GLWindowPtr window, Window::Input::Key key, int scancode, Window::Input::Action action, int mods)
{
    // Forward keyboard event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processKeyboard(window, key, scancode, action, mods);
    }
}

void Scene::processMouseButton(GLWindowPtr window, Window::Input::MouseButton button, Window::Input::Action action, int mods)
{
    // Forward mouse button event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processMouseButton(window, button, action, mods);
    }
}

void Scene::processMouseCursor(GLWindowPtr window, double xpos, double ypos)
{
    // Forward mouse cursor event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processMouseCursor(window, xpos, ypos);
    }
}

void Scene::objectTransformModified(const unsigned int& id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: the object transform callback was called with non-existing ID " + std::to_string(id) + "!";
        throw std::runtime_error(s.c_str());
    }

    markForUpdate(id);
}

void Scene::markForUpdate(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve object metadata
    SceneObjectMetadata meta = it->second;
    BoolTree::NodePtr updateNode = _updateMarkers[meta.updateNodeId];
    // Set the marker to true
    updateNode->value = true;
    _transformsUpToDate = false;
}

void Scene::updateWorldTransformDFS(unsigned int startingId)
{
    SceneObjectMetadata meta = _objectMetadata[startingId];

    if (_updateMarkers[meta.updateNodeId]->value)
    {
        // If the current object is marked for update, update all transforms downwards
        cascadeWorldTransformUpdate(startingId);
    }
    else
    {
        // Otherwise, run the DFS routine on each of its children
        ObjectTree::NodePtr object = _objects[meta.objectNodeId];
        std::vector<ObjectTree::NodeWPtr> children = object->getChildren();

        for (auto it = children.begin(); it != children.end(); it++)
        {
            SceneObjectPtr child = it->lock()->value;
            updateWorldTransformDFS(child->id);
        }
    }
}

unsigned int Scene::findFurthestOutdatedParent(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve the object node ID, the node pointer as well as its parent chain
    SceneObjectMetadata meta = it->second;
    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    std::vector<ObjectTree::NodeWPtr> parentChain = objectNode->getParentChain();

    // Keep track of the furthest parent in the chain whose transform was outdated
    unsigned int furthestOutdatedId = -1;
    for (auto parentIt = parentChain.begin(); parentIt != parentChain.end(); parentIt++)
    {
        unsigned int id = parentIt->lock()->value->id;
        SceneObjectMetadata meta = _objectMetadata[id];

        // Use the flag to check on the modified state of the object transform
        if (_updateMarkers[meta.updateNodeId])
        {
            furthestOutdatedId = id;
        }
    }

    return furthestOutdatedId;
}

void Scene::cascadeWorldTransformUpdate(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve node metadata
    SceneObjectMetadata meta = it->second;

    // Get object node pointer as well as the parent transform node pointer
    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    TransformTree::NodePtr transformNode = _transforms[meta.transformNodeId];
    TransformTree::NodePtr parentTransformNode = transformNode->getParent().lock();

    if (parentTransformNode != nullptr)
    {
        // Apply the parent world transform to the object transform, and save it to the object world transform node
        Transform newTransform = parentTransformNode->value.applyTo((Transform)objectNode->value->transform);
        transformNode->value = newTransform;
    }
    else
    {
        transformNode->value = (Transform)objectNode->value->transform;
    }

    // Reset the update marker
    _updateMarkers[meta.updateNodeId]->value = false;
    
    // Reverberate changes to children transforms
    std::vector<ObjectTree::NodeWPtr> children = objectNode->getChildren();
    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        ObjectTree::NodePtr child = childIt->lock();
        cascadeWorldTransformUpdate(child->id);
    }
}

bool Scene::hasDisabledParent(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve graph node IDs, object node pointer and node parent chain
    SceneObjectMetadata meta = it->second;
    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    std::vector<ObjectTree::NodeWPtr> parents = objectNode->getParentChain();

    for (auto it = parents.begin(); it != parents.end(); it++)
    {
        // Return whether any parent in the chain is disabled
        auto parent = it->lock();
        if (!parent->value->enabled) return true;
    }
    return false;
}
