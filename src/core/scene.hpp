#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../tools/tree.hpp"
#include "scene/scene_object.hpp"

class Scene : public std::enable_shared_from_this<Scene>
{
    public:
        using ObjPtr = std::shared_ptr<SceneObject>;
        using WeakObjPtr = std::weak_ptr<SceneObject>;
        using ObjTree = Tree<ObjPtr>;
        using MatTree = Tree<glm::mat4>;

    private:
        ObjTree _graph;
        MatTree _modelMatrices;
        std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> _objectIdsToNodeIds;

        void processOutdatedTransformsFromNode(unsigned int id);
        void recalculateModelMatrix(unsigned int id);
        void removeIdMapping(unsigned int id);
        bool hasDisabledParent(unsigned int id);

    public:
        Scene();

        WeakObjPtr operator[](unsigned int id);
        WeakObjPtr newObject();
        WeakObjPtr newObject(unsigned int parentId);
        void registerObject(ObjPtr object, unsigned int parentId);
        void registerObject(ObjPtr object);
        void removeObject(unsigned int id);
        void moveObject(unsigned int id, unsigned int newParentId);
        glm::mat4 getWorldModelMatrix(unsigned int id);

        std::vector<WeakObjPtr> getAllObjects();

        template<class T>
        std::vector<WeakObjPtr> getObjectsWithComponent(bool mustBeEnabled = true);
};

template<class T>
std::vector<Scene::WeakObjPtr> Scene::getObjectsWithComponent(bool mustBeEnabled)
{
    std::vector<WeakObjPtr> all = getAllObjects();
    std::vector<WeakObjPtr> result;

    std::function<bool(WeakObjPtr)> componentChecker = [this, mustBeEnabled](WeakObjPtr obj)
    {
        auto strongObj = obj.lock();
        if (mustBeEnabled && !strongObj->enabled) return false;
        if (mustBeEnabled && hasDisabledParent(strongObj->id)) return false;
        return strongObj->hasComponent<T>();
    };

    std::copy_if(all.begin(), all.end(), std::back_inserter(result), componentChecker);

    return result;
}

#endif//SCENE_HPP