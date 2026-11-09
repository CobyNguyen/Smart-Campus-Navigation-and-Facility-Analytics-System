#ifndef FACILITY_DATABASE_H
#define FACILITY_DATABASE_H

#include "facility.h"
#include "facilityTable.h"
#include <string>
#include <vector>

using namespace std;

class FacilityDatabase {
public:
    explicit FacilityDatabase(int tableCapacity = 11);

    bool load(const string& filename,
              const vector<string>& acceptedBuildings);

    static bool isValidId(const string& id);
    void displayAll() const;
    const Facility* findById(const string& id) const;
    vector<Facility> findByName(const string& name) const;
    vector<Facility> findByCategory(const string& category) const;
    void printCategoryReport() const;

    int acceptedCount() const;
    int rejectedCount() const;
    const FacilityTable& table() const;

private:
    vector<Facility> facilities;
    FacilityTable idTable;
    int rejectedRecords;
};

#endif