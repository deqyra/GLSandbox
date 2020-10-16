#include "scene.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../../tools/tree.hpp"
#include "../factory.hpp"
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
    _outdatedTransformNodes(0),
    _objectMetadata(),
    _scripts(),
    _inputProcessors(),
    _lastTime(std::chrono::system_clock::now())
{
    
}

void Scene::init()
{
    ObjectTree::NodePtr objectRootNode = _objects.getRoot();
    TransformTree::NodePtr transformRootNode = _transforms.getRoot();
    BoolTree::NodePtr updateRootNode = _updateMarkers.getRoot();

    // Reset the object graph root node pointer to a scene object initialized with a pointer to this Scene.
    objectRootNode->value.reset(new SceneObject());
    objectRootNode->value->setScene(this->shared_from_this());

    // Set and map metadata
    SceneObjectMetadata meta = {
        objectRootNode->value->id,  // ID of the object this metadata refers to
        MaxUInt,                    // ID of the object which is parent to the object this metadata refers to
        objectRootNode->id,         // ID of the graph node containing the object
        transformRootNode->id,      // ID of the graph node containing the world matrix of the object
        updateRootNode->id,         // ID of the graph node containing the update flag of the object
        MaxUInt                     // ID of the subscription to the transform notifier of the object
    };
    _objectMetadata[meta.id] = meta;
}

SceneObjectPtr Scene::operator[](unsigned int id)
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

SceneObjectPtr Scene::newObject()
{
    ObjectTree::NodePtr node = _objects.getRoot();
    // Create the new object as a child of the root node
    return newObject(node->value->id);
}

SceneObjectPtr Scene::newObject(unsigned int parentId)
{
    // Create the new object, initialized with a pointer to this Scene
    SceneObjectPtr obj = Factory::makeSceneObject();

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
    object->setScene(this->shared_from_this());

    // Retrieve parent object metadata and transform
    SceneObjectMetadata parentMeta = it->second;
    Transform parentTransform = _transforms[parentMeta.transformNodeId]->value;
    // Get transform of new object and apply parent transform to it
    Transform newTransform = (Transform)object->transform;
    newTransform = newTransform.applyOver(parentTransform);

    // Create nodes in graphs, using parent node IDs from retrieved metadata
    unsigned int objectNodeId = _objects.addNode(object, parentMeta.objectNodeId);
    unsigned int transformNodeId = _transforms.addNode(newTransform, parentMeta.transformNodeId);
    unsigned int updateNodeId = _updateMarkers.addNode(false, parentMeta.updateNodeId);

    // Hook up the notification receiver of the scene with the notifier of the object transform
    std::function<void(const unsigned int&)> callback = [this](const unsigned int& id)
    {
        this->objectTransformModified(id);
    };
    unsigned int transformSubscriberId = object->transform.getNotifier().addSubscriber(callback);

    // Create metadata
    SceneObjectMetadata meta = {
        object->id,             // ID of the object this metadata refers to
        parentId,               // ID of the object which is parent to the object this metadata refers to
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

    // Within the markers about to be removed, subtract the count of those set to true from the outdated transform count
    _outdatedTransformNodes -= _updateMarkers.countValue(true, meta.updateNodeId);

    // Remove nodes in graphs
    _objects.removeBranch(meta.objectNodeId);
    _transforms.removeBranch(meta.transformNodeId);
    _updateMarkers.removeBranch(meta.updateNodeId);
    
    // Unsubscribe from transform notifier
    objectNode->value->transform.getNotifier().deleteSubscriber(meta.transformSubscriberId);

    // Remove metadata
    _objectMetadata.erase(meta.id);
}

void Scene::moveObject(unsigned int id, unsigned int newParentId, bool worldPositionStays)
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

    // Fetch the world transform of moved object, updating if necessary
    Transform worldTransform = getWorldTransform(id);

    // Move nodes in all graphs
    _objects.moveBranch(meta.objectNodeId, parentMeta.objectNodeId);
    _transforms.moveBranch(meta.transformNodeId, parentMeta.transformNodeId);
    _updateMarkers.moveBranch(meta.updateNodeId, parentMeta.updateNodeId);

    if (worldPositionStays)
    {
        // Get the world transform of the new parent, not cascading the update
        Transform parentTransform = getWorldTransform(newParentId, false);
        _objects[meta.objectNodeId]->value->transform = worldTransform.compoundFrom(parentTransform);
        // No need to mark the object for update as it stayed in place
    }
    // Mark object for update if it did not keep its world position
    else markForUpdate(id);
}

unsigned int Scene::getParentId(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve object metadata
    SceneObjectMetadata meta = it->second;
    return meta.parentId;
}

SceneObjectPtr Scene::getParent(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve object metadata
    SceneObjectMetadata meta = it->second;
    return _objects[meta.parentId]->value;
}

void Scene::updateAllTransforms()
{
    if (_outdatedTransformNodes)
    {
        ObjectTree::NodePtr objectRootNode = _objects.getRoot();
        std::vector<ObjectTree::NodeWPtr> childNodes = objectRootNode->getChildren();

        // Start the DFS routine on each of the root children
        for(auto it = childNodes.begin(); it != childNodes.end(); it++)
        {
            SceneObjectPtr object = it->lock()->value;
            worldTransformDFSUpdate(object->id);
        }

        _outdatedTransformNodes = 0;
    }
}

Transform Scene::getWorldTransform(unsigned int id, bool cascadeUpdate)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot retrieve world transform.";
        throw std::runtime_error(s.c_str());
    }

    SceneObjectMetadata meta = it->second;

    // Update transform if required
    if (_outdatedTransformNodes)
    {
        // Find the longest outdated parent
        std::vector<unsigned int> outdatedIds = findLongestOutdatedParentChain(id);

        // If no outdated parent was found, the vector is empty
        if (outdatedIds.size() == 0)
        {
            // If the update marker of the considered object is set, update its transform
            if (_updateMarkers[meta.updateNodeId]->value)
            {
                // Update it along with all of its children's if appropriate
                if (cascadeUpdate) worldTransformCascadeUpdate(id);
                else worldTransformUpdateNoCascade(id);
            }

            // If it isn't set, the transform of that object is up to date and no action is required
        }
        // If an outdated parent chain was found
        else
        {
            // Update the furthest parent, cascading, if appropriate
            if (cascadeUpdate) worldTransformCascadeUpdate(outdatedIds.back());
            else
            {
                // Otherwise, update each element in the chain from the top, not cascading
                for (auto it = outdatedIds.rbegin(); it != outdatedIds.rend(); it++)
                {
                    worldTransformUpdateNoCascade(*it);
                }
            }
        }
    }

    // Retrieve matrix graph node ID and return matrix
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
    if (script == nullptr)
    {
        std::string s = "Scene: provided nullptr for script registration.";
        throw std::runtime_error(s.c_str());
    }

    _scripts[script->id] = script;
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
    if (inputProcessor == nullptr)
    {
        std::string s = "Scene: provided nullptr for input processor registration.";
        throw std::runtime_error(s.c_str());
    }
    _inputProcessors[inputProcessor->id] = inputProcessor;
}

void Scene::detachInputProcessor(unsigned int id)
{
    _inputProcessors.erase(id);
}

void Scene::registerInputProcessingScript(InputProcessingScriptPtr script)
{
    if (script == nullptr)
    {
        std::string s = "Scene: provided nullptr for input processing script registration.";
        throw std::runtime_error(s.c_str());
    }
    
    // Register the input processing script both as a script and an input processor
    ScriptPtr baseScriptPtr = std::static_pointer_cast<Script>(script);
    InputProcessorPtr baseIpPtr = std::static_pointer_cast<InputProcessor>(script);
    registerScript(baseScriptPtr);
    registerInputProcessor(baseIpPtr);
}

void Scene::detachInputProcessingScript(InputProcessingScriptPtr script)
{
    if (script == nullptr)
    {
        std::string s = "Scene: provided nullptr for input processing script deletion.";
        throw std::runtime_error(s.c_str());
    }

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
    if (!updateNode->value)
    {
        _outdatedTransformNodes++;
        updateNode->value = true;
    }
}

void Scene::worldTransformDFSUpdate(unsigned int startingId)
{
    SceneObjectMetadata meta = _objectMetadata[startingId];

    if (_updateMarkers[meta.updateNodeId]->value)
    {
        // If the current object is marked for update, update all transforms downwards
        worldTransformCascadeUpdate(startingId);
    }
    else
    {
        // Otherwise, run the DFS routine on each of its children
        ObjectTree::NodePtr object = _objects[meta.objectNodeId];
        std::vector<ObjectTree::NodeWPtr> children = object->getChildren();

        for (auto it = children.begin(); it != children.end(); it++)
        {
            SceneObjectPtr child = it->lock()->value;
            worldTransformDFSUpdate(child->id);
        }
    }
}

std::vector<unsigned int> Scene::findLongestOutdatedParentChain(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve the object node ID, the node pointer as well as its parent chain
    SceneObjectMetadata meta = it->second;
    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    std::vector<ObjectTree::NodeWPtr> parentChain = objectNode->getParentChain();

    std::vector<unsigned int> parentIdChain;
    parentIdChain.reserve(parentChain.size());

    // Find the count of elements in the chain up until the last outdated element
    unsigned int count = 0;
    unsigned int outdatedCount = 0;
    for (auto parentIt = parentChain.begin(); parentIt != parentChain.end(); parentIt++)
    {
        count++;

        unsigned int id = parentIt->lock()->value->id;
        parentIdChain.push_back(id);
        SceneObjectMetadata meta = _objectMetadata[id];

        // Use the flag to check on the modified state of the object transform
        if (_updateMarkers[meta.updateNodeId]->value)
        {
            // Keep track of the element count up until the last outdated element
            outdatedCount = count;
        }
    }

    // If no outdated parents, return empty vector
    if (outdatedCount == 0) return std::vector<unsigned int>();

    // If last parent is outdated, return whole ID vector
    if (outdatedCount == parentChain.size()) return parentIdChain;

    // Otherwise, copy the outdated element IDs into a new vector and return it
    std::vector<unsigned int> outdatedChain;
    outdatedChain.reserve(outdatedCount);
    std::copy(parentIdChain.begin(), parentIdChain.begin() + outdatedCount, std::back_inserter(outdatedChain));

    return outdatedChain;
}

void Scene::worldTransformCascadeUpdate(unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve node metadata
    SceneObjectMetadata meta = it->second;

    // Get object node pointer
    ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    worldTransformUpdateNoCascade(id);
    
    // Reverberate changes to children transforms
    std::vector<ObjectTree::NodeWPtr> children = objectNode->getChildren();
    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        ObjectTree::NodePtr child = childIt->lock();
        worldTransformCascadeUpdate(child->value->id);
    }
}

void Scene::worldTransformUpdateNoCascade(unsigned int id)
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
        Transform newTransform = ((Transform)(objectNode->value->transform)).applyOver(parentTransformNode->value);
        transformNode->value = newTransform;
    }
    else
    {
        transformNode->value = (Transform)objectNode->value->transform;
    }

    // Reset the update marker
    if (_updateMarkers[meta.updateNodeId]->value)
    {
        _updateMarkers[meta.updateNodeId]->value = false;
        _outdatedTransformNodes--;
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
