#include "../include/facilityNode.h"

#include <queue>

using namespace std;

class FacilityGraph{
    private:
        vector<FacilityNode> graph;

    public:
        FacilityGraph();

        void display_connections();
        bool has_node(const FacilityNode &in);
        bool has_node(string in);
        bool add_node(const FacilityNode &in);
        bool add_node(string in);
        bool connect_nodes(FacilityNode &a, FacilityNode &b);
        bool connect_nodes(string a, string b);
        
        int search(string findName);
        void reachability(FacilityNode &in);
        void shortest_path(FacilityNode &start, FacilityNode &target);
        FacilityNode* get(string findName);
        void unit_test();
};