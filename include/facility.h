#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using namespace std;

class Facility {
public:
    Facility() = default; // Default constructors
    Facility(const string& facilityId,
             const string& facilityName,
             const string& facilityBuilding,
             const string& facilityCategory);

                //Getters
    const string& getId() const;
    const string& getName() const;
    const string& getBuilding() const;
    const string& getCategory() const;
        //setters
    void setId(const string& facilityId);
    void setName(const string& facilityName);
    void setBuilding(const string& facilityBuilding);
    void setCategory(const string& facilityCategory);

private:
    string id;
    string name;
    string building;
    string category;
};

#endif