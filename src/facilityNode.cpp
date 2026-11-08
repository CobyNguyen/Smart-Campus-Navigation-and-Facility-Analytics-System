#include "../include/facilityNode.h"

using namespace std;

//Constructors
FacilityNode::FacilityNode(){
    name = "none";
}

FacilityNode::FacilityNode(string newName){
    name = newName;
}

//Setters and getters
void FacilityNode::set_name(string newName){
    name = newName;
}

string FacilityNode::get_name() const{
    return name;
}

vector<FacilityNode*> FacilityNode::get_connections(){
    return connections;
}

//Other methods
bool FacilityNode::has_connection(FacilityNode* in){
    for (int i = 0; i < connections.size(); i++){
        if (connections.at(i) == in){
            return true;
        }
    }

    return false;
}

bool FacilityNode::add_connection(FacilityNode* in){
    if (!has_connection(in)) {
        //cout << "Connecting node: " << name << " to " << in -> get_name() << endl;
        connections.push_back(in); //Add the target node to our connections
        return true;
    } else {
        return false;
    }
}

//Overrides the print operator so nodes get printed with their connections
void FacilityNode::print(){
    cout << name << " : ";
    
    if (connections.size() > 0){
        for (int i = 0; i < connections.size() - 1; i++){
        cout << connections.at(i) -> get_name() << " | ";
        }

        cout << connections.back() -> get_name();
    }
    else{
        cout << "N/A";
    }

    cout << endl;

    //cout << name << endl;
}

bool FacilityNode::operator==(const FacilityNode& other) const {
    return this->name == other.name; // Compare unique identifiers
}