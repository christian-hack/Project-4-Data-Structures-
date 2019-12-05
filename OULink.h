//
//  OULink.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef OU_LINK_H
#define OU_LINK_H

template <typename T>
class OULink {
    template <typename U>
    friend class OULinkedList;
    template <typename U>
    friend class OULinkedListEnumerator;
private:
    T data{};                  // data item of any type
    OULink* next = NULL;       // pointer to next link
public:
    OULink(T item);
    virtual ~OULink();
};

// Add your implementation below this line. Do not add or modify anything above this line.
template <typename T>
OULink<T>::OULink(T item) {
    data = item;
}
template <typename T>
OULink<T>::~OULink() {
    if (next != NULL) {
        delete next;
    }
}

#endif // !OU_LINK
