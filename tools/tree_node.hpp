#ifndef TOOLS__TREE_NODE_HPP
#define TOOLS__TREE_NODE_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template<typename T>
// A node to be used in a tree, stores a T-type value and weak pointers to surronding nodes
class TreeNode : public std::enable_shared_from_this<TreeNode<T>>
{
    using TreeNodeWPtr = std::weak_ptr<TreeNode<T>>;
    using TreeNodePtr = std::shared_ptr<TreeNode<T>>;

    private:
        TreeNode(const TreeNode& other) = delete;
        TreeNode<T>& operator=(const TreeNode& other) = delete;

        // Keeps track of how many TreeNode objects were created (used as an ID system)
        static unsigned int _count;

        // Weak pointer to the parent of this node
        TreeNodeWPtr _wParent;
        // Weak pointers to the children of this node
        std::vector<TreeNodeWPtr> _wChildren;

        // Weak pointers to all the parents in the parent chain of this node
        std::vector<TreeNodeWPtr> _wParentChain;
        // IDs of all the parents in the parent chain of this node
        std::vector<unsigned int> _parentIdChain;

        // Update the lists of IDs and pointers to parents in the parent chain
        void generateParentChains();

    public:
        // Construct new node with value attached
        TreeNode(T value);

        // Get a weak pointer to the parent of this node
        TreeNodeWPtr getParent();
        // Set the parent of this node from a weak pointer
        void setParent(TreeNodeWPtr wParent);
        // Get a list of weak pointers to all parents of this node, from closest to furthest
        std::vector<TreeNodeWPtr> getParentChain();
        // Get list of IDs of all parents of this node, from closest to furthest
        std::vector<unsigned int> getParentIdChain();
        // Get list of weak pointers to all children of this node
        std::vector<TreeNodeWPtr> getChildren();
        // Add new child to this node from a weak pointer
        void addChild(TreeNodeWPtr wChild);
        // Remove child from this node from a weak pointer
        void removeChild(TreeNodeWPtr wChild);
        // Remove child from this node using its ID
        void removeChild(unsigned int id);
        // Whether the provided weak pointer is a child of this node
        bool hasChild(TreeNodeWPtr wChild);
        // Whether the node with provided ID is a child of this node
        bool hasChild(unsigned int id);
        // Whether this node is a parent (to any degree) of the provided node weak pointer
        bool isParentOf(TreeNodeWPtr wNode);
        // Whether this node is a child (to any degree) of the provided node weak pointer
        bool isChildOf(TreeNodeWPtr wNode);
        // Whether this node is a child (to any degree) of the node with provided ID
        bool isChildOf(unsigned int id);
        
        // Unique ID of this node
        const unsigned int id;
        // Value attached to this node
        T value;
};

template<typename T>
unsigned int TreeNode<T>::_count = 0;

template<typename T>
TreeNode<T>::TreeNode(T value) :
    id(_count++),
    _wParent(),
    _wChildren(),
    _wParentChain(),
    _parentIdChain(),
    value(value)
{

}

template<typename T>
typename TreeNode<T>::TreeNodeWPtr TreeNode<T>::getParent()
{
    return _wParent;
}

template<typename T>
void TreeNode<T>::setParent(TreeNodeWPtr wNewParent)
{
    TreeNodePtr newParent = wNewParent.lock();
    TreeNodePtr parent = _wParent.lock();

    // If the new parent is already this node's parent, do nothing
    if (newParent == parent) return;

    if (newParent.get() == this)
    {
        std::string s = "TreeNode: node cannot be its own parent.";
        throw std::runtime_error(s.c_str());
    }

    // If this node is a parent of the new parent, abort
    if (isParentOf(wNewParent))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a parent of node " << static_cast<void*>(newParent.get()) << " and cannot set it as its own parent.";
        throw std::runtime_error(sstr.str().c_str());
    }

    // Remove this node as a child from its current parent if applicable
    if (parent != nullptr)
    {
        parent->removeChild(id);
    }

    // If applicable, register this node as a new child of the new parent
    if (newParent != nullptr)
    {
        newParent->_wChildren.push_back(this->weak_from_this());
    }
    // Update this node's parent weak pointer
    _wParent = wNewParent;

    // Update parent chains
    generateParentChains();
}

template<typename T>
std::vector<typename TreeNode<T>::TreeNodeWPtr> TreeNode<T>::getParentChain()
{
    return _wParentChain;
}

template<typename T>
std::vector<unsigned int> TreeNode<T>::getParentIdChain()
{
    return _parentIdChain;
}

template<typename T>
std::vector<typename TreeNode<T>::TreeNodeWPtr> TreeNode<T>::getChildren()
{
    return _wChildren;
}

template<typename T>
void TreeNode<T>::addChild(TreeNodeWPtr wChild)
{
    TreeNodePtr child = wChild.lock();
    if (child == nullptr)
    {
        std::string s = "TreeNode: provided node pointer is dangling, cannot add as child.";
        throw std::runtime_error(s.c_str());
    }

    auto childParent = child->_wParent.lock();

    // If this node is already the new child's parent node, do nothing
    if (childParent == this->shared_from_this()) return;

    // If the new child already has a parent node, abort
    if (childParent != nullptr)
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(child.get()) << " already has a parent, cannot add as child.";
        throw std::runtime_error(sstr.str().c_str());
    }

    // If this node is a child of the new child node, abort
    if (isChildOf(wChild))
    {
        std::stringstream sstr;
        sstr << "TreeNode: node " << static_cast<void*>(this) << " is a child of node " << static_cast<void*>(child.get()) << " and cannot set it as one of its own children.";
        throw std::runtime_error(sstr.str().c_str());
    }

    // Update parent pointer in the new child
    child->_wParent = this->weak_from_this();
    // Update parent chains in the new child
    child->generateParentChains();
    // Register child
    _wChildren.push_back(wChild);
}

template<typename T>
bool TreeNode<T>::hasChild(TreeNodeWPtr wChild)
{
    TreeNodePtr child = wChild.lock();
    if (child == nullptr)
    {
        std::string s = "TreeNode: provided node pointer is dangling, cannot check as a potential child.";
        throw std::runtime_error(s.c_str());
    }
    // Defer the check to the ID-version of the method
    return hasChild(child->id);
}

template<typename T>
bool TreeNode<T>::hasChild(unsigned int id)
{
    // Lambda to check whether a provided weak pointer to a node has the provided ID
    std::function<bool(TreeNodeWPtr)> checkId = [id](TreeNodeWPtr wNode)
    {
        TreeNodePtr node = wNode.lock();
        // Not checking whether node is nullptr because it should never happen
        return id == node->id;
    };

    // Find any child with the provided ID in this node's children, using the lambda
    auto it = std::find_if(_wChildren.begin(), _wChildren.end(), checkId);

    return it != _wChildren.end();
}

template<typename T>
void TreeNode<T>::removeChild(TreeNodeWPtr wChild)
{
    TreeNodePtr child = wChild.lock();
    if (child == nullptr)
    {
        std::string s = "TreeNode: provided node pointer is dangling, cannot remove child.";
        throw std::runtime_error(s.c_str());
    }
    // Defer the processing to the ID-version of the method
    removeChild(child->id);
}

template<typename T>
void TreeNode<T>::removeChild(unsigned int id)
{
    // Lambda to check whether a provided weak pointer to a node has the provided ID
    std::function<bool(TreeNodeWPtr)> checkId = [id](TreeNodeWPtr wNode)
    {
        auto node = wNode.lock();
        // Not checking whether node is nullptr because it should never happen
        return id == node->id;
    };

    // Find any child with the provided ID in this node's children, using the lambda
    auto it = std::find_if(_wChildren.begin(), _wChildren.end(), checkId);

    // If a result was found, delete it
    if (it != _wChildren.end())
    {
        auto child = it->lock();
        // Reset the child node's parent pointer
        child->_wParent.reset();
        child->generateParentChains();
        // Effectively remove the pointer to the child node
        _wChildren.erase(it);
    }
}

template<typename T>
bool TreeNode<T>::isParentOf(TreeNodeWPtr wNode)
{
    auto node = wNode.lock();
    if (node.get() == nullptr) return false;
    // This node is a parent of the provided node if the provided node is a child of this node
    return node->isChildOf(id);
}

template<typename T>
bool TreeNode<T>::isChildOf(TreeNodeWPtr wNode)
{
    auto node = wNode.lock();
    if (node.get() == nullptr) return false;
    // Defer the check to the ID-version of the method
    return isChildOf(node->id);
}

template<typename T>
bool TreeNode<T>::isChildOf(unsigned int id)
{
    // Find the provided ID in the parent ID chain
    auto it = std::find(_parentIdChain.begin(), _parentIdChain.end(), id);
    return it != _parentIdChain.end();
}

template<typename T>
void TreeNode<T>::generateParentChains()
{
    auto parent = _wParent.lock();
    if (parent != nullptr)
    {
        // If this node has a parent, copy both of its own parent chains
        _wParentChain = parent->_wParentChain;
        _parentIdChain = parent->_parentIdChain;
        // Prepend the parent node to both
        _wParentChain.insert(_wParentChain.begin(), _wParent);
        _parentIdChain.insert(_parentIdChain.begin(), parent->id);
    }
    else
    {
        // If this node has no parent, the chains are empty
        _wParentChain = std::vector<TreeNodeWPtr>();
        _parentIdChain = std::vector<unsigned int>();
    }

    // Reverberate the update to all children
    for (auto it = _wChildren.begin(); it != _wChildren.end(); it++)
    {
        auto child = it->lock();
        child->generateParentChains();
    }
}

#endif//TOOLS__TREE_NODE_HPP