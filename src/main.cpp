#include "../include/facilityGraph.h"

FacilityGraph graph;

int main(){
    graph.add_node("A");
    graph.add_node("B");
    graph.add_node("C");
    graph.add_node("D");

    graph.connect_nodes("A", "C");

    graph.display_connections();

    return 0;
}