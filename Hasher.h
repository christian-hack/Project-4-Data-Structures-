//
//  Hasher.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

// Do not add or modify anything in this file.

#ifndef HASHER_H
#define HASHER_H

template <typename T>
class Hasher {
public:
    virtual unsigned long hash(const T& item) const = 0;
};

#endif //! HASHER_H
