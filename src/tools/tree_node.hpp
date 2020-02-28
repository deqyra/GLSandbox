#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

template<typename T>
class TreeNode;

template<typename T>
class TreeNode
{
    using TreeNodePtr = std::shared_ptr<TreeNode<T>>;

    private:
        TreeNode(const TreeNode& other) = delete;
        TreeNode<T>& operator=(const TreeNode& other) = delete;

        static unsigned int _count;

        TreeNodePtr  _parent;
        std::vector<TreeNodePtr > _children;

        std::vector<unsigned int> _parentIdChain;
        void generateParentChain();

    public:
        TreeNode(TreeNodePtr parent, T value);

        TreeNodePtr getParent();
        void setParent(TreeNodePtr parent);

        std::vector<TreeNodePtr> getChildren();
        void addChild(TreeNodePtr child);
        void removeChild(TreeNodePtr child);
        void removeChild(unsigned int id);
        bool hasChild(TreeNodePtr child);
        bool hasChild(unsigned int id);
        bool isParentOf(TreeNodePtr node);
        bool isChildOf(TreeNodePtr node);
        
        const unsigned int id;
        T value;
};

template<typename T>
unsigned int TreeNode<T>::_count = 0;

template<typename T>
TreeNode<T>::TreeNode(TreeNodePtr parent, T value) :
    id(_count++),
    _parent(parent),
    _children(),
    _parentIdChain(),
    value(value)
{

}

template<typename T>
TreeNode<T>::TreeNodePtr TreeNode<T>::getParent()
{
    return _parent;
}

template<typename T>
void TreeNode<T>::setParent(TreeNodePtr parent)
{
    if (parent == _parent) return;

    if (TreeNodePtr(this) == parent)
    {
        std::string s = "TreeNode: node cannot be its own parent.";
        throw std::runtime_error(s.c_str());
    }

    if (isParentOf(parent))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a parent of node " << static_cast<void*>(parent.get()) << " and cannot set it as its own parent.";
        throw std::runtime_error(sstr.str().c_str());
    }

    if (_parent != nullptr)
    {
        _parent->removeChild(id);
    }

    _parent = parent;

    if (_parent != nullptr)
    {
        _parent->_children.push_back(TreeNodePtr(this));
    }

    generateParentChain();
}

template<typename T>
std::vector<typename TreeNode<T>::TreeNodePtr> TreeNode<T>::getChildren()
{
    return _children;
}

template<typename T>
void TreeNode<T>::addChild(typename TreeNode<T>::TreeNodePtr child)
{
    if (child->_parent == TreeNodePtr(this)) return;

    if (child->_parent != nullptr)
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(child.get()) << " already has a parent.";
        throw std::runtime_error(sstr.str().c_str());
    }

    if (isChildOf(child))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a child of node " << static_cast<void*>(child.get()) << " and cannot set it as one of its own children.";
        throw std::runtime_error(sstr.str().c_str());
    }

    child->_parent = TreeNodePtr(this);
    child->generateParentChain();
    _children.push_back(child);
}

template<typename T>
bool TreeNode<T>::hasChild(TreeNodePtr child)
{
    return hasChild(child->id);
}

template<typename T>
bool TreeNode<T>::hasChild(unsigned int id)
{
    std::function<bool(TreeNodePtr)> checkId = [id](TreeNodePtr node)
    {
        return id == node->id;
    };

    auto it = std::find_if(_children.begin(), _children.end(), checkId);
    return it != _children.end();
}

template<typename T>
void TreeNode<T>::removeChild(TreeNodePtr child)
{
    removeChild(child->id);
}

template<typename T>
void TreeNode<T>::removeChild(unsigned int id)
{
    std::function<bool(TreeNodePtr)> checkId = [id](TreeNodePtr node)
    {
        return id == node->id;
    };

    auto it = std::find_if(_children.begin(), _children.end(), checkId);
    if (it != _children.end())
    {
        _children.erase(it);
    }
}

template<typename T>
bool TreeNode<T>::isParentOf(typename TreeNode<T>::TreeNodePtr node)
{
    if (node == nullptr) return false;

    return node->isChildOf(TreeNodePtr(this));
}

template<typename T>
bool TreeNode<T>::isChildOf(typename TreeNode<T>::TreeNodePtr node)
{
    if (node == nullptr) return false;

    auto it = std::find(_parentIdChain.begin(), _parentIdChain.end(), node->id);
    return it != _parentIdChain.end();
}

template<typename T>
void TreeNode<T>::generateParentChain()
{
    if (_parent != nullptr)
    {
        _parentIdChain = _parent->_parentIdChain;
        _parentIdChain.insert(_parentIdChain.begin(), _parent->id);
    }
    else
    {
        _parentIdChain = std::vector<unsigned int>();
    }
}

#endif//TREE_NODE_HPP