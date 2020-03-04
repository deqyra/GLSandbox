#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "scene/scene_types_decl.hpp"
#include "scene/scene_object.hpp"
#include "scene/script.hpp"
#include "../tools/tree.hpp"
#include "../tools/input_processor.hpp"

class Scene : public InputProcessor, public std::enable_shared_from_this<Scene>
{
    public:
        using ObjTree = Tree<SceneObjectPtr>;
        using MatTree = Tree<glm::mat4>;

    private:
        ObjTree _graph;
        MatTree _modelMatrices;
        std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> _objectIdsToNodeIds;
        std::unordered_map<unsigned int, ScriptPtr> _scripts;
        std::unordered_map<unsigned int, InputProcessorPtr> _inputProcessors;

        void processOutdatedTransformsFromNode(unsigned int id);
        void recalculateModelMatrix(unsigned int id);
        void removeIdMapping(unsigned int id);
        bool hasDisabledParent(unsigned int id);

    public:
        Scene();
        unsigned int init();

        SceneObjectWPtr operator[](unsigned int id);
        SceneObjectWPtr newObject();
        SceneObjectWPtr newObject(unsigned int parentId);
        void registerObject(SceneObjectPtr object, unsigned int parentId);
        void registerObject(SceneObjectPtr object);
        void removeObject(unsigned int id);
        void moveObject(unsigned int id, unsigned int newParentId);
        glm::mat4 getWorldModelMatrix(unsigned int id);
        glm::vec3 getWorldPosition(unsigned int id);

        std::vector<SceneObjectWPtr> getAllObjects();

        void registerScript(ScriptPtr script);
        void removeScript(ScriptPtr script);
        void removeScript(unsigned int id);
        void triggerUpdate();

        void registerInputProcessor(InputProcessorPtr inputProcessor);
        void removeInputProcessor(InputProcessorPtr inputProcessor);
        void removeInputProcessor(unsigned int id);

        virtual void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        virtual void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

        template<class T>
        std::vector<SceneObjectWPtr> getObjectsWithComponent(bool mustBeEnabled = true);
};

template<class T>
std::vector<SceneObjectWPtr> Scene::getObjectsWithComponent(bool mustBeEnabled)
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