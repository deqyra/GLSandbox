#include "scene.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <renderboi/utilities/to_string.hpp>

#include <cpptools/tree.hpp>

#include "../factory.hpp"
#include "../script.hpp"
#include "scene_object.hpp"

namespace Renderboi
{

// As this->shared_from_this cannot be called until the Scene is fully 
// constructed, the object graph is initialized with a null shared pointer, 
// hence why this->init needs to be called immediately thereafter in order to 
// properly initialize the root node with a shared pointer to this.
Scene::Scene() :
    _objects(SceneObjectPtr()),
    _transforms(Transform()),
    _updateMarkers(false),
    _outdatedTransformNodes(0),
    _objectMetadata(),
    _scripts(),
    _lastTime(std::chrono::system_clock::now())
{
    
}

SceneObjectPtr Scene::operator[](const unsigned int id)
{
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);
    const ObjectTree::NodePtr node = _objects[meta.objectNodeId];
    return node->value;
}

SceneObjectPtr Scene::newObject(const std::string name)
{
    const ObjectTree::NodePtr node = _objects.getRoot();
    const unsigned int rootId = node->value->id;

    const SceneObjectPtr object = Factory::MakeSceneObject(name);

    _performObjectRegistration(object, _objectMetadata[rootId]);

    return object;
}

SceneObjectPtr Scene::newObject(const unsigned int parentId)
{
    const SceneObjectPtr object = Factory::MakeSceneObject();
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(parentId);
 
    _performObjectRegistration(object, meta);

    return object;
}

SceneObjectPtr Scene::newObject(const std::string name, const unsigned int parentId)
{
    const SceneObjectPtr object = Factory::MakeSceneObject(name);
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(parentId);
 
    _performObjectRegistration(object, meta);

    return object;
}

void Scene::registerObject(const SceneObjectPtr object)
{
    _checkNotNullOrThrow(object);
    _verifyNoParentSceneOrThrow(object);

    const ObjectTree::NodePtr node = _objects.getRoot();
    const unsigned int rootId = node->value->id;
    // Register object as a root child
    _performObjectRegistration(object, _objectMetadata[rootId]);
}

void Scene::registerObject(const SceneObjectPtr object, const unsigned int parentId)
{
    _checkNotNullOrThrow(object);
    _verifyNoParentSceneOrThrow(object);
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(parentId);
 
    _performObjectRegistration(object, meta);
}

void Scene::removeObject(const unsigned int id)
{
    // Retrieve object metadata
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);

    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> children = objectNode->getChildren();

    // Recursively remove all children before carrying on with the object at hand
    for (const auto child : children)
    {
        removeObject(child->value->id);
    }

    // Within the markers about to be removed, subtract the count of those set 
    // to true from the outdated transform count
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

void Scene::moveObject(const unsigned int id, const unsigned int newParentId, const bool worldPositionStays)
{
    // Retrieve IDs of both nodes in all graphs
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);
    const SceneObjectMetadata parentMeta = _findObjectMetaOrThrow(id);

    // Fetch the world transform of moved object, updating if necessary
    const Transform worldTransform = getWorldTransform(id);

    // Move nodes in all graphs
    _objects.moveBranch(meta.objectNodeId, parentMeta.objectNodeId);
    _transforms.moveBranch(meta.transformNodeId, parentMeta.transformNodeId);
    _updateMarkers.moveBranch(meta.updateNodeId, parentMeta.updateNodeId);

    if (worldPositionStays)
    {
        // Get the world transform of the new parent, not cascading the update
        const Transform parentTransform = getWorldTransform(newParentId, false);
        _objects[meta.objectNodeId]->value->transform = worldTransform.compoundFrom(parentTransform);
        // No need to mark the object for update as it stayed in place
    }
    // Mark object for update if it did not keep its world position
    else _markForUpdate(id);
}

unsigned int Scene::getParentId(const unsigned int id) const
{
    // Retrieve object metadata
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);
    return meta.parentId;
}

SceneObjectPtr Scene::getParent(const unsigned int id) const
{
    // Retrieve object metadata
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);
    return _objects[meta.parentId]->value;
}

void Scene::updateAllTransforms()
{
    if (_outdatedTransformNodes)
    {
        const ObjectTree::NodePtr objectRootNode = _objects.getRoot();
        const std::vector<ObjectTree::NodePtr> childNodes = objectRootNode->getChildren();

        // Start the DFS routine on each of the root children
        for(auto it = childNodes.begin(); it != childNodes.end(); it++)
        {
            SceneObjectPtr object = (*it)->value;
            _worldTransformDFSUpdate(object->id);
        }

        _outdatedTransformNodes = 0;
    }
}

Transform Scene::getWorldTransform(const unsigned int id, const bool cascadeUpdate) const
{
    const SceneObjectMetadata meta = _findObjectMetaOrThrow(id);

    // Update transform if required
    if (_outdatedTransformNodes)
    {
        // Find the longest outdated parent
        const std::vector<unsigned int> outdatedIds = _findLongestOutdatedParentChain(id);

        // If no outdated parent was found, the vector is empty
        if (outdatedIds.size() == 0)
        {
            // If the update marker of the considered object is set, update its transform
            if (_updateMarkers[meta.updateNodeId]->value)
            {
                // Update it along with all of its children's if appropriate
                if (cascadeUpdate) _worldTransformCascadeUpdate(id);
                else _worldTransformUpdateNoCascade(id);
            }

            // If it isn't set, the transform of that object is up to date and no action is required
        }
        // If an outdated parent chain was found
        else
        {
            // Update the furthest parent, cascading, if appropriate
            if (cascadeUpdate) _worldTransformCascadeUpdate(outdatedIds.back());
            else
            {
                // Otherwise, update each element in the chain from the top, not cascading
                for (auto it = outdatedIds.rbegin(); it != outdatedIds.rend(); it++)
                {
                    _worldTransformUpdateNoCascade(*it);
                }
            }
        }
    }

    // Retrieve matrix graph node ID and return matrix
    const TransformTree::NodePtr node = _transforms[meta.transformNodeId];
    return node->value;
}

std::vector<SceneObjectPtr> Scene::getAllObjects(const bool mustBeEnabled) const
{
    std::vector<SceneObjectPtr> result;

    for (const auto& [_, meta] : _objectMetadata)
    {
        const ObjectTree::NodePtr node = _objects[meta.objectNodeId];
        if (!mustBeEnabled || node->value->enabled) result.push_back(node->value);
    }

    return result;
}

void Scene::registerScript(const ScriptPtr script)
{
    if (script == nullptr)
    {
        throw std::runtime_error("Scene: provided nullptr for script registration.");
    }

    _scripts[script->id] = script;
}

void Scene::detachScript(const unsigned int id)
{
    _scripts.erase(id);
}

void Scene::triggerUpdate()
{
    // Get time delta (in seconds) and upate last update time
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::chrono::duration<double> delta = now - _lastTime;
    _lastTime = now;

    // Update all registered scripts
    for (const auto& [_, script] : _scripts)
    {
        script->update((float)(delta.count()));
    }
}
void Scene::_init()
{
    const ObjectTree::NodePtr objectRootNode = _objects.getRoot();
    const TransformTree::NodePtr transformRootNode = _transforms.getRoot();
    const BoolTree::NodePtr updateRootNode = _updateMarkers.getRoot();

    // Reset the object graph root node pointer to a scene object initialized with a pointer to this Scene.
    objectRootNode->value.reset(new SceneObject("SCENE_ROOT"));
    objectRootNode->value->setScene(this->shared_from_this());

    // Set and map metadata
    const SceneObjectMetadata meta = {
        objectRootNode->value->id,  // ID of the object this metadata refers to
        _MaxUInt,                    // ID of the object which is parent to the object this metadata refers to
        objectRootNode->id,         // ID of the graph node containing the object
        transformRootNode->id,      // ID of the graph node containing the world matrix of the object
        updateRootNode->id,         // ID of the graph node containing the update flag of the object
        _MaxUInt                     // ID of the subscription to the transform notifier of the object
    };
    _objectMetadata[meta.id] = meta;
}

void Scene::_terminate()
{
    // Remove scene references in all scene objects, unsubscribe scene from object updates
    for (const auto& [_, meta] : _objectMetadata)
    {
        _objects[meta.id]->value->setScene(nullptr);
        _objects[meta.id]->value->transform.getNotifier().deleteSubscriber(meta.transformSubscriberId);
    }

    // Clear all trees
    _objects.clear();
    _transforms.clear();
    _updateMarkers.clear();

    // Clear metadata, scripts, inputProcessors
    _objectMetadata.clear();
    _scripts.clear();
}

void Scene::_objectTransformModified(const unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: the object transform callback was called with non-existing ID " + std::to_string(id) + "!";
        throw std::runtime_error(s.c_str());
    }

    _markForUpdate(id);
}

void Scene::_markForUpdate(const unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve object metadata
    const SceneObjectMetadata& meta = it->second;
    const BoolTree::NodePtr updateNode = _updateMarkers[meta.updateNodeId];
    // Set the marker to true
    if (!updateNode->value)
    {
        _outdatedTransformNodes++;
        updateNode->value = true;
    }
}

void Scene::_checkNotNullOrThrow(const SceneObjectPtr object) const
{
    if (!object)
    {
        throw std::runtime_error("Scene: null SceneObject pointer was provided.");
    }
}

void Scene::_verifyNoParentSceneOrThrow(const SceneObjectPtr object) const
{
    ScenePtr objectScene = object->getScene();
    if (objectScene == this->shared_from_this())
    {
        const std::string s = "Scene: SceneObject with ID " + std::to_string(object->id)
            + " is already registered to this scene.";

        throw std::runtime_error(s.c_str());
    }

    if (objectScene != nullptr)
    {
        const std::string s = "Scene: SceneObject with ID " + std::to_string(object->id) 
            + " already has a parent scene.";

        throw std::runtime_error(s.c_str());
    }
}

SceneObjectMetadata Scene::_findObjectMetaOrThrow(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        const std::string s = "Scene: no SceneObject with ID " + std::to_string(id) 
            + ", cannot retrieve world transform.";

        throw std::runtime_error(s.c_str());
    }

    return it->second;
}

void Scene::_performObjectRegistration(const SceneObjectPtr object, const SceneObjectMetadata& parentMeta)
{
    // Set the scene pointer upon registering the object
    object->setScene(this->shared_from_this());

    const Transform parentTransform = _transforms[parentMeta.transformNodeId]->value;
    // Get transform of new object and apply parent transform to it
    Transform newTransform = (Transform)object->transform;
    newTransform = newTransform.applyOver(parentTransform);

    // Create nodes in graphs, using parent node IDs from retrieved metadata
    const unsigned int objectNodeId = _objects.addNode(object, parentMeta.objectNodeId);
    const unsigned int transformNodeId = _transforms.addNode(newTransform, parentMeta.transformNodeId);
    const unsigned int updateNodeId = _updateMarkers.addNode(false, parentMeta.updateNodeId);

    // Hook up the notification receiver of the scene with the notifier of the object transform
    std::function<void(const unsigned int&)> callback = [this](const unsigned int& id)
    {
        this->_objectTransformModified(id);
    };
    const unsigned int transformSubscriberId = object->transform.getNotifier().addSubscriber(callback);

    // Create metadata
    const SceneObjectMetadata meta = {
        object->id,             // ID of the object this metadata refers to
        parentMeta.id,          // ID of the object which is parent to the object this metadata refers to
        objectNodeId,           // ID of the graph node containing the object
        transformNodeId,        // ID of the graph node containing the world matrix of the object
        updateNodeId,           // ID of the graph node containing the update flag of the object
        transformSubscriberId   // ID of the subscription to the transform notifier of the object
    };
    _objectMetadata[meta.id] = meta;
}

void Scene::_worldTransformDFSUpdate(const unsigned int startingId) const
{
    const SceneObjectMetadata meta = _objectMetadata.at(startingId);

    if (_updateMarkers[meta.updateNodeId]->value)
    {
        // If the current object is marked for update, update all transforms downwards
        _worldTransformCascadeUpdate(startingId);
    }
    else
    {
        // Otherwise, run the DFS routine on each of its children
        const ObjectTree::NodePtr object = _objects[meta.objectNodeId];
        const std::vector<ObjectTree::NodePtr> children = object->getChildren();

        for (const auto& child : children)
        {
            _worldTransformDFSUpdate(child->id);
        }
    }
}

std::vector<unsigned int> Scene::_findLongestOutdatedParentChain(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve the object node ID, the node pointer as well as its parent chain
    const SceneObjectMetadata& meta = it->second;
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> parentChain = objectNode->getParentChain();

    std::vector<unsigned int> parentIdChain;
    parentIdChain.reserve(parentChain.size());

    // Find the count of elements in the chain up until the last outdated element
    unsigned int count = 0;
    unsigned int outdatedCount = 0;
    for (const auto& parent : parentChain)
    {
        count++;

        const unsigned int id = parent->value->id;
        parentIdChain.push_back(id);
        const SceneObjectMetadata meta = _objectMetadata.at(id);

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

void Scene::_worldTransformCascadeUpdate(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve node metadata
    const SceneObjectMetadata& meta = it->second;

    // Get object node pointer
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    _worldTransformUpdateNoCascade(id);
    
    // Reverberate changes to children transforms
    const std::vector<ObjectTree::NodePtr> children = objectNode->getChildren();
    for (const auto child : children)
    {
        _worldTransformCascadeUpdate(child->value->id);
    }
}

void Scene::_worldTransformUpdateNoCascade(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    const SceneObjectMetadata& meta = it->second;

    // Get object node pointer as well as the parent transform node pointer
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const TransformTree::NodePtr transformNode = _transforms[meta.transformNodeId];
    const TransformTree::NodePtr parentTransformNode = transformNode->getParent();

    if (parentTransformNode != nullptr)
    {
        // Apply the parent world transform to the object transform, and save it to the object world transform node
        const Transform newTransform = ((Transform)(objectNode->value->transform)).applyOver(parentTransformNode->value);
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

bool Scene::_hasDisabledParent(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve graph node IDs, object node pointer and node parent chain
    const SceneObjectMetadata meta = it->second;
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> parents = objectNode->getParentChain();

    for (const auto parent : parents)
    {
        // Return whether any parent in the chain is disabled
        if (!parent->value->enabled) return true;
    }
    return false;
}

}//namespace Renderboi
