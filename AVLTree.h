//
//  AVLTree.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include "AVLTreeEnumerator.h"

template <typename T>
class AVLTree {
    template <typename U>
    friend class AVLTreeEnumerator;
private:
    Comparator<T>* comparator = NULL;               // used to determine tree order and item equality
    unsigned long size = 0;                         // actual number of items currently in tree
    T data{};                                       // data item of any type
    bool empty = true;                              // flag to indicate whether node contains data
    int diff = 0;                                   // height of right minus height of left
    AVLTree<T>* left = NULL;                        // pointer to left subtree
    AVLTree<T>* right = NULL;                       // pointer to right subtree
    void zig();                                     // right rotation
    void zag();                                     // left rotation
    void zigzag();                                  // left rotation on left subtree, followed by right rotation
    void zagzig();                                  // right rotation on right subtree, followed by left rotation
    void rebalance();                               // check for and rebalance this node, if needed
public:
    AVLTree(Comparator<T>* comparator);             // creates empty linked tree with comparator
    virtual ~AVLTree();                             // deletes subtrees
    
    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave tree unchanged and return false
    bool insert(const T item);
    
    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool replace(const T item);
    
    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool remove(const T item);
    
    // if an equivalent item is present, return true
    // if an equivalent item is not present, false
    bool contains(T item) const;
    
    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
    T find(const T item) const;
    
    unsigned long getSize() const;                  // returns the current number of items in the tree
    
    AVLTreeEnumerator<T> enumerator(AVLTreeOrder order = AVLTreeOrder::inorder) const;    // create an enumerator for this AVL tree
};

// Add your implementation below this line.

template <typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator) {
    this->comparator = comparator;
}

template <typename T>
void AVLTree<T>::zig() {
    T root = data;
    int oldDiff = diff;
    int oldLeftDiff = right -> diff;
    if (!empty || (left != nullptr || !left -> empty)) {
        AVLTree<T>* leftChild = left;
        left = leftChild -> left;
        leftChild -> left = leftChild -> right;
        leftChild -> right = right;
        right = leftChild;
        data = leftChild -> data;
        leftChild -> data = root;
        if (oldDiff >= 0) {
            leftChild -> diff = oldDiff - oldLeftDiff + 1;
            diff = oldDiff + 2;
        }
        else {
            diff = oldLeftDiff + 1;
            leftChild -> diff = oldDiff + 1;
        }
    }
}

template <typename T>
void AVLTree<T>::zag() {
    T root = data;
    int oldDiff = diff;
    int oldRightDiff = right -> diff;
    if (!empty || (right != nullptr || !right -> empty)) {
        AVLTree<T>* rightChild = right;
        right = rightChild -> right;
        rightChild -> right = rightChild -> left;
        rightChild -> left = left;
        left = rightChild;
        data = rightChild -> data;
        rightChild -> data = root;
        if (oldDiff >= 0) {
            rightChild -> diff = oldDiff - oldRightDiff - 1;
            diff = oldDiff - 2;
        }
        else {
            diff = oldRightDiff - 1;
            rightChild -> diff = oldDiff - 1;
        }
    }
}

template <typename T>
void AVLTree<T>::zigzag() {
    left -> zag();
    zig();
}

template <typename T>
void AVLTree<T>::zagzig() {
    right -> zig();
    zag();
}

template <typename T>
void AVLTree<T>::rebalance() {
    if ((diff >= -1) && (diff <= 1)) {
        return;
    }
    if ((diff < 0) && (left -> diff <= 0)) {
        zig();
    }
    else if ((diff < 0) && (left -> diff > 0)) {
        zigzag();
    }
    else if ((diff > 0) && (left -> diff < 0)) {
        zagzig();
    }
    else if ((diff > 0) && (left -> diff >= 0)) {
        zag();
    }
}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete left;
    delete right;
    size = 0;
    empty = true;
    right = nullptr;
    left = nullptr;
    diff = 0;
}

template <typename T>
bool AVLTree<T>::insert(const T item) {
    if (empty) {
        data = item;
        left = new AVLTree<T>(comparator);
        right = new AVLTree<T>(comparator);
        empty = false;
        if (size == 0) {
            ++size;
        }
        return true;
    }
    if (comparator -> compare(item, data) == 0) {
        return false;
    }
    int olddiff = 0;
    if (comparator -> compare(data, item) == 1) {
        if (left -> empty) {
            if (!left -> insert(item)) {
                return false;
            }
            else {
                size++;
                diff--;
            }
        }
        else {
            olddiff = left -> diff;
            if (!left -> insert(item)) {
                return false;
            }
            else {
                ++size;
            }
        }
        if ((olddiff != left -> diff) && (left -> diff != 0)) {
            diff--;
        }
    }
    else {
        if (right -> empty) {
            if (!right -> insert(item)) {
                return false;
            }
            else {
                diff++;
                size++;
            }
        }
        else {
            olddiff = right -> diff;
            if (!right -> insert(item)) {
                return false;
            }
            else {
                size++;
            }
        }
        if ((olddiff != right -> diff) && (right -> diff != 0)) {
            diff++;
        }
    }
    rebalance();
    return true;
}

template <typename T>
bool AVLTree<T>::replace(const T item) {
    if (empty) {
        return false;
    }
    else if (comparator -> compare(item, data) == -1) {
        return left -> replace(item);
    }
    else if (comparator -> compare(item, data) == 1) {
        return right -> replace(item);
    }
    else {
        data = item;
        return true;
    }
}

template <typename T>
bool AVLTree<T>::remove(const T item) {
    int olddiff;
    /*
    if (empty) {
        throw new ExceptionAVLTreeAccess;
    }
     */ 
    if (comparator -> compare(item, data) == 1) {
        olddiff = left -> diff;
        right -> remove(item);
        if ((right -> empty) || ((right -> diff != olddiff) && (right -> diff != 0))) {
            diff--;
        }
    }
    else if (comparator -> compare(item, data) == -1) {
        olddiff = right -> diff;
        left -> remove(item);
        if ((left -> empty) || ((left -> diff != olddiff) && (left -> diff != 0))) {
            diff++;
        }
    }
    else {
        empty = true;
    }
    
    if (right -> empty) {
        AVLTree<T>* oldleft = left;
        right = nullptr;
        /*
         if (data -> empty) {
         empty = true;
         return true;
         }
         */
        // added
        empty = true;
        right = left;
        oldleft = nullptr;
        delete oldleft;
        diff = 0;
    }
    else if (left -> empty) {
        AVLTree<T>* oldright = right;
        left = nullptr;
        /*
        if (data -> empty) {
            empty = true;
            return true;
        }
         */
        // added
        empty = true;
        left = right;
        oldright = nullptr;
        delete oldright;
        diff = 0;
    }
    else {
        AVLTree<T>* succ = right;
        while (!succ -> left -> empty) {
            succ = succ -> left;
        }
        //delete data;
        data = succ -> data;
        olddiff = right -> diff;
        right -> remove(succ -> data);
        if ((right -> empty) || ((right -> diff != olddiff) && (right -> diff == 0))) {
            diff--;
        }
    }
    return true;
}

template <typename T>
bool AVLTree<T>::contains(T item) const {
    if (empty) {
        return false;
    }
    else if (comparator -> compare(item, data) == -1) {
        return left -> contains(item);
    }
    else if (comparator -> compare(item, data) == 1) {
        return right -> contains(item);
    }
    else {
        return true;
    }
}

template <typename T>
T AVLTree<T>::find(const T item) const {
    if (empty) {
        throw new ExceptionAVLTreeAccess;
    }
    else if (comparator -> compare(item, data) == -1) {
        return left -> find(item);
    }
    else if (comparator -> compare(item, data) == 1) {
        return right -> find(item);
    }
    else {
        return data;
    }
}

template <typename T>
unsigned long AVLTree<T>::getSize() const {
    return size;
}

template <typename T>
AVLTreeEnumerator<T> AVLTree<T>::enumerator(AVLTreeOrder order) const {
    return AVLTreeEnumerator<T>(this, order);
}
#endif // !AVL_TREE


