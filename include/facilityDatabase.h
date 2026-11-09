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

    bool load(const std::string& filename,
              const std::vector<std::string>& acceptedBuildings);

    static bool isValidId(const std::string& id);
    void displayAll() const;
    const Facility* findById(const std::string& id) const;
    vector<Facility> findByName(const std::string& name) const;
    vector<Facility> findByCategory(const std::string& category) const;
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