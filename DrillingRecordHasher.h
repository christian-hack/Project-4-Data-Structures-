//
//  DrillingRecordHasher.hpp
//  Project 4
//
//  Created by Christian Hackelman on 11/18/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//
//

#ifndef DRILLING_RECORD_HASHER_H
#define DRILLING_RECORD_HASHER_H

#include "Hasher.h"
#include "DrillingRecord.h"

class DrillingRecordHasher : public Hasher<DrillingRecord> {
public:
    unsigned long hash(const DrillingRecord& item) const;
};

// Implement in DrillingRecordHasher.cpp

#endif //!DRILLING_RECORD_HASHER_H
