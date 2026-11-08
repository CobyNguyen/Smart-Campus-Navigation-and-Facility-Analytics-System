#include "../include/facilityGraph.h"

using namespace std;

//Constructors
FacilityGraph::FacilityGraph(){
    
}

//Other methods
void FacilityGraph::display_connections(){
    cout << endl << "Connectivity Report: " << endl;

    if (graph.size() < 1){
        cout << "Graph is empty!" << endl;
    }

    for (int i = 0; i < graph.size(); i++){
        cout << i << ": ";
        graph.at(i).print();
    }

    //Print adjacency matrix
    
    //cout << "\t  ";
    //for (int i = 0; i < graph.size(); i++){
    //    cout << graph.at(i).get_name() << " ";
    //}
    //cout << endl;

    //Compares each node with every other node to create the matrix
    //for (int row = 0; row < graph.size(); row++){
    //    cout << "\t" << graph.at(row).get_name() << " ";
    //    for (int col = 0; col < graph.size(); col++){
    //        if (graph.at(row).has_connection(&graph.at(col))){
    //            cout << "X ";
    //        }
    //        else{
    //            cout << ". ";
    //        }
    //    }
    //    cout << endl;
    //}
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
    cout << endl;
    connect_nodes("W", "A");
    connect_nodes("A", "E");
    connect_nodes("Q", "N");
    cout << endl;
    connect_nodes("A", "A");
    cout << endl;
    cout << "Displaying connectivity:" << endl;
    display_connections();
}