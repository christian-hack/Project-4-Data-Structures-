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
void AVLTree<T>::zig()
{
    if (left == NULL) {  // no left, no way to zig
        return;
    }
    // keep track of diff of node and left for changes
    int gdiff = diff;
    int pdiff = left->diff;
    // modify the tree
    AVLTree<T>* olnr = left;  // olnr is "old left, new right"
    left = olnr->left;
    olnr->left = olnr->right;
    olnr->right = right;
    right = olnr;
    // note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
    T tempData = data;
    data = olnr->data;
    olnr->data = tempData;
    // update the diff fields for node and new right
    if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
        diff = gdiff + 2;
        right->diff = gdiff - pdiff + 1;
    }
    else {  // otherwise imbalance was inside left heavy (left-right violation)
        diff = pdiff + 1;
        right->diff = gdiff + 1;
    }
}

template <typename T>
void AVLTree<T>::zag() {
    if (right == NULL) {  // no right, no way to zig
        return;
    }
    // keep track of diff of node and right for changes
    int gdiff = diff;
    int pdiff = right->diff;
    // modify the tree
    AVLTree<T>* ornl = right;  // ornl is "old right, new left"
    right = ornl->right;
    ornl->right = ornl->left;
    ornl->left = left;
    left = ornl;
    // note that the modifications kept the node itself in place, so we need toswap its data with its ornl's
    T tempData = data;
    data = ornl->data;
    ornl->data = tempData;
    // update the diff fields for node and new left
    if (pdiff > 0) {  // if imbalance was outside right heavy (right-right violation)
        diff = gdiff - 2;
        left->diff = gdiff - pdiff - 1;
    }
    else {  // otherwise imbalance was inside right heavy (right-left violation)
        diff = pdiff - 1;
        left->diff = gdiff - 1;
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
    else if ((diff > 0) && (right -> diff < 0)) {
        zagzig();
    }
    else if ((diff > 0) && (right -> diff >= 0)) {
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
                if ((olddiff != left -> diff) && (left -> diff != 0)) {
                    diff--;
                }
                ++size;
            }
            
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
                if ((olddiff != right -> diff) && (right -> diff != 0)) {
                    diff++;
                }
                size++;
            }
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
    if (!this -> contains(item)) {
        return false;
    }
    int olddiff;
    if (comparator -> compare(item, data) == 1) {
        if (right != nullptr) {
            olddiff = right -> diff;
            if (right -> remove(item)) {
                size--;
                if (right->empty || (right -> diff == 0 && olddiff != right -> diff)) {
                    diff--;
                }
            }
        }
        else {
            return false;
        }
    }
    else if (comparator -> compare(item, data) == -1) {
        if (left != nullptr) {
            olddiff = left -> diff;
            if (left -> remove(item)) {
                size--;
                if (left->empty || (left -> diff == 0 && olddiff != left ->diff)) {
                    diff++;
                }
            }
        }
        else {
            return false;
        }
    }
    else {
        if (left == nullptr && right == nullptr) {
            empty = true;
            size--;
        }
        else if (left == nullptr) {
            if (right -> empty) {
                // que es varamos aqui
                // do nothing?
            }
            else {
                data = right -> data;
                delete right;
                right = nullptr;
                diff--;
                size--;
            }
        }
        else if (right == nullptr) {
            if (left -> empty) {
                // que es varamos aqui
                // do nothing?
            }
            else {
                data = left -> data;
                delete left;
                left = nullptr;
                diff++;
                size--;
            }
        }
        else {
            if (left -> empty && right -> empty) {
                empty = true;
                size--;
            }
            // If only left is empty: -- > case above
            else if ((!right -> empty) && left -> empty) {
                data = right -> data;
                delete right;
                right = nullptr;
                diff--;
                size--;
            }
            // If only right is empty: -- > case abover
            else if ((!left -> empty) && right -> empty) {
                data = left -> data;
                delete left;
                left = nullptr;
                diff++;
                size--;
            }
            // Both not empty
            else {
                olddiff = right -> diff;
                AVLTree<T>* succ = right;
                while (succ -> left != nullptr) {
                    succ = succ -> left;
                }
                data = succ -> data;
                right -> remove(succ -> data);
                
                
                if (((right -> diff != olddiff) && (right -> diff == 0))) {
                    diff--;
                }
            }
        }
    }
    if (right != nullptr) {
        if (right -> empty) {
            delete right;
            right = nullptr;
        }
    }
    if (left != nullptr) {
        if (left -> empty) {
            delete left;
            left = nullptr;
        }
    }
    rebalance();
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


