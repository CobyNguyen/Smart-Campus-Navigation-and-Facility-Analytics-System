#include "../include/facilityTable.h"

#include <cctype>

namespace {
// IDs are case-insensitive, so every key must use the same uppercase form.
std::string normalizeId(const std::string& value) {
    std::string normalized = value;
    for (char& character : normalized) {
        character = static_cast<char>(
            std::toupper(static_cast<unsigned char>(character)));
    }
    return normalized;
}
}

FacilityTable::FacilityTable(int capacity)
    // A zero-capacity table cannot produce a valid hash index.
    : buckets(capacity == 0 ? 1 : capacity),
      entryCount(0),
      collisionCount(0) {}

int FacilityTable::hash(const std::string& id) const {
    int value = 0;
    for (unsigned char character : id) {
        // Polynomial hashing combines character order and keeps the result
        // inside the table using the remainder operator.
        value = (value * 31 + character) %
                static_cast<int>(buckets.size());
    }
    return value;
}

bool FacilityTable::insert(const Facility& facility) {
    Facility normalized = facility;
    normalized.setId(normalizeId(normalized.getId()));
    std::vector<Facility>& bucket = buckets.at(hash(normalized.getId()));

    // Separate chaining stores multiple records in one bucket. A matching
    // key is rejected so the first accepted facility is never overwritten.
    for (const Facility& existing : bucket) {
        if (existing.getId() == normalized.getId()) {
            return false;
        }
    }

    if (!bucket.empty()) {
        // For this table, one collision means an accepted record entered a
        // bucket that already contained an accepted record.
        ++collisionCount;
    }
    bucket.push_back(normalized);
    ++entryCount;
    return true;
}

const Facility* FacilityTable::find(const std::string& id) const {
    const std::string normalized = normalizeId(id);
    const std::vector<Facility>& bucket = buckets.at(hash(normalized));

    // Lookup hashes the ID first, then scans only that bucket.
    for (const Facility& facility : bucket) {
        if (facility.getId() == normalized) {
            return &facility;
        }
    }
    return nullptr;
}

bool FacilityTable::contains(const std::string& id) const {
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