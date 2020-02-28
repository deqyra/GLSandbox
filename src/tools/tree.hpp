#ifndef TREE_HPP
#define TREE_HPP

#include "tree_node.hpp"

template<typename T>
class Tree
{
    using NodePtr = TreeNode<T>::TreeNodePtr;

    private:
        Tree(const Tree& other) = delete;
        Tree& operator=(const Tree& other) = delete;

    public:
        Tree(NodePtr root);
        const NodePtr root;
};

template<typename T>
Tree<T>::Tree(NodePtr root) :
    root(root)
{

}

#endif//TREE_HPP