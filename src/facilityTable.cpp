#include "../include/facilityTable.h"
#include <cctype>

// IDs are case-insensitive, so every key must use the same uppercase form.
string normalizeId(const string& value) {
    string normalized = value;
    for (char& character : normalized) {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character)));
    }
    return normalized;
}

FacilityTable::FacilityTable(int capacity)
    // A zero-capacity table can't produce a valid hash index.
    : buckets(capacity == 0 ? 1 : capacity),
      entryCount(0),
      collisionCount(0) {}

int FacilityTable::hash(const string& id) const {
    int value = 0;
    for (unsigned char character : id) {
        // Polynomial hashing combines character order and keeps the result inside the table siz
        value = (value * 31 + character) %
                static_cast<int>(buckets.size());
    }
    return value;
}

bool FacilityTable::insert(const Facility& facility) {
    Facility normalized = facility;
    normalized.setId(normalizeId(normalized.getId()));
    vector<Facility>& bucket = buckets.at(hash(normalized.getId()));

    // Separate chaining stores multiple records in one bucket. A matchin key is rejected so the first facility is never overwritten
    for (const Facility& existing : bucket) {
        if (existing.getId() == normalized.getId()) {
            return false;
        }
    }

    if (!bucket.empty()) {
        ++collisionCount;
    }
    bucket.push_back(normalized);
    ++entryCount;
    return true;
}

const Facility* FacilityTable::find(const string& id) const {
    const string normalized = normalizeId(id);
    const vector<Facility>& bucket = buckets.at(hash(normalized));

    // Lookup hashes the ID first, then scans only that bucket.
    for (const Facility& facility : bucket) {
        if (facility.getId() == normalized) {
            return &facility;
        }
    }
    return nullptr;
}

//Getters for the table's capacity, size, and collision count
bool FacilityTable::contains(const string& id) const {
    return find(id) != nullptr;
}

int FacilityTable::capacity() const {
    return static_cast<int>(buckets.size());
}

int FacilityTable::size() const {
    return entryCount;
}

int FacilityTable::collisions() const {
    return collisionCount;
}