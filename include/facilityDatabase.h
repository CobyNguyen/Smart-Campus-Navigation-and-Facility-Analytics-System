#ifndef FACILITY_DATABASE_H
#define FACILITY_DATABASE_H

#include "facility.h"
#include "facilityTable.h"

#include <string>
#include <vector>

class FacilityDatabase {
public:
    explicit FacilityDatabase(size_t tableCapacity = 11);

    bool load(const std::string& filename,
              const std::vector<std::string>& acceptedBuildings);

    static bool isValidId(const std::string& id);
    void displayAll() const;
    const Facility* findById(const std::string& id) const;
    std::vector<Facility> findByName(const std::string& name) const;
    std::vector<Facility> findByCategory(const std::string& category) const;
    void printCategoryReport() const;

    size_t acceptedCount() const;
    size_t rejectedCount() const;
    const FacilityTable& table() const;

private:
    std::vector<Facility> facilities;
    FacilityTable idTable;
    size_t rejectedRecords;
};

#endif