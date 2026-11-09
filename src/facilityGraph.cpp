#include "../include/facilityGraph.h"
#include <cstddef>
#include <fstream>
#include <queue>
#include <sstream>

using namespace std;

namespace {
string trim(const string& value) {
    const string whitespace = " \t\r\n";
    const int first = static_cast<int>(value.find_first_not_of(whitespace));
    if (first < 0) {
        return "";
    }
    const int last = static_cast<int>(value.find_last_not_of(whitespace));
    return value.substr(first, last - first + 1);
}
}

//Constructors
FacilityGraph::FacilityGraph(){
    
}

//Other methods
void FacilityGraph::display_connections(){
    cout << endl << "Connectivity Report: " << endl;
    cout << endl;

    if (graph.size() < 1){
        cout << "No buildings!" << endl;
        return;
    }
    
    cout << "Total buildings: " << graph.size() << endl;
    cout << endl;

    //sum of degrees theorem
    int sum = 0;
    for (int i = 0; i < graph.size(); i++){
        sum += graph.at(i).get_connections().size();
    }

    sum /= 2;

    cout << "Total connections: " << sum << endl;
    cout << endl;

    int maxDegree = 0;
    for (int i = 0; i < graph.size(); i++){
        int degree = graph.at(i).get_connections().size();
        if (degree > maxDegree){
            maxDegree = degree;
        }
    }

    cout << "Building(s) with highest degree (" << maxDegree << "):" << endl;

    for (int i = 0; i < graph.size(); i++){
        int degree = graph.at(i).get_connections().size();
        if (degree == maxDegree){
            cout << " - " << graph.at(i).get_name() << endl;
        }
    }

    cout << endl;

    cout << "Isolated buildings: " << endl;

    bool hasIsolated = false;
    for (int i = 0; i < graph.size(); i++){
        int degree = graph.at(i).get_connections().size();
        if (degree == 0){
            cout << " - " << graph.at(i).get_name() << endl;
            hasIsolated = true;
        }
    }

    if (!hasIsolated){
        cout << " - None" << endl;
    }

    cout << endl;

    cout << "Connected components: " << "N/A" << endl;

    cout << endl;

    //for (int i = 0; i < graph.size(); i++){
    //    graph.at(i).print();
    //}

    //cout << endl;

}

bool FacilityGraph::has_node(const FacilityNode &in){
    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i) == in){
            return true;
        }
    }

    return false;
}

bool FacilityGraph::add_node(const FacilityNode &in){
    if (!has_node(in)) {
        cout << "Added " << in.get_name() << " to graph" << endl;
        graph.push_back(in); //Add the node to our graph if the node isnt already in the graph
        return true;
    } else {
        cout << in.get_name() << " already in graph" << endl;
        return false;
    }
}

bool FacilityGraph::add_node(string in){
    FacilityNode newNode = FacilityNode(in);

    if (!has_node(newNode)) {
        cout << "Added " << newNode.get_name() << " to graph" << endl;
        graph.push_back(newNode); //Add the node to our graph if the node isnt already in the graph
        return true;
    } else {
        cout << newNode.get_name() << " already in graph" << endl;
        return false;
    }
}

bool FacilityGraph::connect_nodes(FacilityNode &nodeA, FacilityNode &nodeB){
    return nodeA.add_connection(&nodeB) && nodeB.add_connection(&nodeA); 
    //Because we want an undirected graph, we need to add the symmetric edge as well
}

bool FacilityGraph::connect_nodes(string nodeNameA, string nodeNameB){
    FacilityNode* nodeRefA = nullptr;
    FacilityNode* nodeRefB = nullptr;

    bool foundA = false;
    bool foundB = false;

    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i).get_name() == nodeNameA){
            nodeRefA = &graph.at(i);
            foundA = true;
        }

        if (graph.at(i).get_name() == nodeNameB){
            foundB = true;
            nodeRefB = &graph.at(i);
        }
    }

    if (nodeRefA == nullptr && nodeRefB == nullptr){
        cout << nodeNameA << " and " << nodeNameB << " not in graph" << endl;
        return false;
    }

    if (nodeRefA == nullptr){
        cout << nodeNameA << " not in graph" << endl;
        return false;
    }

    if (nodeRefB == nullptr){
        cout << nodeNameB << " not in graph" << endl;
        return false;
    }


    //Because we want an undirected graph, we need to add the symmetric edge as well
    if (nodeRefA == nodeRefB){
        nodeRefA -> add_connection(nodeRefB);
        cout << "Connecting " << nodeRefA -> get_name() << " to self" << endl;
        return true;
    }
    else{
        bool connectAB = nodeRefA -> add_connection(nodeRefB);
        bool connectBA = nodeRefB -> add_connection(nodeRefA);
        
        if (connectAB || connectBA){
            cout << "Connecting " << nodeRefA -> get_name() << " and " << nodeRefB -> get_name() << endl;
            return true;
        }
        else{
            cout << nodeRefA -> get_name() << " and " << nodeRefB -> get_name() << " already connected" << endl;
        return false;
        }  
    }
}

bool FacilityGraph::loadBuildings(const string& filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << endl;
        return false;
    }

    graph.clear();
    string line;
    while (getline(input, line)) {
        string building = trim(line);
        if (building.empty() || building.front() == '#') {
            continue;
        }
        add_node(building);
    }

    cout << "Loaded " << filename << ": "
         << graph.size() << " buildings" << endl;
    return true;
}

bool FacilityGraph::loadConnections(const string& filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << endl;
        return false;
    }

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;
    string line;
    while (getline(input, line)) {
        ++lineNumber;
        string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine.front() == '#') {
            continue;
        }

        string first;
        string second;
        stringstream fields(trimmedLine);
        getline(fields, first, '|');
        getline(fields, second, '|');
        first = trim(first);
        second = trim(second);

        if (first.empty() || second.empty() || first == second ||
            !connect_nodes(first, second)) {
            ++rejected;
            cerr << "Warning: " << filename << ':' << lineNumber
                 << ": invalid, duplicate, or unknown connection" << endl;
        } else {
            ++accepted;
        }
    }

    cout << "Loaded " << filename << ": "
         << accepted << " accepted, " << rejected << " rejected" << endl;
    return true;
}

int FacilityGraph::search(string findName){ //case insensitive searching
    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i).get_name() == findName){
            return i;
        }
    }
    return -1;
}

FacilityNode* FacilityGraph::get(string findName){ //case insensitive searching
    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i).get_name() == findName){
            return &graph.at(i);
        }
    }
    return nullptr;
}


bool add_node_to_BFS_queue(FacilityNode &in, queue<FacilityNode*> &BFSqueue, vector<FacilityNode*> &visited, vector<int> &distances, int currentDist){
    bool hasBeenVisited = false;
   
    //check to see if node has already been visited
    for (int i = 0; i < visited.size(); i++){
        if (visited.at(i) == &in){
            hasBeenVisited = true;
        }
    }

    //if it hasn't been visited, add node to the queue and visited list
    if(!hasBeenVisited){
        BFSqueue.push(&in);
        visited.push_back(&in);
        distances.push_back(currentDist);
    }

    return !hasBeenVisited;
}

//handle null inputs outside of method
void FacilityGraph::reachability(FacilityNode &in){ //breadth first search to find reachability
    queue<FacilityNode*> BFSqueue;
    vector<FacilityNode*> visited;
    vector<int> distances;
    int distance = 0;

    add_node_to_BFS_queue(in, BFSqueue, visited, distances, distance);

    distance++;

    while(BFSqueue.size() > 0){
        FacilityNode* current = BFSqueue.front();
        vector<FacilityNode*> nextToAdd = current -> get_connections();
        bool wasAdded = false;

        for(int i = 0; i < nextToAdd.size(); i++){
            if (add_node_to_BFS_queue(*nextToAdd.at(i), BFSqueue, visited, distances, distance)){
                wasAdded = true;
            }
        }
        if (wasAdded){
            distance++;
        }
        BFSqueue.pop();
    }

    cout << "Reachability of " << in.get_name() << endl;
    cout << "Total reachable: " << visited.size() << endl;

    for (int i = 0; i < visited.size(); i++){
        cout << visited.at(i)->get_name() << " dist(" << distances.at(i) << ") ";
    }

    cout << endl << endl;
}

//handle null inputs outside of method
void FacilityGraph::shortest_path(FacilityNode &start, FacilityNode &target){
    if (start == target){
        return;
    }
    
    queue<FacilityNode*> BFSqueue;
    vector<FacilityNode*> visited;
    vector<FacilityNode*> parents;
    vector<FacilityNode*> path;
    vector<int> distances;
    int distance = 0;

    add_node_to_BFS_queue(start, BFSqueue, visited, distances, distance);
    parents.push_back(&start);
    distance++;

    while(BFSqueue.size() > 0){
        FacilityNode* current = BFSqueue.front();
        vector<FacilityNode*> nextToAdd = current -> get_connections();
        bool wasAdded = false;

        for(int i = 0; i < nextToAdd.size(); i++){
            if (add_node_to_BFS_queue(*nextToAdd.at(i), BFSqueue, visited, distances, distance)){
                parents.push_back(current);
                wasAdded = true;

                if (nextToAdd.at(i) == &target){ //Found target
                    
                    FacilityNode* traverse = visited.back(); //start at found target node
                    while (traverse != &start){
                        //find index of the traverse node
                        int index = -1;
                        for (int i = 0; i < visited.size(); i++){
                            if (visited.at(i) == traverse){
                                index = i;
                                break;
                            }
                        }
                        //find parent of traverse node and set traverse to its parent
                        path.push_back(traverse);
                        traverse = parents.at(index);
                    }

                    path.push_back(traverse);

                    cout << "Shortest path from " << start.get_name() << " to " << target.get_name() << ": " << endl;
                    for (int i = path.size() - 1; i >= 0; i--){
                        cout << path.at(i)->get_name() << " ";
                    }
                    cout << endl;

                    return;
                }
            }
        }
        if (wasAdded){
            distance++;
        }
        BFSqueue.pop();
    }

    //completed BFS

    cout << "No path from " << start.get_name() << " to " << target.get_name() << endl;

}


void FacilityGraph::unit_test(){
    cout << "_UNIT TESTING FACILITY GRAPH_" << endl;

    cout << "Adding nodes:" << endl;
    add_node("A");
    add_node("B");
    add_node("C");
    add_node("D");
    add_node("E");
    add_node("F");
    add_node("G");

    add_node("A");
    cout << endl;

    cout << "Connecting nodes:" << endl;
    connect_nodes("A", "C");
    connect_nodes("B", "C");
    connect_nodes("A", "C");
    connect_nodes("A", "E");
    connect_nodes("C", "F");
    connect_nodes("D", "E");
    connect_nodes("F", "D");
    cout << endl;
    connect_nodes("W", "A");
    connect_nodes("Q", "N");
    cout << endl;
    connect_nodes("A", "A");
    cout << endl;

    display_connections();

    if (search("A") != -1){ //Always do checks like this before using the search result
        reachability(graph.at(search("A")));
    }
    if (search("G") != -1){ 
        reachability(graph.at(search("G")));
    }
    shortest_path(graph.at(search("A")), graph.at(search("D")));
}