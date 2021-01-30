#include <iostream>
#include <algorithm>
#include <functional>
#include "evrp_saving.hpp"

void eVRPSaving(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    // setup
    // int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    // int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    // int nv = (int)VEHICLES.size();

    for (int i = nd; i < nd+nc; ++i) {
        route r;
        r.push_back(i);
        
        routeData rd;
        rd.rlen = DISTANCE[0][i] + DISTANCE[i][0];
        rd.rdem = CUSTOMERS[i].dem;
        
        s.push_back(r);
        sd.push_back(rd);
    }
    
    std::vector <savingCost> saving;
    double lamda = 1.0;
    
    for (int i = nd; i < nd+nc; ++i) {
        for (int j = i + 1; j < nd+nc; ++j) {
            double d = DISTANCE[0][i] + DISTANCE[0][j] - lamda * DISTANCE[i][j];
            saving.push_back(savingCost(int(i), int(j), d));
        }
    }
    
    sort(saving.begin(), saving.end(), std::greater<savingCost>());
    
    for (int i = 0; i < (int)saving.size(); ++i) {
        int r_i = -1;
        int r_ip = -1; // 1 == front, 2 == back
        int r_j = -1;
        int r_jp = -1;
        
        for (int j = 0; j < (int)s.size(); ++j) {
            if (s[j].front() == saving[i].i) {
                r_i = j;
                r_ip = 1;
            }
            if (s[j].back() == saving[i].i) {
                r_i = j;
                r_ip = 2;
            }
            if (s[j].front() == saving[i].j) {
                r_j = j;
                r_jp = 1;
            }
            if (s[j].back() == saving[i].j) {
                r_j = j;
                r_jp = 2;
            }
        }
        if (r_i == -1) continue;
        if (r_j == -1) continue;
        if (r_i == r_j) continue;
        
        int newdem = sd[r_i].rdem + sd[r_j].rdem;
        double newlen = sd[r_i].rlen + sd[r_j].rlen - saving[i].differ;
        
        if (newdem > VEHICLES[0].cap) continue;
        
        std::vector<int> newr1 = s[r_i];
        std::vector<int> newr2 = s[r_j];
        
        if (r_ip == 1) reverse(newr1.begin(), newr1.end());
        if (r_jp == 2) reverse(newr2.begin(), newr2.end());
        
        route newr;
        newr.insert(newr.end(), newr1.begin(), newr1.end());
        newr.insert(newr.end(), newr2.begin(), newr2.end());
        
        s[r_i] = newr;
        sd[r_i].rdem = newdem;
        sd[r_i].rlen = newlen;
        
        s.erase(s.begin()+r_j);
        sd.erase(sd.begin()+r_j);
    }
    
    for (size_t i = 0; i < s.size(); ++i) {
        s[i].insert(s[i].begin(), 0);
        s[i].insert(s[i].end(), 0);
    }    
}
