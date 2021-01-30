#ifndef evrp_h
#define evrp_h

#include <vector>
#include <algorithm>

struct eVRPNode {
    double x;
    double y;
    enum NodeType {nodef, depot, customer, service};
    NodeType type = eVRPNode::nodef;
};

struct eVRPDepot : eVRPNode {
};

struct eVRPCustomer : eVRPNode {
    int dem;
};

struct eVRPService : eVRPNode {
};

struct eVRPVehicle {
    int cap;
};

extern std::vector <eVRPNode> NODES;
extern std::vector <eVRPDepot> DEPOTS;
extern std::vector <eVRPCustomer> CUSTOMERS;
extern std::vector <eVRPService> SERVICES;
extern std::vector <eVRPVehicle> VEHICLES;
extern std::vector <std::vector<double> > DISTANCE;

typedef std::vector <int> route;
struct routeData {
    int rdem;
    double rlen;
};

typedef std::vector<route> Solution;
typedef std::vector<routeData> SolutionData;

struct TabuGranularAnnealing {
    // TS
    struct eVRPTabu {
        int a;
        int b;
        eVRPTabu(int i, int j) {
            a = i;
            b = j;
        }
        bool operator==(const eVRPTabu& b) const {
            if (this->a == b.a && this->b == b.b) return true;
            if (this->b == b.a && this->a == b.b) return true;
            return false;
        }
    };
    std::vector<eVRPTabu> ts_list;
    size_t ts_nList = 7*4;
    int typetabu = 0;
    
    bool isTabu(int a, int b) {
        if (typetabu == 0) return false;
        if (typetabu == 1) return true;
        
        auto it = std::find(ts_list.begin(), ts_list.end(), eVRPTabu(a, b));
        if (it == ts_list.end()) {
            return false;
        } else {
            return true;
        }
    }
    void addTabu(int a, int b) {
        if (typetabu == 0) return;
        if (typetabu == 1) return;
        
        auto it = std::find(ts_list.begin(), ts_list.end(), eVRPTabu(a, b));
        if ( it == ts_list.end() ) ts_list.push_back( eVRPTabu(a, b));
    }
    void cleanTabu() {
        if (typetabu == 0) return;
        if (typetabu == 1) return;
        
        while (ts_list.size() > ts_nList) {
            ts_list.erase( ts_list.begin() );
        }
    }
    // SA
    int typesa = 0;
    double sa_t = 1000;
    double sa_alpha = 0.99;
    int sa_down = 100;
};

#define TRACEFCALL0
#ifdef TRACEFCALL
#define TRACEFUNC std::cout << __PRETTY_FUNCTION__ << " * at " << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define TRACEFUNC
#endif

#endif /* evrp_h */
