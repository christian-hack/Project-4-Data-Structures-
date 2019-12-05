//
//  AVLTreeEnumerator.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"

template <typename S>
class AVLTree;

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
    AVLTreeOrder order;
    std::stack<const AVLTree<T>*> traversalStack;
    void buildTraversalStack(const AVLTree<T>* current);
public:
    AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder::inorder);
    virtual ~AVLTreeEnumerator();
    bool hasNext() const;
    T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
    AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};

// Add your implementation below this line.
template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order) {
    if (root -> empty) {
        throw new ExceptionEnumerationBeyondEnd;
    }
    this -> order = order;
    buildTraversalStack(root);
}

template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator<T>() {
    // do nothing
}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const {
    if (traversalStack.empty()) {
        return false;
    }
    else {
        return true;
    }
}

template <typename T>
T AVLTreeEnumerator<T>::next() {
    if (!hasNext()) {
        throw new ExceptionEnumerationBeyondEnd;
    }
    else {
        T data = traversalStack.top() -> data;
        traversalStack.pop();
        return data;
    }
}

template <typename T>
T AVLTreeEnumerator<T>::peek() const {
    if (!hasNext()) {
        throw new ExceptionEnumerationBeyondEnd;
    }
    else {
        return traversalStack.top() -> data;
    }
}

template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder() {
    return order;
}

template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T> *current) {
    if (current == NULL || current -> empty) {
        return;
    }
    if (order == AVLTreeOrder::inorder) { //left, root, right
        buildTraversalStack(current -> right);
        traversalStack.push(current);
        buildTraversalStack(current -> left);
    }
    else if (order == AVLTreeOrder::preorder) { // root, left, right
        buildTraversalStack(current -> right);
        buildTraversalStack(current -> left);
        traversalStack.push(current);
    }
    else if (order == AVLTreeOrder::postorder) { // left, right, root
        traversalStack.push(current);
        buildTraversalStack(current -> right);
        buildTraversalStack(current -> left);
    }
}
#endif // !AVL_TREE_ENUMERATOR
