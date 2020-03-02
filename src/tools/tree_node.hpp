#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

template<typename T>
class TreeNode : public std::enable_shared_from_this<TreeNode<T>>
{
    using TreeNodePtr = std::weak_ptr<TreeNode<T>>;

    private:
        TreeNode(const TreeNode& other) = delete;
        TreeNode<T>& operator=(const TreeNode& other) = delete;

        static unsigned int _count;

        TreeNodePtr _parent;
        std::vector<TreeNodePtr> _children;

        std::vector<TreeNodePtr> _parentChain;
        std::vector<unsigned int> _parentIdChain;
        void generateParentChains();

    public:
        TreeNode(T value);

        TreeNodePtr getParent();
        void setParent(TreeNodePtr parent);
        std::vector<TreeNodePtr> getParentChain();
        std::vector<unsigned int> getParentIdChain();

        std::vector<TreeNodePtr> getChildren();
        void addChild(TreeNodePtr child);
        void removeChild(TreeNodePtr child);
        void removeChild(unsigned int id);
        bool hasChild(TreeNodePtr child);
        bool hasChild(unsigned int id);
        bool isParentOf(TreeNodePtr node);
        bool isChildOf(TreeNodePtr node);
        bool isChildOf(unsigned int id);
        
        const unsigned int id;
        T value;
};

template<typename T>
unsigned int TreeNode<T>::_count = 0;

template<typename T>
TreeNode<T>::TreeNode(T value) :
    id(_count++),
    _parent(),
    _children(),
    _parentChain(),
    _parentIdChain(),
    value(value)
{

}

template<typename T>
typename TreeNode<T>::TreeNodePtr TreeNode<T>::getParent()
{
    return _parent;
}

template<typename T>
void TreeNode<T>::setParent(TreeNodePtr newParent)
{
    auto strongNewParent = newParent.lock();
    auto strongParent = _parent.lock();

    if (strongNewParent == strongParent) return;

    if (strongNewParent.get() == this)
    {
        std::string s = "TreeNode: node cannot be its own parent.";
        throw std::runtime_error(s.c_str());
    }

    if (isParentOf(newParent))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a parent of node " << static_cast<void*>(strongNewParent.get()) << " and cannot set it as its own parent.";
        throw std::runtime_error(sstr.str().c_str());
    }

    if (strongParent != nullptr)
    {
        strongParent->removeChild(id);
    }

    if (strongNewParent != nullptr)
    {
        //_parent->_children.push_back(TreeNodePtr(this));
        strongNewParent->_children.push_back(this->weak_from_this());
    }
    _parent = newParent;

    generateParentChains();
}

template<typename T>
std::vector<typename TreeNode<T>::TreeNodePtr> TreeNode<T>::getParentChain()
{
    return _parentChain;
}

template<typename T>
std::vector<unsigned int> TreeNode<T>::getParentIdChain()
{
    return _parentIdChain;
}

template<typename T>
std::vector<typename TreeNode<T>::TreeNodePtr> TreeNode<T>::getChildren()
{
    return _children;
}

template<typename T>
void TreeNode<T>::addChild(typename TreeNode<T>::TreeNodePtr child)
{
    auto strongChild = child.lock();
    auto strongChildParent = strongChild->_parent.lock();

    if (strongChildParent.get() == this) return;

    if (strongChildParent != nullptr)
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(strongChild.get()) << " already has a parent.";
        throw std::runtime_error(sstr.str().c_str());
    }

    if (isChildOf(child))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a child of node " << static_cast<void*>(strongChild.get()) << " and cannot set it as one of its own children.";
        throw std::runtime_error(sstr.str().c_str());
    }

    strongChild->_parent = this->weak_from_this();
    strongChild->generateParentChains();
    _children.push_back(child);
}

template<typename T>
bool TreeNode<T>::hasChild(TreeNodePtr child)
{
    auto strongChild = child.lock();
    return hasChild(strongChild->id);
}

template<typename T>
bool TreeNode<T>::hasChild(unsigned int id)
{
    std::function<bool(TreeNodePtr)> checkId = [id](TreeNodePtr node)
    {
        auto strongNode = node.lock();
        return id == strongNode->id;
    };

    auto it = std::find_if(_children.begin(), _children.end(), checkId);
    return it != _children.end();
}

template<typename T>
void TreeNode<T>::removeChild(TreeNodePtr child)
{
    auto strongChild = child.lock();
    removeChild(strongChild->id);
}

template<typename T>
void TreeNode<T>::removeChild(unsigned int id)
{
    std::function<bool(TreeNodePtr)> checkId = [id](TreeNodePtr node)
    {
        auto strongNode = node.lock();
        return id == strongNode->id;
    };

    auto it = std::find_if(_children.begin(), _children.end(), checkId);
    if (it != _children.end())
    {
        auto strongChild = it->lock();
        strongChild->_parent.reset();
        strongChild->generateParentChains();
        _children.erase(it);
    }
}

template<typename T>
bool TreeNode<T>::isParentOf(typename TreeNode<T>::TreeNodePtr node)
{
    auto strongNode = node.lock();
    if (strongNode.get() == nullptr) return false;

    return strongNode->isChildOf(id);
}

template<typename T>
bool TreeNode<T>::isChildOf(typename TreeNode<T>::TreeNodePtr node)
{
    auto strongNode = node.lock();
    if (strongNode.get() == nullptr) return false;
    return isChildOf(strongNode->id);
}

template<typename T>
bool TreeNode<T>::isChildOf(unsigned int id)
{
    auto it = std::find(_parentIdChain.begin(), _parentIdChain.end(), id);
    return it != _parentIdChain.end();
}

template<typename T>
void TreeNode<T>::generateParentChains()
{
    auto strongParent = _parent.lock();
    if (strongParent.get() != nullptr)
    {
        _parentChain = strongParent->_parentChain;
        _parentChain.insert(_parentChain.begin(), _parent);

        _parentIdChain = strongParent->_parentIdChain;
        _parentIdChain.insert(_parentIdChain.begin(), strongParent->id);
    }
    else
    {
        _parentChain = std::vector<TreeNodePtr>();
        _parentIdChain = std::vector<unsigned int>();
    }

    for (auto it = _children.begin(); it != _children.end(); it++)
    {
        auto strongChild = it->lock();
        strongChild->generateParentChains();
    }
}

#endif//TREE_NODE_HPP