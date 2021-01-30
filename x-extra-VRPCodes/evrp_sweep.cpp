#include <iostream>
#include <cmath>
#include <algorithm>
#include "evrp_sweep.hpp"

void eVRPSweep(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    // setup
    // int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    // int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    // int nv = (int)VEHICLES.size();
    
    std::vector <angleTheta> theta;
    for (int i = nd; i < nd+nc; ++i) {
        double cx = NODES[i].x - NODES[0].x;
        double cy = NODES[i].y - NODES[0].y;
        double pt = atan2(cy, cx);
        
        theta.push_back(angleTheta(i, pt));
    }
    sort(theta.begin(), theta.end());
    
    {
        route r;
        r.push_back(0);
        
        routeData rd;
        rd.rlen = 0;
        rd.rdem = 0;

        s.push_back(r);
        sd.push_back(rd);
    }

    for (size_t i = 0; i < theta.size(); ++i) {
        
        if (sd.back().rdem + CUSTOMERS[ theta[i].id ].dem > VEHICLES[0].cap) {
            sd.back().rlen += DISTANCE[ s.back().back() ][ 0 ];
            sd.back().rdem += 0;
            s.back().push_back(0);
            
            route r;
            r.push_back(0);
            
            routeData rd;
            rd.rlen = 0;
            rd.rdem = 0;
            
            s.push_back(r);
            sd.push_back(rd);
        }
        
        sd.back().rlen += DISTANCE[ s.back().back() ][ theta[i].id ];
        sd.back().rdem += CUSTOMERS[ theta[i].id ].dem;
        s.back().push_back( theta[i].id );
    }
    sd.back().rlen += DISTANCE[ s.back().back() ][ 0 ];
    sd.back().rdem += 0;
    s.back().push_back(0);

}
