#include "scene.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "scene_object.hpp"
#include "script.hpp"
#include "input_processing_script.hpp"

#include "../../tools/tree.hpp"
#include "../../tools/input_processor.hpp"

// As this->shared_from_this cannot be called until the Scene is fully constructed, the object graph is initialized with a null weak pointer,
// hence why this->init needs to be called immediately thereafter in order to properly initialize the root node with a shared pointer to this.
Scene::Scene() :
    InputProcessor(),
    _graph(SceneObjectPtr()),
    _modelMatrices(glm::mat4(1.f)),
    _objectIdsToNodeIds()
{
    _lastTime = std::chrono::system_clock::now();
}

unsigned int Scene::init()
{
    ObjTree::NodePtr rootNode = _graph.getRoot().lock();
    MatTree::NodePtr matRootNode = _modelMatrices.getRoot().lock();

    // Reset the object graph root node pointer to a scene object initialized with a pointer to this Scene.
    rootNode->value.reset(new SceneObject(this->shared_from_this()));

    // Set the graph nodes ID mappings
    unsigned int rootObjId = rootNode->value->id;
    _objectIdsToNodeIds[rootObjId] = { rootNode->id, matRootNode->id };
    
    return rootObjId;
}

SceneObjectWPtr Scene::operator[](unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve the node ID in the object graph
    std::pair<unsigned int, unsigned int> pair = it->second;
    // Retrieve the node
    ObjTree::NodePtr node = _graph[pair.first].lock();
    return node->value;
}

SceneObjectWPtr Scene::newObject()
{
    ObjTree::NodePtr node = _graph.getRoot().lock();
    // Create the new object as a child of the root node
    return newObject(node->value->id);
}

SceneObjectWPtr Scene::newObject(unsigned int parentId)
{
    auto it = _objectIdsToNodeIds.find(parentId);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(parentId) + ", cannot create object as child.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve the parent node IDs in the graphs
    std::pair<unsigned int, unsigned int> pair = it->second;

    // Create the new object, initialized with a pointer to this Scene
    SceneObjectPtr obj = std::make_shared<SceneObject>(this->shared_from_this());
    // Create nodes in graphs, using retrieved parent node IDs
    unsigned int objNodeId = _graph.addNode(obj, pair.first);
    unsigned int matNodeId = _modelMatrices.addNode(glm::mat4(1.f), pair.second);
    // Map IDs
    _objectIdsToNodeIds[obj->id] = { objNodeId, matNodeId };

    return obj;
}

void Scene::registerObject(SceneObjectPtr object)
{
    ObjTree::NodePtr node = _graph.getRoot().lock();
    // Register object as a root child
    registerObject(object, node->value->id);
}

void Scene::registerObject(SceneObjectPtr object, unsigned int parentId)
{
    auto it = _objectIdsToNodeIds.find(parentId);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(parentId) + ", cannot register new object as one of its children.";
        throw std::runtime_error(s.c_str());
    }

    ScenePtr objectScene = object->getScene().lock();
    if (objectScene != nullptr && objectScene != this->shared_from_this())
    {
        std::string s = "Scene: SceneObject with ID " + std::to_string(object->id) + " already has a parent Scene, cannot register new object.";
        throw std::runtime_error(s.c_str());
    }
    // Set the scene pointer upon registering the object
    object->setScene(this->weak_from_this());

    // Retrieve the parent node IDs in the graphs
    std::pair<unsigned int, unsigned int> pair = it->second;
    // Create nodes in graphs, using retrieved parent node IDs
    unsigned int graphNodeId = _graph.addNode(object, pair.first);
    unsigned int matrixNodeId = _modelMatrices.addNode(glm::mat4(1.f), pair.second);

    // Map IDs
    _objectIdsToNodeIds[object->id] = { graphNodeId, matrixNodeId };
    // Update world model matrix
    recalculateModelMatrix(object->id);
}

void Scene::removeObject(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot remove object.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve the parent node IDs in the graphs
    std::pair<unsigned int, unsigned int> pair = it->second;
    // Remove nodes in graphs, using retrieved parent node IDs
    _graph.removeBranch(pair.first);
    _modelMatrices.removeBranch(pair.second);

    removeIdMappings(id);
}

void Scene::moveObject(unsigned int id, unsigned int newParentId)
{
    auto objectIt = _objectIdsToNodeIds.find(id);
    if (objectIt == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot move object.";
        throw std::runtime_error(s.c_str());
    }

    auto parentIt = _objectIdsToNodeIds.find(newParentId);
    if (parentIt == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(newParentId) + ", cannot move object " + std::to_string(id) + " to it.";
        throw std::runtime_error(s.c_str());
    }

    // Retrieve IDs of both nodes in both graphs
    std::pair<unsigned int, unsigned int> objectPair = objectIt->second;
    std::pair<unsigned int, unsigned int> parentPair = parentIt->second;

    // Move nodes in both graphs
    _graph.moveBranch(objectPair.first, parentPair.first);
    _modelMatrices.moveBranch(objectPair.second, parentPair.second);
    // Update transforms
    recalculateModelMatrix(id);
}

glm::mat4 Scene::getWorldModelMatrix(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot retrieve model matrix.";
        throw std::runtime_error(s.c_str());
    }

    // Update transform if required
    processOutdatedTransformsFromNode(id);

    // Retrieve matrix graph node ID and return matrix
    std::pair<unsigned int, unsigned int> pair = it->second;
    MatTree::NodePtr node = _modelMatrices[pair.second].lock();
    return node->value;
}

glm::vec3 Scene::getWorldPosition(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot compute world position.";
        throw std::runtime_error(s.c_str());
    }

    // Update transform if required
    processOutdatedTransformsFromNode(id);

    // Retrieve matrix graph node ID and return matrix
    std::pair<unsigned int, unsigned int> pair = it->second;
    // Retrieve matrix, return it translation component (which is what a null homogenous vector multiplied by the whole model matrix would be equal to)
    glm::mat4 model = _modelMatrices[pair.second].lock()->value;
    return glm::vec3(model[3]);
}

std::vector<SceneObjectWPtr> Scene::getAllObjects()
{
    std::vector<SceneObjectWPtr> result;

    for (auto it = _objectIdsToNodeIds.begin(); it != _objectIdsToNodeIds.end(); it++)
    {
        std::pair<unsigned int, unsigned int> pair = it->second;
        ObjTree::NodePtr node = _graph[pair.first].lock();
        result.push_back(node->value);
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
    // Get time delta and upate last update time
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

void Scene::processFramebufferResize(GLFWwindow* window, int width, int height)
{
    // Forward framebuffer resize event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processFramebufferResize(window, width, height);
    }
}

void Scene::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Forward keyboard event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processKeyboard(window, key, scancode, action, mods);
    }
}

void Scene::processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    // Forward mouse button event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processMouseButton(window, button, action, mods);
    }
}

void Scene::processMouseCursor(GLFWwindow* window, double xpos, double ypos)
{
    // Forward mouse cursor event to all input processors
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->processMouseCursor(window, xpos, ypos);
    }
}

void Scene::processOutdatedTransformsFromNode(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    // Retrieve the object node ID, the node pointer as well as its parent chain
    std::pair<unsigned int, unsigned int> pair = it->second;
    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    std::vector<ObjTree::NodeWPtr> parentChain = objNode->getParentChain();

    // Check whether a transform is outdated in the node's parent chain
    bool outdated = false;
    // Keep track of the furthest parent in the chain whose transform was outdated
    unsigned int uppermostOutdatedId;
    for (auto parentIt = parentChain.begin(); parentIt != parentChain.end(); parentIt++)
    {
        SceneObjectPtr obj = parentIt->lock()->value;
        // Use the flag to check on the modified state of the object transform
        if (obj->transformModifiedFlagState())
        {
            obj->resetTransformModifiedFlag();
            uppermostOutdatedId = obj->id;
            outdated = true;
        }
    }

    // Recalculate the furthest parent's model matrix (which will cascade to all of its children as well)
    if (outdated)
    {
        recalculateModelMatrix(uppermostOutdatedId);
    }
    // If no parent was updated just update the node transform if appropriate
    else if (objNode->value->transformModifiedFlagState())
    {
        objNode->value->resetTransformModifiedFlag();
        recalculateModelMatrix(id);
    }
}

void Scene::recalculateModelMatrix(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    // Retrieve graph node IDs
    std::pair<unsigned int, unsigned int> pair = it->second;

    // Get graph node pointers as well as the parent matrix node pointer
    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    MatTree::NodePtr matNode = _modelMatrices[pair.second].lock();
    MatTree::NodePtr parentMatNode = matNode->getParent().lock();

    glm::mat4 localTransform = objNode->value->getModelMatrix();
    // The default world transform is a neutral transform (in case the parent matrix node is null)
    glm::mat4 worldTransform = glm::mat4(1.f);
    // Otherwise it is the parent transform
    if (parentMatNode.get() != nullptr) worldTransform = parentMatNode->value;

    // Compute the new world transform
    matNode->value = worldTransform * localTransform;

    // Reverberate changes to children transforms
    std::vector<ObjTree::NodeWPtr> children = objNode->getChildren();
    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        ObjTree::NodePtr child = childIt->lock();
        recalculateModelMatrix(child->id);
    }
}

void Scene::removeIdMappings(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    // Retrieve graph node IDs, object node pointer and node children pointers
    std::pair<unsigned int, unsigned int> pair = it->second;
    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    std::vector<ObjTree::NodeWPtr> children = objNode->getChildren();

    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        // Remove mappings for all children
        ObjTree::NodePtr child = childIt->lock();
        removeIdMappings(child->id);
    }

    // Remove the current object ID mapping
    _objectIdsToNodeIds.erase(id);
}

bool Scene::hasDisabledParent(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    // Retrieve graph node IDs, object node pointer and node parent chain
    std::pair<unsigned int, unsigned int> pair = it->second;
    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    std::vector<ObjTree::NodeWPtr> parents = objNode->getParentChain();

    for (auto it = parents.begin(); it != parents.end(); it++)
    {
        // Return whether any parent in the chain is disabled
        auto parent = it->lock();
        if (!parent->value->enabled) return true;
    }
    return false;
}
