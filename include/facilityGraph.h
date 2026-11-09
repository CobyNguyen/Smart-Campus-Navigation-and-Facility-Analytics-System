#include "../include/facilityNode.h"

using namespace std;

class FacilityGraph{
    private:
        vector<FacilityNode> graph;

    public:
        FacilityGraph();

        void display_connections();
        bool has_node(const FacilityNode &in);
        bool add_node(const FacilityNode &in);
        bool add_node(string in);
        bool connect_nodes(FacilityNode &a, FacilityNode &b);
        bool connect_nodes(string a, string b);
        int search(string findName);
        FacilityNode* get(string findName);
        bool loadBuildings(const string& filename);
        bool loadConnections(const string& filename);
        void reachability(FacilityNode &in);
        void shortest_path(FacilityNode &start, FacilityNode &target);
        void unit_test();
};