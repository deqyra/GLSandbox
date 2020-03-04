#include "scene.hpp"

Scene::Scene() :
    InputProcessor(),
    _graph(SceneObjectPtr()),
    _modelMatrices(glm::mat4(1.f)),
    _objectIdsToNodeIds()
{
}

unsigned int Scene::init()
{
    auto rootNode = _graph.getRoot().lock();
    auto matRootNode = _modelMatrices.getRoot().lock();

    rootNode->value.reset(new SceneObject(this->shared_from_this()));

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

    auto pair = it->second;
    auto node = _graph[pair.first].lock();
    return node->value;
}

SceneObjectWPtr Scene::newObject()
{
    auto node = _graph.getRoot().lock();
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

    auto pair = it->second;

    SceneObjectPtr obj = std::make_shared<SceneObject>(this->shared_from_this());
    unsigned int objHandle = _graph.addNode(obj, pair.first);
    unsigned int matHandle = _modelMatrices.addNode(glm::mat4(1.f), pair.second);
    _objectIdsToNodeIds[obj->id] = { objHandle, matHandle };

    return obj;
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
    object->setScene(this->weak_from_this());

    auto pair = it->second;
    unsigned int graphNodeId = _graph.addNode(object, pair.first);
    unsigned int matrixNodeId = _modelMatrices.addNode(glm::mat4(1.f), pair.second);

    _objectIdsToNodeIds[object->id] = { graphNodeId, matrixNodeId };
    recalculateModelMatrix(object->id);
}

void Scene::registerObject(SceneObjectPtr object)
{
    auto node = _graph.getRoot().lock();
    registerObject(object, node->value->id);
}

void Scene::removeObject(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    if (it == _objectIdsToNodeIds.end())
    {
        std::string s = "Scene: no SceneObject with ID " + std::to_string(id) + ", cannot remove object.";
        throw std::runtime_error(s.c_str());
    }

    auto pair = it->second;
    _graph.removeBranch(pair.first);
    _modelMatrices.removeBranch(pair.second);

    removeIdMapping(id);
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

    auto objectPair = objectIt->second;
    auto parentPair = parentIt->second;

    _graph.moveBranch(objectPair.first, parentPair.first);
    _modelMatrices.moveBranch(objectPair.second, parentPair.second);
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

    processOutdatedTransformsFromNode(id);

    auto pair = it->second;
    auto node = _modelMatrices[pair.second].lock();
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

    processOutdatedTransformsFromNode(id);

    auto pair = it->second;
    glm::mat4 model = _modelMatrices[pair.second].lock()->value;
    glm::vec4 position = model * glm::vec4(glm::vec3(0.f), 1.f);
    return glm::vec3(position);
}

std::vector<SceneObjectWPtr> Scene::getAllObjects()
{
    std::vector<SceneObjectWPtr> result;

    for (auto it = _objectIdsToNodeIds.begin(); it != _objectIdsToNodeIds.end(); it++)
    {
        auto pair = it->second;
        auto node = _graph[pair.first].lock();
        result.push_back(node->value);
    }

    return result;
}

void Scene::registerScript(ScriptPtr script)
{
    _scripts[script->id] = script;
}

void Scene::removeScript(ScriptPtr script)
{
    _scripts.erase(script->id);
}

void Scene::removeScript(unsigned int id)
{
    _scripts.erase(id);
}

void Scene::triggerUpdate()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> delta = now - _lastTime;
    _lastTime = now;

    for (auto it = _scripts.begin(); it != _scripts.end(); it++)
    {
        it->second->update((float)(delta.count()));
    }
}

void Scene::registerInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessors[inputProcessor->id] = inputProcessor;
}

void Scene::removeInputProcessor(InputProcessorPtr inputProcessor)
{
    _inputProcessors.erase(inputProcessor->id);
}

void Scene::removeInputProcessor(unsigned int id)
{
    _inputProcessors.erase(id);
}

void Scene::processOutdatedTransformsFromNode(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    auto pair = it->second;

    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    std::vector<ObjTree::NodeWPtr> parentChain = objNode->getParentChain();

    bool outdated = false;
    unsigned int uppermostOutdatedId;
    for (auto it = parentChain.begin(); it != parentChain.end(); it++)
    {
        SceneObjectPtr obj = it->lock()->value;
        if (obj->transformModifiedFlagState())
        {
            obj->resetTransformModifiedFlag();
            uppermostOutdatedId = obj->id;
            outdated = true;
        }
    }

    if (outdated)
    {
        recalculateModelMatrix(uppermostOutdatedId);
    }
    else if (objNode->value->transformModifiedFlagState())
    {
        objNode->value->resetTransformModifiedFlag();
        recalculateModelMatrix(id);
    }
}

void Scene::recalculateModelMatrix(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    auto pair = it->second;

    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    MatTree::NodePtr matNode = _modelMatrices[pair.second].lock();
    MatTree::NodePtr parentMatNode = matNode->getParent().lock();

    glm::mat4 localTransform = objNode->value->getModelMatrix();
    glm::mat4 worldTransform = glm::mat4(1.f);
    if (parentMatNode.get() != nullptr) worldTransform = parentMatNode->value;

    matNode->value = worldTransform * localTransform;

    auto children = objNode->getChildren();
    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        auto strongChild = childIt->lock();
        recalculateModelMatrix(strongChild->id);
    }
}

void Scene::removeIdMapping(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    auto pair = it->second;

    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    auto children = objNode->getChildren();
    for (auto childIt = children.begin(); childIt != children.end(); childIt++)
    {
        auto strongChild = childIt->lock();
        removeIdMapping(strongChild->id);
    }

    _objectIdsToNodeIds.erase(id);
}

bool Scene::hasDisabledParent(unsigned int id)
{
    auto it = _objectIdsToNodeIds.find(id);
    auto pair = it->second;

    ObjTree::NodePtr objNode = _graph[pair.first].lock();
    std::vector<ObjTree::NodeWPtr> parents = objNode->getParentChain();
    for (auto it = parents.begin(); it != parents.end(); it++)
    {
        auto strongParent = it->lock();
        if (!strongParent->value->enabled) return true;
    }
    return false;
}

void Scene::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->framebufferResizeCallback(window, width, height);
    }
}

void Scene::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->keyboardCallback(window, key, scancode, action, mods);
    }
}

void Scene::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->mouseButtonCallback(window, button, action, mods);
    }
}

void Scene::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    for (auto it = _inputProcessors.begin(); it != _inputProcessors.end(); it++)
    {
        it->second->mouseCursorCallback(window, xpos, ypos);
    }
}
