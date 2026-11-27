// Program entry point and shared menu for the facility, graph, and department modules.
// Authors: Coby Nguyen, Noah Clifford, and Aayus Tamang (shared integration).

#include "../include/facilityGraph.h"
#include "../include/facilityDatabase.h"
#include "../include/DepartmentMenu.h"
#include "../include/DepartmentTree.h"
#include <iostream>  
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

using namespace std;

FacilityDatabase database; //Creates global instance of both facility database and class
FacilityGraph graph;
DepartmentTree departmentTree;

constexpr int exitOption = 18;

void PrintMenu() { //Menu
    cout << "====================\n";
    cout << "Menu Options:\n";
    cout << "1. View all facilities\n";
    cout << "2. Search facility by ID\n";
    cout << "3. Search facility by name\n";
    cout << "4. Search facility by category\n";
    cout << "5. View campus graph\n";
    cout << "6. Display graph counts and degrees\n";
    cout << "7. Find reachable buildings (BFS)\n";
    cout << "8. Find shortest path (BFS)\n";
    cout << "9. Add department\n";
    cout << "10. Search department\n";
    cout << "11. Display department hierarchy\n";
    cout << "12. Preorder traversal\n";
    cout << "13. Postorder traversal\n";
    cout << "14. Level-order traversal\n";
    cout << "15. Department statistics\n";
    cout << "16. Facility category report\n";
    cout << "17. Graph connectivity report\n";
    cout << exitOption << ". Exit\n";
    cout << "====================\n";
}

// Print one facility in the same format as the database listing.
void PrintFacility(const Facility& facility) {
    cout << facility.getId() << " | "
         << facility.getName() << " | "
         << facility.getBuilding() << " | "
         << facility.getCategory() << "\n";
}

// Read a complete name, including spaces, and reject blank input.
bool ReadName(const char* prompt, string& name) {
    cout << prompt;
    if (!getline(cin, name)) {
        return false;
    }
    if (name.find_first_not_of(" \t\r\n\v\f") == string::npos) {
        cout << "Error: name cannot be blank.\n";
        return false;
    }
    return true;
}

// Look up a building through the graph and identify unknown names.
FacilityNode* ReadBuilding(const char* prompt) {
    string name;
    if (!ReadName(prompt, name)) {
        return nullptr;
    }
    FacilityNode* building = graph.get(name);
    if (!building) {
        cout << "Error: unknown building: " << name << '\n';
    }
    return building;
}

void runCommand(int choice) { //Function to run commands based on user input
    if (handleDepartmentOption(choice, departmentTree, MenuPhase::Final, cin, cout)) {
        return;
    }

    switch (choice) {
        case 1: {
            database.displayAll();
            break;
        }
        case 2: {
            string id;
            cout << "Enter facility ID: ";
            getline(cin, id);
            const Facility* facility = database.findById(id);
            if (facility) {
                PrintFacility(*facility);
                cout << "Facility found.\n";
            } else {
                cout << "Facility not found.\n";
            }
            break;
        }
        case 3:
        case 4: {
            string name;
            if (ReadName(choice == 3 ? "Enter facility name: " : "Enter category: ", name)) {
                const auto matches = choice == 3 ? database.findByName(name)
                                                 : database.findByCategory(name);
                if (matches.empty()) {
                    cout << "No matching facilities found.\n";
                }
                for (const Facility& facility : matches) {
                    PrintFacility(facility);
                }
                cout << "Matches: " << matches.size() << '\n';
            }
            break;
        }
        case 5: {
            graph.display_connections();
            break;
        }
        case 6: {
            graph.connectivity_report();
            break;
        }
        case 7: {
            if (FacilityNode* startNode = ReadBuilding("Enter starting building name: ")) {
                graph.reachability(*startNode);
            }
            break;
        }
        case 8: {
            FacilityNode* start = ReadBuilding("Enter starting building name: ");
            if (start) {
                if (FacilityNode* destination = ReadBuilding("Enter destination building name: ")) {
                    graph.shortest_path(*start, *destination);
                }
            }
            break;
        }
        case 16: {
            database.printCategoryReport();
            break;
        }
        case 17: {
            graph.connectivity_report();
            break;
        }
        default: {
            cout << "Invalid choice. Please try again.\n";
            break;
        }
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



// Load the starter data or a test directory, then run the menu until exit or EOF.
int main(int argc, char* argv[]) {
    const filesystem::path dataDirectory = argc > 1 ? argv[1] : "data";
    const string buildingsFile = (dataDirectory / "buildings.txt").string();
    vector<string> buildings = LoadBuildings(buildingsFile);
    graph.loadBuildings(buildingsFile);
    graph.loadConnections((dataDirectory / "connections.txt").string());
    database.load((dataDirectory / "facilities.txt").string(), buildings);
    departmentTree.loadFile((dataDirectory / "departments.txt").string(), cout);

    while (cin) {
        PrintMenu();
        cout << "Enter your choice: ";
        int choice;
        if (!readMenuChoice(cin, choice)) {
            if (!cin) {
                cout << '\n';
                break;
            }
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }
        if (choice == exitOption || choice == 0) {
            break;
        }
        runCommand(choice);
    }

    cout << "Exiting program.\n";
    return 0;
}
