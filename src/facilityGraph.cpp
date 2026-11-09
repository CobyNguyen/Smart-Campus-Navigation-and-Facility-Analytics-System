#include "../include/facilityGraph.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//Constructors
FacilityGraph::FacilityGraph(){
    
}

//Other methods
void FacilityGraph::display_connections(){
    cout << "Connectivity Report: " << endl;

    //if (graph.size() < 1){
    //    cout << "Graph is empty!" << endl;
    //}

    //for (int i = 0; i < graph.size(); i++){
    //    cout << i << ": ";
    //    graph.at(i).print();
    //}

    //Print adjacency matrix

    cout << "\t  ";
    for (int i = 0; i < graph.size(); i++){
        cout << graph.at(i).get_name() << " ";
    }
    cout << endl;

    for (int row = 0; row < graph.size(); row++){
        cout << "\t" << graph.at(row).get_name() << " ";
        for (int col = 0; col < graph.size(); col++){
            if (graph.at(row).has_connection(&graph.at(col))){
                cout << "1 ";
            }
            else{
                cout << "0 ";
            }
        }
        cout << endl;
    }
}

bool FacilityGraph::has_node(const FacilityNode &in){
    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i).get_name() == in.get_name()){
            return true;
        }
    }

    return false;
}

bool FacilityGraph::add_node(const FacilityNode &in){
    if (!has_node(in)) {
        cout << "Added node: " << in.get_name() << " to graph." << endl;
        graph.push_back(in); //Add the node to our graph if the node isnt already in the graph
        return true;
    } else {
        cout << "Node: " << in.get_name() << " already in graph." << endl;
        return false;
    }
}

bool FacilityGraph::add_node(string in){
    FacilityNode newNode = FacilityNode(in);

    if (!has_node(newNode)) {
        cout << "Added node: " << newNode.get_name() << " to graph." << endl;
        graph.push_back(newNode); //Add the node to our graph if the node isnt already in the graph
        return true;
    } else {
        cout << "Node: " << newNode.get_name() << " already in graph." << endl;
        return false;
    }
}

bool FacilityGraph::connect_nodes(FacilityNode &nodeA, FacilityNode &nodeB){
    return nodeA.add_connection(&nodeB) && nodeB.add_connection(&nodeA); 
    //Because we want an undirected graph, we need to add the symmetric edge as well
}

bool FacilityGraph::connect_nodes(string nodeNameA, string nodeNameB){
    FacilityNode* nodeRefA;
    FacilityNode* nodeRefB;

    for (int i = 0; i < graph.size(); i++){
        if (graph.at(i).get_name() == nodeNameA){
            nodeRefA = &graph.at(i);
        }
        if (graph.at(i).get_name() == nodeNameB){
            nodeRefB = &graph.at(i);
        }
    }
    
    return nodeRefA -> add_connection(nodeRefB) && nodeRefB -> add_connection(nodeRefA); 
    //Because we want an undirected graph, we need to add the symmetric edge as well
}