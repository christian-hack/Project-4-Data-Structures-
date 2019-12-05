//
//  Comparator.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

// Do not add or modify anything in this file.

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "Exceptions.h"

template <typename T>
class Comparator {
public:
    // returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
    virtual int compare(const T& item1, const T& item2) const = 0;
    virtual ~Comparator() = default;
};

#endif //! COMPARATOR_H
