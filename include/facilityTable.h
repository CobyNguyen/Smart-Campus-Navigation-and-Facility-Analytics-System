#ifndef FACILITY_TABLE_H
#define FACILITY_TABLE_H

#include "facility.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class FacilityTable {
public:
    FacilityTable(int capacity = 11);

    bool insert(const Facility& facility);
    const Facility* find(const string& id) const;
    bool contains(const string& id) const;

    int capacity() const;
    int size() const;
    int collisions() const;

private:
    int hash(const string& id) const;
    vector<vector<Facility>> buckets;
    int entryCount;
    int collisionCount;
};

#endif