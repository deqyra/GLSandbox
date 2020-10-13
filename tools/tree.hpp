#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "tree_node.hpp"

template<typename T>
// A tree composed from nodes, manages and stores pointers to all of its nodes
class Tree
{
    public:
        using Node = TreeNode<T>;
        using NodePtr = std::shared_ptr<Node>;
        using NodeWPtr = std::weak_ptr<Node>;

    private:
        // Pointer to the root node of the tree
        NodePtr _root;
        // Map containing pointers to all of the nodes in the tree (mapped to node ID)
        std::unordered_map<unsigned int, NodePtr> _nodes;

        // Construct tree from a single root node
        Tree(NodePtr root);
        // Construct tree from a single root node and copy all nodes from the provided map
        Tree(NodePtr root, std::unordered_map<unsigned int, NodePtr> nodeMap);

        // Recursively destruct the provided node and all of its children
        void destructBranch(NodePtr root);
        // Get a map containing the provided node and all of its children (mapped to node ID)
        std::unordered_map<unsigned int, NodePtr> branchNodeMap(NodePtr branchRoot);
        // Given a source root node and a destination root node, recursively copy all source children nodes as children of the destination node
        void copyNodeStructure(NodePtr source, NodePtr destination);
        // Count all elements with a certain value from a starting node downwards
        unsigned int countValueRoutine(const T& value, NodePtr startingNode);

    public:
        // Construct a tree from a single root value
        Tree(T rootValue);

        Tree(const Tree<T>& other);
        Tree<T>& operator=(const Tree<T>& other);
        ~Tree();

        // Get a weak pointer to the tree root
        NodePtr getRoot();
        // Get a weak pointer to the node whose ID is provided, if it's part of the tree
        NodePtr operator[](unsigned int id);
        // Whether the tree has a node with provided ID
        bool hasNode(unsigned int id);
        // Add a node to the tree. It will have the provided value and will be a child of the node with provided ID.
        unsigned int addNode(T value, unsigned int parentId);
        // Add a whole branch to the tree. Its structure will be copied from the provided tree, and it will be a child of the node with provided ID.
        unsigned int addBranch(const Tree<T>& tree, unsigned int parentId);
        // Remove the whole branch whose root node has the provided ID
        void removeBranch(unsigned int branchRootId);
        // Remove the whole branch whose root node has the provided ID, and return it as a tree
        Tree<T> popBranch(unsigned int branchRootId);
        // Get (as a tree) the whole branch whose root node has the provided ID
        Tree<T> getBranch(unsigned int branchRootId);
        // Change the parent of a branch whose root has the provided root ID
        void moveBranch(unsigned int branchRootId, unsigned int newParentId);
        // Count elements with provided value, starting from the node with provided ID, downwards
        unsigned int countValue(const T& value, unsigned int branchRootId);
};

template<typename T>
Tree<T>::Tree(T rootValue) :
    _root(nullptr),
    _nodes()
{
    // Create a node from the root value and register it in the node map
    _root = std::make_shared<Node>(rootValue);
    _nodes[_root->id] = _root;
}

template<typename T>
Tree<T>::Tree(const Tree<T>& other) :
    _root(nullptr),
    _nodes()
{
    // Copy the root node value from the other tree, make it this tree's own root
    _root = std::make_shared<Node>(other._root->value);
    // Copy the node structure from the other tree's root to this tree's root
    copyNodeStructure(other._root, _root);
    // Compute the node map
    _nodes = branchNodeMap(_root);
}

template<typename T>
Tree<T>& Tree<T>::operator=(const Tree<T>& other)
{
    // As this tree is being assigned to, its current content must be deleted
    destructBranch(_root);
    _nodes.clear();

    // Copy the root node value from the other tree, make it this tree's own root
    _root = std::make_shared<Node>(other._root->value);
    // Copy the node structure from the other tree's root to this tree's root
    copyNodeStructure(other._root, _root);
    // Compute the new node map
    _nodes = branchNodeMap(_root);
}

template<typename T>
Tree<T>::~Tree()
{
    // Basically delete everything
    destructBranch(_root);
    _nodes.clear();
}

template<typename T>
Tree<T>::Tree(NodePtr root) :
    _root(root),
    _nodes()
{
    _nodes = branchNodeMap(_root);
}

template<typename T>
Tree<T>::Tree(NodePtr root, std::unordered_map<unsigned int, NodePtr> nodeMap) :
    _root(root),
    _nodes(nodeMap)
{

}

template<typename T>
typename Tree<T>::NodePtr Tree<T>::getRoot()
{
    return _root;
}

template<typename T>
typename Tree<T>::NodePtr Tree<T>::operator[](unsigned int id)
{
    auto it = _nodes.find(id);
    if (it == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(id) + ".";
        throw std::runtime_error(s.c_str());
    }

    // Return a weak pointer constructed from the actual pointer
    return it->second;
}

template<typename T>
bool Tree<T>::hasNode(unsigned int id)
{
    NodePtr it = _nodes.find(id);
    return it != _nodes.end();
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

    // Create the new node and have it automatically registered in its parent
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

    // Copy the other tree's root value as a new node of this tree, whose parent is a node with the provided ID
    unsigned int newBranchRootId = addNode(tree._root->value, parentId);
    // Get the pointer to the new node
    NodePtr newBranchRoot = _nodes[newBranchRootId];
    // Copy everything from the provided tree into the new node
    copyNodeStructure(tree._root, newBranchRoot);
    // Compute new node map starting from new node and merge it with the current node map
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
    // Detach the node with provided ID from its parent
    parent->removeChild(branchRootId);
    // Destruct all content starting from that node
    destructBranch(node);
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
    // Detach the node with provided ID from its parent
    parent->removeChild(branchRootId);

    // Construct new tree from that node
    Tree<T> newTree = Tree<T>(node);

    // Delete all children pointers from this tree's node map, using the new tree's node map
    for (auto it = newTree._nodes.begin(); it != newTree._nodes.end(); it++)
    {
        _nodes.erase(it->first);
    }

    return newTree;
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

    // Create a new node with a copy of the branch root node
    NodePtr root = std::make_shared<Node>(it->second->value);
    // Copy the branch structure into the new root
    copyNodeStructure(it->second, root);
    // Make a tree out of it
    return Tree<T>(root);
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

    // Simply reset the parent of the branch root node to new parent node
    branchIt->second->setParent(parentIt->second);
}

template<typename T>
unsigned int Tree<T>::countValue(const T& value, unsigned int branchRootId)
{
    auto branchIt = _nodes.find(branchRootId);
    if (branchIt == _nodes.end())
    {
        std::string s = "Tree: no node with ID " + std::to_string(branchRootId) + ", cannot move branch.";
        throw std::runtime_error(s.c_str());
    }

    return countValueRoutine(value, branchIt->second);
}

template<typename T>
void Tree<T>::destructBranch(NodePtr root)
{
    // First destroy all children of root
    std::vector<NodeWPtr> children = root->getChildren();
    for (auto it = children.begin(); it != children.end(); it++)
    {
        auto child = it->lock();
        destructBranch(child);
    }
    // Reset root parent
    root->setParent(NodeWPtr());
    // Delete pointer to root
    _nodes.erase(root->id);
}

template<typename T>
std::unordered_map<unsigned int, typename Tree<T>::NodePtr> Tree<T>::branchNodeMap(NodePtr branchRoot)
{
    std::unordered_map<unsigned int, NodePtr> registeredNodes;

    std::vector<NodeWPtr> children = branchRoot->getChildren();
    for (auto it = children.begin(); it != children.end(); it++)
    {
        NodePtr child = it->lock();
        // Compute the node map of each child
        std::unordered_map<unsigned int, NodePtr> childNodes = branchNodeMap(child);
        // Merge it with the current node map
        registeredNodes.insert(childNodes.begin(), childNodes.end());
    }

    // Add root node to map
    registeredNodes[branchRoot->id] = branchRoot;
    return registeredNodes;
}

template<typename T>
void Tree<T>::copyNodeStructure(NodePtr source, NodePtr destination)
{
    std::vector<NodeWPtr> sourceChildren = source->getChildren();
    for (auto it = sourceChildren.begin(); it != sourceChildren.end(); it++)
    {
        NodePtr child = it->lock();
        // Make new nodes by copying each child node's value
        NodePtr newChild = std::make_shared<Node>(child->value);
        newChild->setParent(destination);
        // Copy the node structure from each child to the corresponding newly created node
        copyNodeStructure(child, newChild);
    }
}

template<typename T>
unsigned int Tree<T>::countValueRoutine(const T& value, NodePtr startingNode)
{
    int count = startingNode->value == value ? 1 : 0;
    auto children = startingNode->getChildren();
    for (auto it = children.begin(); it != children.end(); it++)
    {
        count += countValueRoutine(value, it->lock());
    }

    return count;
}

#endif//TREE_HPP