#include "../include/facilityGraph.h"
#include "../include/facilityDatabase.h"
#include "../include/DepartmentMenu.h"
#include "../include/departmentTree.h"
#include <iostream>  
#include <string>
#include <vector>
#include <fstream>

using namespace std;

FacilityDatabase database; //Creates global instance of both facility database and class
FacilityGraph graph;
DepartmentTree departmentTree;

void PrintMenu() { //Menu
    cout << "====================\n";
    cout << "Menu Options:\n";
    cout << "1. View all facilities\n";
    cout << "2. Search facility by ID\n";
    cout << "3. View campus graph\n";
    cout << "4. Display graph counts and degrees\n";
    cout << "5. Find reachable buildings (BFS)\n";
    cout << "6. Add department\n";
    cout << "7. Search department\n";
    cout << "8. Display department hierarchy\n";
    cout << "9. Run preorder traversal\n";
    cout << "0. Exit\n";
    cout << "====================\n";
}

void PrintFacility(const Facility& facility) {
    cout << facility.getId() << " | "
         << facility.getName() << " | "
         << facility.getBuilding() << " | "
         << facility.getCategory() << "\n";
}

void runCommand(int choice) { //Function to run commands based on user input
    switch (choice) {
        case 1: {
            database.displayAll();
            break;
        }
        case 2: {
            string id;
            cout << "Enter facility ID: ";
            cin >> id;
            const Facility* facility = database.findById(id);
            if (facility) {
                PrintFacility(*facility);
            } else {
                cout << "Facility not found.\n";
            }
            break;
        }
        case 3: {
            graph.display_connections();
            break;
        }
        case 4: {
            graph.connectivity_report();
            break;
        }
        case 5: {
            string buildingName;
            cout << "Enter starting building name: ";
            cin >> buildingName;
            cin.ignore(); // Clear the newline character from the input buffer
            FacilityNode* startNode = graph.get(buildingName);
            if (startNode) {
                graph.reachability(*startNode);
            } else {
                cout << "Building not found in the graph.\n";
            }
            break;
        }
        case 6: {
            string name, parentName;
            cout << "Enter department name: ";
            cin.ignore(); // Clear the newline character from the input buffer
            getline(cin, name);
            cout << "Enter parent department name (or leave blank for root): ";
            getline(cin, parentName);
            string result = departmentTree.addDepartment(name, parentName);
            if (!result.empty()) {
                cout << "Error: " << result << "\n";
            } else {
                cout << "Department added successfully.\n";
            }
            break;
        }
        case 7: {
            string name;
            cout << "Enter department name to search: ";
            cin.ignore(); // Clear the newline character from the input buffer
            getline(cin, name);
            departmentTree.printDepartment(name, cout);
            break;
        }
        case 8: {
            departmentTree.printHierarchy(cout);
            break;
        }
        case 9: {
            departmentTree.printTraversal(DepartmentTree::Traversal::Preorder, cout);
            break;
        }
        case 0: {
            cout << "Exiting program.\n";
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



int main(){

    vector<string> buildings = LoadBuildings("data/buildings.txt");
    graph.loadBuildings("data/buildings.txt");
    graph.loadConnections("data/connections.txt");
    database.load("data/facilities.txt", buildings);
    departmentTree.loadFile("data/departments.txt", cout);

    int choice = -1;

    while (choice != 0) {
        PrintMenu();
        cout << "Enter your choice: ";
        if (!readMenuChoice(cin, choice)) {
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }
        runCommand(choice);
    }

    return 0;
}