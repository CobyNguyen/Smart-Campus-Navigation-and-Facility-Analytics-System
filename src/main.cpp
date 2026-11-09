#include "../include/facilityGraph.h"
#include "../include/facilityDatabase.h"
#include <iostream>  
#include <string>
#include <vector>
#include <fstream>

using namespace std;

FacilityDatabase database; //Creates global instance of both facility database and class
FacilityGraph graph;

void PrintMenu() { //Menu
    cout << "====================\n";
    cout << "Menu Options:\n";
    cout << "1. Display all facilities\n";
    cout << "2. Find facility by ID\n";
    cout << "3. Find facilities by name\n";
    cout << "4. Find facilities by category\n";
    cout << "5. Print category report\n";
    cout << "6. Display graph connections\n";
    cout << "7. Check if a facility is reachable\n";
    cout << "8. Find shortest path between two facilities\n";
    cout << "9. Print connectivity report\n";
    cout << "0. Exit\n";
    cout << "====================\n";
}

void PrintFacility(const Facility& facility) {
    cout << facility.getId() << " | "
         << facility.getName() << " | "
         << facility.getBuilding() << " | "
         << facility.getCategory() << "\n";
}

void PrintMatches(const vector<Facility>& matches) {
    for (const Facility& facility : matches) {
        PrintFacility(facility);
    }
    cout << "Matches: " << matches.size() << "\n";
}

void runCommand(int choice) { //Function to run commands based on user input
    switch (choice) {
        case 1:
            database.displayAll();
            break;
        case 2: {
            string id;
            cout << "Enter facility ID: ";
            getline(cin, id);

            if (!FacilityDatabase::isValidId(id)) {
                cout << "Invalid facility ID.\n";
                break;
            }

            const Facility* facility = database.findById(id);
            if (facility) {
                cout << "Facility found:\n";
                PrintFacility(*facility);
            } else {
                cout << "Facility not found.\n";
            }
            break;
        }
        case 3: {
            string name;
            cout << "Enter facility name: ";
            getline(cin, name);
            PrintMatches(database.findByName(name));
            break;
        }
        case 4: {
            string category;
            cout << "Enter category: ";
            getline(cin, category);
            PrintMatches(database.findByCategory(category));
            break;
        }
        case 5:
            database.printCategoryReport();
            break;
        case 6:
            // graph.display_connections();
            break;
        case 7: {
            break;
        }
        case 8: {
            break;
        }
        case 9:
            // graph.printConnectivityReport();
            break;
        case 0:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

vector<string> LoadBuildings(const string& filename) { // Function to load buildings from a file
    vector<string> buildings;
    ifstream input(filename); 

    if (!input) {
        cerr << "Could not open " << filename << "\n";
        return buildings;
    }

    string line;
    while (getline(input, line)) {
        if (!line.empty() && line[0] != '#') {
            buildings.push_back(line);
        }
    }
    return buildings;
}



int main(){
    vector<string> buildings = LoadBuildings("data/buildings.txt");
    database.load("data/facilities.txt", buildings);

    int choice = -1;

    while (choice != 0) {
        PrintMenu();
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }
        cin.ignore(10000, '\n');
        runCommand(choice);
    }

    return 0;
}