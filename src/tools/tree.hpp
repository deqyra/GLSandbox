#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "tree_node.hpp"

template<typename T>
class Tree
{
    public:
        using Node = TreeNode<T>;
        using NodePtr = std::shared_ptr<Node>;
        using NodeWPtr = std::weak_ptr<Node>;

    private:
        NodePtr _root;
        std::unordered_map<unsigned int, NodePtr> _nodes;

        Tree(NodePtr root);
        Tree(NodePtr root, std::unordered_map<unsigned int, NodePtr> nodeMap);

        void destructBranch(NodePtr root);
        std::unordered_map<unsigned int, NodePtr> branchNodeMap(NodePtr branchRoot);
        void copyNodeStructure(NodePtr source, NodePtr destination);

    public:
        Tree(T rootValue);
        Tree(const Tree<T>& other);
        Tree<T>& operator=(const Tree<T>& other);
        ~Tree();

        NodeWPtr getRoot();
        NodeWPtr operator[](unsigned int id);
        bool hasNode(unsigned int id);
        bool hasNode(NodeWPtr node);
        unsigned int addNode(T value, unsigned int parentId);
        unsigned int addBranch(const Tree<T>& tree, unsigned int parentId);
        void removeBranch(unsigned int branchRootId);
        void removeBranch(NodeWPtr branchRootNode);
        Tree<T> popBranch(unsigned int branchRootId);
        Tree<T> popBranch(NodeWPtr branchRootNode);
        Tree<T> getBranch(unsigned int branchRootId);
        Tree<T> getBranch(NodeWPtr branchRootNode);
        void moveBranch(unsigned int branchRootId, unsigned int newParentId);
        void moveBranch(NodeWPtr branchRootNode, NodeWPtr newParentNode);
};

template<typename T>
Tree<T>::Tree(T rootValue) :
    _root(nullptr),
    _nodes()
{
    _root = std::make_shared<Node>(rootValue);
    _nodes[_root->id] = _root;
}

template<typename T>
Tree<T>::Tree(const Tree<T>& other) :
    _root(nullptr),
    _nodes()
{
    _root = std::make_shared<Node>(nullptr, other._root->value);
    copyNodeStructure(other._root, _root);
    _nodes = branchNodeMap(_root);
}

template<typename T>
Tree<T>& Tree<T>::operator=(const Tree<T>& other)
{
    destructBranch(_root);
    _nodes.clear();

    _root = std::make_shared<Node>(nullptr, other._root->value);
    copyNodeStructure(other._root, _root);
    _nodes = branchNodeMap(_root);
}

template<typename T>
Tree<T>::~Tree()
{
    destructBranch(_root);
    _nodes.clear();
}

template<typename T>
Tree<T>::Tree(NodePtr root) :
    _root(root),
    _nodes()
{
    _nodes = branchNodeMap(root);
}

template<typename T>
Tree<T>::Tree(NodePtr root, std::unordered_map<unsigned int, NodePtr> nodeMap) :
    _root(root),
    _nodes(nodeMap)
{

}

template<typename T>
typename Tree<T>::NodeWPtr Tree<T>::getRoot()
{
    return NodeWPtr(_root);
}

template<typename T>
typename Tree<T>::NodeWPtr Tree<T>::operator[](unsigned int id)
{
    auto it = _nodes.find(id);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    return NodeWPtr(it->second);
}

template<typename T>
bool Tree<T>::hasNode(unsigned int id)
{
    auto it = _nodes.find(id);
    return it != _nodes.end();
}

template<typename T>
bool Tree<T>::hasNode(NodeWPtr node)
{
    auto strongNode = node.lock();
    return hasNode(strongNode->id);
}

template<typename T>
unsigned int Tree<T>::addNode(T value, unsigned int parentId)
{
    auto it = _nodes.find(parentId);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(parentId) + ", cannot add child node.";
        throw std::runtime_error(s.c_str());
    }

    // Create the new node and have it automatically registered in its parent.
    NodePtr node = std::make_shared<Node>(value);
    node->setParent(it->second);
    _nodes[node->id] = node;
    return node->id;
}

template<typename T>
unsigned int Tree<T>::addBranch(const Tree<T>& tree, unsigned int parentId)
{
    auto it = _nodes.find(parentId);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(parentId) + ", cannot add child branch.";
        throw std::runtime_error(s.c_str());
    }

    unsigned int newBranchRootId = addNode(tree._root->value, parentId);
    NodePtr newBranchRoot = _nodes[newBranchRootId];
    copyNodeStructure(tree._root, newBranchRoot);
    std::unordered_map<unsigned int, NodePtr> newNodes = branchNodeMap(newBranchRoot);
    _nodes.insert(newNodes.begin(), newNodes.end());
    return newBranchRootId;
}

template<typename T>
void Tree<T>::removeBranch(unsigned int branchRootId)
{
    auto it = _nodes.find(branchRootId);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(branchRootId) + ", cannot remove branch.";
        throw std::runtime_error(s.c_str());
    }

    NodePtr node = _nodes[branchRootId];
    NodePtr parent = it->second->getParent().lock();
    parent->removeChild(branchRootId);
    destructBranch(node);
}

template<typename T>
void Tree<T>::removeBranch(NodeWPtr branchRootNode)
{
    auto strongNode = branchRootNode.lock();
    removeBranch(strongNode->id);
}

template<typename T>
Tree<T> Tree<T>::popBranch(unsigned int branchRootId)
{
    auto it = _nodes.find(branchRootId);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(branchRootId) + ", cannot pop branch.";
        throw std::runtime_error(s.c_str());
    }

    NodePtr node = _nodes[branchRootId];
    NodePtr parent = node->getParent().lock();
    parent->removeChild(branchRootId);

    std::unordered_map<unsigned int, NodePtr> newTreeNodes;
    for (auto it = newTreeNodes.begin(); it != newTreeNodes.end(); it++)
    {
        _nodes.erase(it->first);
    }

    return Tree<T>(node);
}

template<typename T>
Tree<T> Tree<T>::popBranch(NodeWPtr branchRootNode)
{
    auto strongNode = branchRootNode.lock();
    return popBranch(strongNode->id);
}

template<typename T>
Tree<T> Tree<T>::getBranch(unsigned int branchRootId)
{
    auto it = _nodes.find(branchRootId);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(branchRootId) + ", cannot get branch.";
        throw std::runtime_error(s.c_str());
    }

    NodePtr root = std::make_shared<Node>(nullptr, it->second->value);
    copyNodeStructure(it->second, root);
    return Tree<T>(root);
}

template<typename T>
Tree<T> Tree<T>::getBranch(NodeWPtr branchRootNode)
{
    auto strongNode = branchRootNode.lock();
    return getBranch(strongNode->id);
}

template<typename T>
void Tree<T>::moveBranch(unsigned int branchRootId, unsigned int newParentId)
{
    auto branchIt = _nodes.find(branchRootId);
    if (branchIt == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(branchRootId) + ", cannot move branch.";
        throw std::runtime_error(s.c_str());
    }

    auto parentIt = _nodes.find(newParentId);
    if (parentIt == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(newParentId) + ", cannot move branch " + std::to_string(branchRootId) + " to it.";
        throw std::runtime_error(s.c_str());
    }

    branchIt->second->setParent(parentIt->second);
}

template<typename T>
void Tree<T>::moveBranch(NodeWPtr branchRootNode, NodeWPtr newParentNode)
{
    auto strongRoot = branchRootNode.lock();
    auto strongParent = newParentNode.lock();
    moveBranch(strongRoot->id, strongParent->id);
}

template<typename T>
void Tree<T>::destructBranch(NodePtr root)
{
    std::vector<NodeWPtr> children = root->getChildren();
    for (auto it = children.begin(); it != children.end(); it++)
    {
        auto strongChild = it->lock();
        destructBranch(strongChild);
    }
    root->setParent(NodeWPtr());
    _nodes.erase(root->id);
}

template<typename T>
std::unordered_map<unsigned int, typename Tree<T>::NodePtr> Tree<T>::branchNodeMap(NodePtr branchRoot)
{
    std::unordered_map<unsigned int, NodePtr> registeredNodes;

    std::vector<NodeWPtr> children = branchRoot->getChildren();
    for (auto it = children.begin(); it != children.end(); it++)
    {
        auto strongChild = it->lock();
        std::unordered_map<unsigned int, NodePtr> childNodes = branchNodeMap(strongChild);
        registeredNodes.insert(childNodes.begin(), childNodes.end());
    }

    registeredNodes[branchRoot->id] = branchRoot;
    return registeredNodes;
}

template<typename T>
void Tree<T>::copyNodeStructure(NodePtr source, NodePtr destination)
{
    std::vector<NodeWPtr> sourceChildren = source->getChildren();
    for (auto it = sourceChildren.begin(); it != sourceChildren.end(); it++)
    {
        auto strongChild = it->lock();
        NodePtr newChild = std::make_shared<Node>(destination, strongChild->value);
        copyNodeStructure(strongChild, newChild);
    }
}

#endif//TREE_HPP