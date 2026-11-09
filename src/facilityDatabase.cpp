#include "../include/facilityDatabase.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
string trim(const string& value) {
    const string whitespace = " \t\r\n";
    const int first = static_cast<int>(
        value.find_first_not_of(whitespace));
    if (first < 0) {
        return "";
    }
    const int last = static_cast<int>(value.find_last_not_of(whitespace));
    return value.substr(first, last - first + 1);
}

string lowerCase(const string& value) {
    string result = value;
    for (char& character : result) {
        character = static_cast<char>(
            tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

string upperCase(string value) {
    for (char& character : value) {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character)));
    }
    return value;
}

bool validNormalizedId(const string& id) {
    if (id.size() != 4 || id.front() != 'F') {
        return false;
    }
    return isdigit(static_cast<unsigned char>(id[1])) &&
           isdigit(static_cast<unsigned char>(id[2])) &&
           isdigit(static_cast<unsigned char>(id[3]));
}

vector<string> splitFields(const string& line) {
    vector<string> fields;
    stringstream stream(line);
    string field;
    while (getline(stream, field, '|')) {
        fields.push_back(trim(field));
    }
    if (!line.empty() && line.back() == '|') {
        fields.push_back("");
    }
    return fields;
}

bool isAcceptedBuilding(const string& building,
                        const vector<string>& acceptedBuildings) {
    const string normalized = lowerCase(building);
    for (const string& accepted : acceptedBuildings) {
        if (lowerCase(accepted) == normalized) {
            return true;
        }
    }
    return false;
}

string canonicalBuilding(const string& building,
                              const vector<string>& acceptedBuildings) {
    const string normalized = lowerCase(building);
    for (const string& accepted : acceptedBuildings) {
        if (lowerCase(accepted) == normalized) {
            return accepted;
        }
    }
    return "";
}

void printFacility(const Facility& facility) {
    cout << facility.getId() << " | " << facility.getName() << " | "
              << facility.getBuilding() << " | " << facility.getCategory()
              << '\n';
}
}

FacilityDatabase::FacilityDatabase(int tableCapacity)
    : idTable(tableCapacity), rejectedRecords(0) {}

bool FacilityDatabase::load(
    const string& filename,
    const vector<string>& acceptedBuildings) {
    facilities.clear();
    idTable = FacilityTable(idTable.capacity());
    rejectedRecords = 0;

    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << '\n';
        return false;
    }

    int acceptedRecords = 0;
    string line;
    int lineNumber = 0;
    while (getline(input, line)) {
        ++lineNumber;
        const string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine.front() == '#') {
            continue;
        }

        const vector<string> fields = splitFields(line);
        string reason;
        if (fields.size() != 4) {
            reason = "expected four fields";
        } else if (fields[0].empty() || fields[1].empty() ||
                   fields[2].empty() || fields[3].empty()) {
            reason = "a required field is empty";
        } else {
            const string normalizedId = upperCase(fields[0]);
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
            cerr << "Warning: " << filename << ':' << lineNumber
                      << ": " << reason << '\n';
        }
    }

    cout << "Loaded " << filename << ": " << acceptedRecords
              << " accepted, " << rejectedRecords << " rejected"
              << " (capacity " << idTable.capacity() << ", entries "
              << idTable.size() << ", collisions " << idTable.collisions()
              << ")\n";
    return true;
}

bool FacilityDatabase::isValidId(const string& id) {
    return validNormalizedId(upperCase(trim(id)));
}

void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facilities loaded.\n";
        return;
    }
    for (const Facility& facility : facilities) {
        printFacility(facility);
    }
    cout << "Total facilities: " << facilities.size() << '\n';
}

const Facility* FacilityDatabase::findById(const string& id) const {
    return idTable.find(upperCase(trim(id)));
}

vector<Facility> FacilityDatabase::findByName(
    const string& name) const {
    const string normalized = lowerCase(trim(name));
    vector<Facility> matches;
    for (const Facility& facility : facilities) {
        if (lowerCase(facility.getName()) == normalized) {
            matches.push_back(facility);
        }
    }
    return matches;
}

vector<Facility> FacilityDatabase::findByCategory(
    const string& category) const {
    const string normalized = lowerCase(trim(category));
    vector<Facility> matches;
    for (const Facility& facility : facilities) {
        if (lowerCase(facility.getCategory()) == normalized) {
            matches.push_back(facility);
        }
    }
    return matches;
}

void FacilityDatabase::printCategoryReport() const {
    vector<string> categories;
    vector<int> counts;
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
        cout << "No facility categories to report.\n";
        return;
    }
    for (int index = 0; index < static_cast<int>(categories.size()); ++index) {
        cout << categories[index] << ": " << counts[index] << '\n';
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