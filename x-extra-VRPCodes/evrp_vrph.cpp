#include <iostream>
#include <fstream>
#include <algorithm>
#include "lib/VRPH-1.0.0/inc/VRPH.h"
#include "evrp_vrph.hpp"

void eVRPvrph(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    VRPWriteData("evrp-test-vrph.vrp");
    VRPCallVRPH("evrp-test-vrph.vrp", "evrp-test-vrph.sol", "");
    VRPReadSolution(s, "evrp-test-vrph.sol");

    sd.resize( s.size() );
    for (size_t i = 0; i < s.size(); ++i) {
        int cdem = 0;
        double clen = 0;
        for (size_t j = 0; j+1 < s[i].size(); ++j) {
            cdem += CUSTOMERS[ s[i][j] ].dem;
            clen += DISTANCE[ s[i][j] ][ s[i][j+1] ];
        }
        sd[i].rdem = cdem;
        sd[i].rlen = clen;
    }
}

void vrphl(char *input, char *output, char *initial) {
    TRACEFUNC;
    
    bool fruity_loop = true;
    bool enable_sa = true;
    bool enable_rtr = true;
    bool enable_ej = false;
    
    if (!input) return;
    
    double tini = clock();
    
    int n = VRPGetDimension(input);
    VRP V(n);
    V.read_TSPLIB_file(input);
    
    int num_ejected = n-2 < 20 ? n-2 : 20;
    int num_trials = 1000;
    
    double best_obj = -1;
    int *best_sol = new int[n+2];
    if (initial) {
        V.read_solution_file(initial);
        best_obj = V.get_total_route_length();
        V.export_solution_buff(best_sol);
        std::cout << "VRPH File " << V.get_total_route_length()
        << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    } else {
        ClarkeWright CW(n);
        CW.Construct(&V,
                     1.0, // lambda
                     false); // verbose
        best_obj = V.get_total_route_length();
        V.export_solution_buff(best_sol);
        std::cout << "VRPH ClarkeWright " << V.get_total_route_length()
        << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    }
    
    if (fruity_loop) std::cout << "VRPH fruity_loop mode" << std::endl;
    do {
        double current_sol = best_obj;
        
        do {
            if ( ! enable_sa ) break;
            V.SA_solve(ONE_POINT_MOVE+TWO_POINT_MOVE+TWO_OPT, // heuristics
                       2, // starting_temperature
                       .99, // cooling_ratio
                       2, // iters_per_loop
                       200, // num_loops
                       10, // nlist_size
                       false); // verbose
            if ( best_obj > V.get_total_route_length() ) {
                best_obj = V.get_total_route_length();
                std::cout << "VRPH SA " << V.get_total_route_length()
                << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                V.export_solution_buff(best_sol);
            } else {
                V.import_solution_buff(best_sol);
                break;
            }
        } while (fruity_loop);
        
        do {
            if ( ! enable_rtr ) break;
            V.RTR_solve(ONE_POINT_MOVE+TWO_POINT_MOVE+TWO_OPT+VRPH_USE_NEIGHBOR_LIST, // heuristics
                        40, // intensity (inicial 30)
                        5, // max_tries
                        2, // num_perturbs
                        .01, // dev
                        40, // nlist_size
                        VRPH_LI_PERTURB, // perturb_type
                        VRPH_FIRST_ACCEPT, // accept_type
                        false); // verbose
            if ( best_obj > V.get_total_route_length() ) {
                best_obj = V.get_total_route_length();
                std::cout << "VRPH RTR " << V.get_total_route_length()
                << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                V.export_solution_buff(best_sol);
            } else {
                V.import_solution_buff(best_sol);
                break;
            }
        } while (fruity_loop);
        
        do {
            if ( ! enable_ej ) break;
            double current_ej = best_obj;
            int *ejected_buff = new int[n+2];
            
            for(int i=0;i<num_trials;++i) {
                int r = VRPH_DEPOT;
                while(r == VRPH_DEPOT) r = (int)(lcgrand(11)*(n-1));
                
                V.eject_neighborhood(r, // j
                                     num_ejected, // num
                                     ejected_buff); // nodelist
                V.inject_set(num_ejected, // num
                             ejected_buff, // nodelist
                             VRPH_REGRET_SEARCH, // rules
                             50); // attempts
                
                if ( best_obj > V.get_total_route_length() ) {
                    best_obj = V.get_total_route_length();
                    std::cout << "VRPH EJ " << i << " Regret " << V.get_total_route_length()
                    << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                    V.export_solution_buff(best_sol);
                } else {
                    V.import_solution_buff(best_sol);
                }
            }
            delete[] ejected_buff;
            
            if (current_ej <= best_obj+1e-4) break;
        } while (fruity_loop);
        
        if (current_sol <= best_obj+1e-4) break;
    } while (fruity_loop);
    
    delete [] best_sol;
    std::cout << "* VRPH"
    << " route/best "
    << V.get_total_route_length() << "/" << V.get_best_known()
    << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    if (output) {
        V.write_solution_file(output);
    }
}   //fin de vrph

void VRPWriteData(std::string sd) {
    TRACEFUNC;
    
    // setup
    //int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    //int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    //int nv = (int)VEHICLES.size();
    
    std::ofstream vrp_in;
    vrp_in.open(sd.c_str());
    if ( ! vrp_in.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }

    vrp_in.setf(std::ios_base::fixed);
    vrp_in << "NAME: " << sd << std::endl;
    vrp_in << "BEST_KNOWN: " << "+inf" << std::endl;
    vrp_in << "COMMENT: " << "vrph" << std::endl;
    vrp_in << "DIMENSION: " << nd+nc << std::endl;
    vrp_in << "CAPACITY: " << VEHICLES[0].cap << std::endl;
    vrp_in << "EDGE_WEIGHT_FORMAT: FUNCTION" << std::endl;
    vrp_in << "EDGE_WEIGHT_TYPE: EXACT_2D" << std::endl;
    
    vrp_in << "NODE_COORD_SECTION" << std::endl;
    for (int i = 0; i < nd+nc; ++i) {
        vrp_in << i+1 << '\t' << NODES[i].x << '\t' << NODES[i].y << std::endl;
    }
    
    vrp_in << "DEMAND_SECTION" << std::endl;
    for (int i = 0; i < nd+nc; ++i) {
        vrp_in << i+1 << '\t' << CUSTOMERS[i].dem << std::endl;
    }
    
    vrp_in << "DEPOT_SECTION" << std::endl;
    vrp_in << "1" << std::endl;
    vrp_in << "-1" << std::endl;
    vrp_in << "EOF" << std::endl;
    vrp_in.close();
}

void VRPCallVRPH(std::string sp, std::string ss, std::string si) {
    TRACEFUNC;
    
    vrphl(ss.empty() ? NULL : const_cast<char *> (sp.c_str()),
          ss.empty() ? NULL : const_cast<char *> (ss.c_str()),
          si.empty() ? NULL : const_cast<char *> (si.c_str()) );
    
}

void VRPReadSolution(Solution &s, std::string ss) {
    TRACEFUNC;
    
    s.clear();
    
    std::ifstream vrp_sol;
    vrp_sol.open(ss.c_str());
    if ( ! vrp_sol.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int vrp_Ncustomer = 0;
    vrp_sol >> vrp_Ncustomer;
    for (int i = 0; i < vrp_Ncustomer ; ++i) {
        int dato = 0;
        vrp_sol >> dato;
        
        if (dato > 0)
            s.back().push_back( dato );
        else {
            if (s.size()) s.back().push_back( 0 );
            s.push_back( route() );
            s.back().push_back( 0 );
            s.back().push_back( -dato );
        }
    }
    s.back().push_back( 0 );

    std::string tmp;
    
    vrp_sol >> tmp; // 0
    if (tmp != "0") std::cout << "ERROR VRPReadSolution 0 "  << ss << std::endl;
    
    vrp_sol >> tmp; // OBJ=
    vrp_sol >> tmp; // double
    vrp_sol >> tmp; // BEST_KNOWN=
    vrp_sol >> tmp; // string
    
    vrp_sol.close();
}

