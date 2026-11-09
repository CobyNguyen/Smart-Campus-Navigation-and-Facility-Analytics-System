#ifndef FACILITY_TABLE_H
#define FACILITY_TABLE_H

#include "facility.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class FacilityTable {
public:
    FacilityTable(size_t capacity = 11);

    bool insert(const Facility& facility);
    const Facility* find(const std::string& id) const;
    bool contains(const std::string& id) const;

    size_t capacity() const; 
    size_t size() const;
    size_t collisions() const;

private:
    size_t hash(const std::string& id) const;
    std::vector<std::vector<Facility>> buckets;
    size_t entryCount;
    size_t collisionCount;
};

#endif