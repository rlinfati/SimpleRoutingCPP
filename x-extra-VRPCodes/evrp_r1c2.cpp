#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include "evrp_r1c2.hpp"

void eVRPSr1c2(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    route tsplkh;
    TSPWriteData("evrp-test-r1c2.tsp", "evrp-test-r1c2.par", "evrp-test-r1c2.sol");
    TSPCallLKH("evrp-test-r1c2.par");
    TSPReadSolution(tsplkh, "evrp-test-r1c2.sol");
    
    {
        route r;
        r.push_back(0);
        
        routeData rd;
        rd.rlen = 0;
        rd.rdem = 0;
        
        s.push_back(r);
        sd.push_back(rd);
    }
    
    for (size_t i = 0; i < tsplkh.size(); ++i) {
        
        if (sd.back().rdem + CUSTOMERS[ tsplkh[i] ].dem > VEHICLES[0].cap) {
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
        
        sd.back().rlen += DISTANCE[ s.back().back() ][ tsplkh[i] ];
        sd.back().rdem += CUSTOMERS[ tsplkh[i] ].dem;
        s.back().push_back( tsplkh[i] );
    }
    sd.back().rlen += DISTANCE[ s.back().back() ][ 0 ];
    sd.back().rdem += 0;
    s.back().push_back(0);
}

void TSPWriteData(std::string sd, std::string sp, std::string ss) {
    TRACEFUNC;
    
    // setup
    //int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    //int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    //int nv = (int)VEHICLES.size();

    std::ofstream file_in;
    file_in.open(sd.c_str());
    if ( ! file_in.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    file_in.setf(std::ios_base::fixed);
    file_in << "NAME : " << sd << std::endl;
    file_in << "COMMENT : " << "r1c2" << std::endl;
    file_in << "TYPE : TSP" << std::endl;
    file_in << "DIMENSION : " << nc << std::endl;
    file_in << "EDGE_WEIGHT_TYPE : EUC_2D" << std::endl;
    file_in << "NODE_COORD_SECTION" << std::endl;
    for (size_t i = nd; i < (size_t)nd+nc; ++i) {
        file_in << i
        << '\t' << NODES[i].x
        << '\t' << NODES[i].y
        << std::endl;
    }
    file_in << "EOF" << std::endl;
    file_in.close();
    
    std::ofstream file_par;
    file_par.open(sp.c_str());
    if ( ! file_par.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    file_par << "PROBLEM_FILE = " << sd << std::endl;
    file_par << "TRACE_LEVEL = 0" << std::endl;
    file_par << "GAIN23 = NO" << std::endl;
    file_par << "RUNS = 1" << std::endl;
    file_par << "OUTPUT_TOUR_FILE " << ss << std::endl;
    file_par.close();
}

void TSPCallLKH(std::string sp) {
    TRACEFUNC;
    
    char lkh_exe[] = "lkh_main";
    char lkh_param[1024];
    
    strcpy(lkh_param, sp.c_str());
    
    char *lkh_argv[] = { lkh_exe , lkh_param };
    
    lkh_main(2 , lkh_argv);
}

void TSPReadSolution(route &s, std::string ss) {
    TRACEFUNC;
    
    s.clear();
    
    int lkh_nheader = 6;
    std::ifstream file_sol(ss.c_str());
    for (int k = 0; k < lkh_nheader; ++k) {
        char Mytemp[1024];
        file_sol.getline(Mytemp, 1024);
    }
    
    while (true) {
        int Mytemp;
        file_sol >> Mytemp;
        
        if (Mytemp == -1) break;
        
        s.push_back(Mytemp);
    }
    
    std::string lkh_temp_s;
    file_sol >> lkh_temp_s;
    if (lkh_temp_s != "EOF") std::cout << "ERROR ReadTSPSolution "  << ss << std::endl;
    
    file_sol.close();
}
