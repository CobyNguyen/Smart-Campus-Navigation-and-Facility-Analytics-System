#include "../include/facilityDatabase.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const int first = static_cast<int>(
        value.find_first_not_of(whitespace));
    if (first < 0) {
        return "";
    }
    const int last = static_cast<int>(value.find_last_not_of(whitespace));
    return value.substr(first, last - first + 1);
}

std::string lowerCase(const std::string& value) {
    std::string result = value;
    for (char& character : result) {
        character = static_cast<char>(
            std::tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

std::string upperCase(std::string value) {
    for (char& character : value) {
        character = static_cast<char>(
            std::toupper(static_cast<unsigned char>(character)));
    }
    return value;
}

bool validNormalizedId(const std::string& id) {
    if (id.size() != 4 || id.front() != 'F') {
        return false;
    }
    return std::isdigit(static_cast<unsigned char>(id[1])) &&
           std::isdigit(static_cast<unsigned char>(id[2])) &&
           std::isdigit(static_cast<unsigned char>(id[3]));
}

std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, '|')) {
        fields.push_back(trim(field));
    }
    if (!line.empty() && line.back() == '|') {
        fields.push_back("");
    }
    return fields;
}

bool isAcceptedBuilding(const std::string& building,
                        const std::vector<std::string>& acceptedBuildings) {
    const std::string normalized = lowerCase(building);
    for (const std::string& accepted : acceptedBuildings) {
        if (lowerCase(accepted) == normalized) {
            return true;
        }
    }
    return false;
}

std::string canonicalBuilding(const std::string& building,
                              const std::vector<std::string>& acceptedBuildings) {
    const std::string normalized = lowerCase(building);
    for (const std::string& accepted : acceptedBuildings) {
        if (lowerCase(accepted) == normalized) {
            return accepted;
        }
    }
    return "";
}

void printFacility(const Facility& facility) {
    std::cout << facility.getId() << " | " << facility.getName() << " | "
              << facility.getBuilding() << " | " << facility.getCategory()
              << '\n';
}
}

FacilityDatabase::FacilityDatabase(int tableCapacity)
    : idTable(tableCapacity), rejectedRecords(0) {}

bool FacilityDatabase::load(
    const std::string& filename,
    const std::vector<std::string>& acceptedBuildings) {
    facilities.clear();
    idTable = FacilityTable(idTable.capacity());
    rejectedRecords = 0;

    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Error: could not open " << filename << '\n';
        return false;
    }

    int acceptedRecords = 0;
    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        const std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine.front() == '#') {
            continue;
        }

        const std::vector<std::string> fields = splitFields(line);
        std::string reason;
        if (fields.size() != 4) {
            reason = "expected four fields";
        } else if (fields[0].empty() || fields[1].empty() ||
                   fields[2].empty() || fields[3].empty()) {
            reason = "a required field is empty";
        } else {
            const std::string normalizedId = upperCase(fields[0]);
            if (!validNormalizedId(normalizedId)) {
                reason = "invalid facility ID";
            } else if (!isAcceptedBuilding(fields[2], acceptedBuildings)) {
                reason = "unknown building";
            } else if (idTable.contains(normalizedId)) {
                reason = "duplicate facility ID";
            } else {
                Facility facility(normalizedId, fields[1],
                                  canonicalBuilding(fields[2], acceptedBuildings),
                                  fields[3]);
                idTable.insert(facility);
                facilities.push_back(facility);
                ++acceptedRecords;
            }
        }

        if (!reason.empty()) {
            ++rejectedRecords;
            std::cerr << "Warning: " << filename << ':' << lineNumber
                      << ": " << reason << '\n';
        }
    }

    std::cout << "Loaded " << filename << ": " << acceptedRecords
              << " accepted, " << rejectedRecords << " rejected"
              << " (capacity " << idTable.capacity() << ", entries "
              << idTable.size() << ", collisions " << idTable.collisions()
              << ")\n";
    return true;
}

bool FacilityDatabase::isValidId(const std::string& id) {
    return validNormalizedId(upperCase(trim(id)));
}

void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        std::cout << "No facilities loaded.\n";
        return;
    }
    for (const Facility& facility : facilities) {
        printFacility(facility);
    }
    std::cout << "Total facilities: " << facilities.size() << '\n';
}

const Facility* FacilityDatabase::findById(const std::string& id) const {
    return idTable.find(upperCase(trim(id)));
}

std::vector<Facility> FacilityDatabase::findByName(
    const std::string& name) const {
    const std::string normalized = lowerCase(trim(name));
    std::vector<Facility> matches;
    for (const Facility& facility : facilities) {
        if (lowerCase(facility.getName()) == normalized) {
            matches.push_back(facility);
        }
    }
    return matches;
}

std::vector<Facility> FacilityDatabase::findByCategory(
    const std::string& category) const {
    const std::string normalized = lowerCase(trim(category));
    std::vector<Facility> matches;
    for (const Facility& facility : facilities) {
        if (lowerCase(facility.getCategory()) == normalized) {
            matches.push_back(facility);
        }
    }
    return matches;
}

void FacilityDatabase::printCategoryReport() const {
    std::vector<std::string> categories;
    std::vector<int> counts;
    for (const Facility& facility : facilities) {
        int index = 0;
        while (index < static_cast<int>(categories.size()) &&
               lowerCase(categories[index]) !=
                   lowerCase(facility.getCategory())) {
            ++index;
        }
        if (index == static_cast<int>(categories.size())) {
            categories.push_back(facility.getCategory());
            counts.push_back(1);
        } else {
            ++counts[index];
        }
    }

    if (categories.empty()) {
        std::cout << "No facility categories to report.\n";
        return;
    }
    for (int index = 0; index < static_cast<int>(categories.size()); ++index) {
        std::cout << categories[index] << ": " << counts[index] << '\n';
    }
}

int FacilityDatabase::acceptedCount() const {
    return facilities.size();
}

int FacilityDatabase::rejectedCount() const {
    return rejectedRecords;
}

const FacilityTable& FacilityDatabase::table() const {
    return idTable;
}