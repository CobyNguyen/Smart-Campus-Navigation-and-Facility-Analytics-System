//This is a class that stores the facility name, and its connections

#include <vector> 
#include <string> 
#include <iostream> 

using namespace std;

class FacilityNode{
    private:
        string name;
        vector<FacilityNode*> connections;

    public:
        FacilityNode();
        FacilityNode(string name);

        void set_name(string in);
        string get_name() const;
        
        vector<FacilityNode*> get_connections();
        bool has_connection(FacilityNode* in);
        bool add_connection(FacilityNode* in);

        void print();
        bool operator==(const FacilityNode& other) const;
};