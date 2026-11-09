#include "../include/facility.h"

Facility::Facility(const string& facilityId,
                   const string& facilityName,
                   const string& facilityBuilding,
                   const string& facilityCategory)
    : id(facilityId),
      name(facilityName),
      building(facilityBuilding),
      category(facilityCategory) {}

const string& Facility::getId() const {
    return id;
}

const string& Facility::getName() const {
    return name;
}

const string& Facility::getBuilding() const {
    return building;
}

const string& Facility::getCategory() const {
    return category;
}

void Facility::setId(const string& facilityId) {
    id = facilityId;
}

void Facility::setName(const string& facilityName) {
    name = facilityName;
}

void Facility::setBuilding(const string& facilityBuilding) {
    building = facilityBuilding;
}

void Facility::setCategory(const string& facilityCategory) {
    category = facilityCategory;
}
