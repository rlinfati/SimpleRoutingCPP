#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <climits>
#include "evrp.h"
#include "evrp_io.hpp"

void eVRPRandomData(int t, int d, int c, int s, int v) {
    TRACEFUNC;
    std::cout << __FUNCTION__
    << " t = " << t
    << " d = " << d
    << " c = " << c
    << " s = " << s
    << " v = " << v
    << std::endl;
    
    double MAX_G = 100.0; // Number of decimals
    int MAX_X = 1000; // Max X
    int MAX_Y = 1000; // Max Y
    
    // setup
    NODES.resize(d+c+s);
    DEPOTS.resize(d+c+s);
    CUSTOMERS.resize(d+c+s);
    SERVICES.resize(d+c+s);
    VEHICLES.resize(v);
    
    // depot
    switch (t % 10) {
        case 1: // center
            std::cout << __FUNCTION__ << " depot-type 1 center" << std::endl;
            for (int i = 0; i < d; ++i) {
                NODES[i].x = MAX_X / 2;
                NODES[i].y = MAX_Y / 2;
                NODES[i].type = eVRPNode::depot;
            }
            break;
        case 2: // random
            std::cout << __FUNCTION__ << " depot-type 2 random" << std::endl;
            for (int i = 0; i < d; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::depot;
            }
            break;
            
        default:
            std::cout << __FUNCTION__ << " depot-type not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    
    t = t/10;
    
    // customer
    switch (t % 10) {
        case 1:
            std::cout << __FUNCTION__ << " customer-type 1 random" << std::endl;
            for (int i = d; i < d+c; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::customer;
            }
            break;
            
        case 2:
            std::cout << __FUNCTION__ << " customer-type 2 cluster" << std::endl;
        {
            int cs = 3 + rand() % 6; // UD3-8
            for (int i = d; i < d+cs; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::customer;
            }
            
            for (int i = d+cs; i < d+c; ++i) {
                do {
                    double dps = 0;
                    NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                    NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                    for (int csp = d; csp < d+cs; ++csp) {
                        dps += exp( -sqrt(pow(NODES[i].x - NODES[csp].x,2) + pow(NODES[i].y - NODES[csp].y,2)) / 40);
                    }
                    if ( rand()/double(INT_MAX) <= dps) break;
                } while ( true );
                NODES[i].type = eVRPNode::customer;
            }
        }
            break;
            
        case 3:
            std::cout << __FUNCTION__ << " customer-type 3 random+cluster" << std::endl;
            for (int i = d; i < d+c/2; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::customer;
            }
        {
            int cs = 3 + rand() % 6; // UD3-8
            for (int i = d+c/2; i < d+c/2+cs; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::customer;
            }
            
            for (int i = d+c/2+cs; i < d+c; ++i) {
                do {
                    double dps = 0;
                    NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                    NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                    for (int csp = d+c/2; csp < d+c/2+cs; ++csp) {
                        dps += exp( -sqrt(pow(NODES[i].x - NODES[csp].x,2) + pow(NODES[i].y - NODES[csp].y,2)) / 40);
                    }
                    if ( rand()/double(INT_MAX) <= dps) break;
                } while ( true );
                NODES[i].type = eVRPNode::customer;
            }
        }
            break;
            
        default:
            std::cout << __FUNCTION__ << " customer-type not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    
    t = t/10;
    
    // demand
    switch (t % 10) {
        case 1:
            std::cout << __FUNCTION__ << " demand-type 1 unitary" << std::endl;
            for (int i = d; i < d+c; ++i) {
                CUSTOMERS[i].dem = 1;
            }
            break;
            
        case 2:
            std::cout << __FUNCTION__ << " demand-type 2 small" << std::endl;
            for (int i = d; i < d+c; ++i) {
                CUSTOMERS[i].dem = 1 + rand() % int(10) ;
            }
            break;
            
        case 3:
            std::cout << __FUNCTION__ << " demand-type 3 large" << std::endl;
            for (int i = d; i < d+c; ++i) {
                CUSTOMERS[i].dem = 1 + rand() % int(100) ;
            }
            break;
            
        case 4:
            std::cout << __FUNCTION__ << " demand-type 4 quadrant" << std::endl;
            for (int i = d; i < d+c; ++i) {
                bool q = (NODES[i].x-MAX_X/2) * (NODES[i].y-MAX_Y/2) >= 0.0;
                std::cout << NODES[i].x << '\t' << NODES[i].y << '\t' << q << std::endl;
                CUSTOMERS[i].dem = (q ? 1 : 51) + rand() % int(50) ;
            }
            break;
            
        case 5:
            std::cout << __FUNCTION__ << " demand-type 5 80-20" << std::endl;
            for (int i = d; i < d+c; ++i) {
                bool q = (rand() / double(INT_MAX) ) <= 0.80;
                CUSTOMERS[i].dem = (q ? 1+rand() % int(10) : 51+rand() % int(50));
            }
            break;
            
        default:
            std::cout << __FUNCTION__ << " demand-type not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    
    t = t/10;
    
    // service
    switch (t % 10) {
        case 1:
            std::cout << __FUNCTION__ << " service-type 1 random" << std::endl;
            for (int i = d+c; i < d+c+s; ++i) {
                NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                NODES[i].type = eVRPNode::service;
            }
            break;
            
        case 2:
            std::cout << __FUNCTION__ << " service-type 2 cuasi-cluster" << std::endl;
        {
            for (int i = d+c; i < d+c+s; ++i) {
                do {
                    double dps = 0;
                    NODES[i].x = (rand()%int(MAX_X*MAX_G) ) / MAX_G;
                    NODES[i].y = (rand()%int(MAX_Y*MAX_G) ) / MAX_G;
                    for (int csp = d; csp < d+c; ++csp) {
                        dps += exp( -sqrt(pow(NODES[i].x - NODES[csp].x,2) + pow(NODES[i].y - NODES[csp].y,2)) / 40);
                    }
                    if ( rand()/double(INT_MAX) <= dps) break;
                } while ( true );
                NODES[i].type = eVRPNode::service;
            }
        }
            break;
            
        default:
            std::cout << __FUNCTION__ << " service-type not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    
    // vehicles
    double sq = std::accumulate(CUSTOMERS.begin(), CUSTOMERS.end(), 0,
                                [](int qq, eVRPCustomer cc) {
                                    return qq + cc.dem;
                                });
    std::vector <eVRPCustomer>::iterator imc = std::max_element(CUSTOMERS.begin(), CUSTOMERS.end(),
                                                                [](eVRPCustomer cl, eVRPCustomer cc) {
                                                                    return cl.dem < cc.dem;
                                                                });
    
    int vcap = ceil(sq / v) > imc->dem ? ceil (sq / v) : imc->dem;
    
    for (int i = 0; i < v; ++i) {
        VEHICLES[i].cap = vcap;
    }
    
    // verifica el v por bin packing heuristico sorted best fit
    int ContenedorCapacidad = VEHICLES[0].cap;
    std::vector <int> TamanoItem;
    for (int i = 0; i < (int)CUSTOMERS.size(); ++i) {
        if (CUSTOMERS[i].dem) {
            TamanoItem.push_back( CUSTOMERS[i].dem );
        }
    }
    std::sort(TamanoItem.begin(), TamanoItem.end(), std::greater<int>());
    
    std::vector <std::vector <size_t> > BF_Contenedores(TamanoItem.size());
    std::vector <int> BF_Usado(TamanoItem.size());
    for (size_t i = 0; i < TamanoItem.size(); ++i) {
        size_t BF_MasLleno_j = -1;
        int BF_MasLleno_x = INT_MIN;
        for (size_t j = 0; j < BF_Contenedores.size(); ++j) {
            if ( BF_Usado[j] + TamanoItem[i] > ContenedorCapacidad ) continue;
            if ( BF_Usado[j] > BF_MasLleno_x ) {
                BF_MasLleno_x = BF_Usado[j];
                BF_MasLleno_j = j;
            }
        }
        BF_Contenedores[ BF_MasLleno_j ].push_back( i );
        BF_Usado[ BF_MasLleno_j ] += TamanoItem[i];
    }
    
    int vheur = (int)std::count_if(BF_Usado.begin(), BF_Usado.end(),
                              [](int i){
                                  return i > 0;
                              });
    std::cout << __FUNCTION__ << " vehicle_v " << v << " vehicle_BP " << vheur << std::endl;
    
    if (vheur > v) {
        VEHICLES.resize(vheur);
        for (int i = 0; i < vheur; ++i) {
            VEHICLES[i].cap = vcap;
        }
    }
}

void eVRPDistance() {
    TRACEFUNC;
    
    int nn = (int)NODES.size();
    DISTANCE.resize(nn);
    for (int i = 0; i < nn; ++i) {
        DISTANCE[i].resize(nn);
        for (int j = 0; j < nn; ++j) {
            DISTANCE[i][j] = sqrt(pow(NODES[i].x - NODES[j].x,2) + pow(NODES[i].y - NODES[j].y,2));
        }
    }
}

void eVRPCleanUp() {
    TRACEFUNC;
    
    // setup
    NODES.resize(0);
    DEPOTS.resize(0);
    CUSTOMERS.resize(0);
    SERVICES.resize(0);
    VEHICLES.resize(0);
    DISTANCE.resize(0);
}

void eVRPWrite(std::string f) {
    TRACEFUNC;
    
    std::ofstream file;
    file.open(f.c_str());
    
    if ( ! file.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // setup
    int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    int nv = (int)VEHICLES.size();
    
    file << "NNODE \t" << nn << std::endl;
    file << "NDEPOT \t" << nd << std::endl;
    file << "NCUSTOMER \t" << nc << std::endl;
    file << "NSERVICE \t" << ns << std::endl;
    file << "NVEHICLE \t" << nv << std::endl;
    
    // depot
    file << "DEPOT_BEGIN" << std::endl;
    file << "id" << '\t' << "x" << '\t' << "y" << '\t' << std::endl;
    for (int i = 0; i < nn; ++i) {
        if (NODES[i].type != eVRPNode::depot) continue;
        file << i << '\t'
        << NODES[i].x << '\t'
        << NODES[i].y << '\t'
        << std::endl;
    }
    file << "DEPOT_END" << std::endl;
    
    // customer
    file << "CUSTOMER_BEGIN" << std::endl;
    file << "id" << '\t' << "x" << '\t' << "y" << '\t' << "dem" << '\t' << std::endl;
    for (int i = 0; i < nn; ++i) {
        if (NODES[i].type != eVRPNode::customer) continue;
        file << i << '\t'
        << NODES[i].x << '\t'
        << NODES[i].y << '\t'
        << CUSTOMERS[i].dem << '\t'
        << std::endl;
    }
    file << "CUSTOMER_END" << std::endl;
    
    // service
    file << "SERVICE_BEGIN" << std::endl;
    file << "id" << '\t' << "x" << '\t' << "y" << '\t' << std::endl;
    for (int i = 0; i < nn; ++i) {
        if (NODES[i].type != eVRPNode::service) continue;
        file << i << '\t'
        << NODES[i].x << '\t'
        << NODES[i].y << '\t'
        << std::endl;
    }
    file << "SERVICE_END" << std::endl;
    
    // service
    file << "VEHICLE_BEGIN" << std::endl;
    file << "id" << '\t' << "cap" << '\t' << std::endl;
    for (int i = 0; i < nv; ++i) {
        file << i << '\t'
        << VEHICLES[i].cap << '\t'
        << std::endl;
    }
    file << "VEHICLE_END" << std::endl;
    
    file.close();
}

void eVRPRead(std::string f) {
    TRACEFUNC;
    
    std::ifstream file;
    std::string tmpS;
    
    file.open(f.c_str());
    
    if ( ! file.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // setup
    int nn = 0;
    int nd = 0;
    int nc = 0;
    int ns = 0;
    int nv = 0;
    
    file >> tmpS >> nn;
    file >> tmpS >> nd;
    file >> tmpS >> nc;
    file >> tmpS >> ns;
    file >> tmpS >> nv;
    NODES.resize(nn);
    DEPOTS.resize(nd+nc+ns);
    CUSTOMERS.resize(nd+nc+ns);
    SERVICES.resize(nd+nc+ns);
    VEHICLES.resize(nv);
    
    if (nn != nd + nc + ns) {
        std::cout << "error nn != nd + nc + ns" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // depot
    file >> tmpS; // DEPOT_BEGIN
    file >> tmpS >> tmpS >> tmpS; // header
    for (int i = 0; i < nd; ++i) {
        file >> tmpS >> NODES[i].x >> NODES[i].y;
        NODES[i].type = eVRPNode::depot;
    }
    file >> tmpS; // DEPOT_END
    
    // customer
    file >> tmpS; // CUSTOMER_BEGIN
    file >> tmpS >> tmpS >> tmpS >> tmpS; // header
    for (int i = nd; i < nd+nc; ++i) {
        file >> tmpS >> NODES[i].x >> NODES[i].y >> CUSTOMERS[i].dem;
        NODES[i].type = eVRPNode::customer;
    }
    file >> tmpS; // CUSTOMER_END
    
    // service
    file >> tmpS; // SERVICE_BEGIN
    file >> tmpS >> tmpS >> tmpS; // header
    for (int i = nd+nc; i < nd+nc+ns; ++i) {
        file >> tmpS >> NODES[i].x >> NODES[i].y;
        NODES[i].type = eVRPNode::service;
    }
    file >> tmpS; // SERVICE_END
    
    // vehicles
    file >> tmpS; // VEHICLE_BEGIN
    file >> tmpS >> tmpS; // header
    for (int i = 0; i < nv; ++i) {
        file >> tmpS >> VEHICLES[i].cap;
    }
    file >> tmpS; // VEHICLE_END
}

void eVRPWriteGML(std::string f) {
    TRACEFUNC;
    
    std::vector <std::string> MyRGBColors;
    MyRGBColors.push_back("#FF0000");
    MyRGBColors.push_back("#00FF00");
    MyRGBColors.push_back("#0000FF");
    MyRGBColors.push_back("#FFFF00");
    MyRGBColors.push_back("#00FFFF");
    MyRGBColors.push_back("#FF00FF");
    MyRGBColors.push_back("#5E5E5E");
    MyRGBColors.push_back("#000080");
    MyRGBColors.push_back("#000000");
    MyRGBColors.push_back("#008000");
    MyRGBColors.push_back("#008080");
    MyRGBColors.push_back("#800080");
    MyRGBColors.push_back("#800000");
    MyRGBColors.push_back("#643200");
    MyRGBColors.push_back("#C86400");
    MyRGBColors.push_back("#964B00");
    
    std::ofstream gml;
    gml.open(f.c_str());
    if ( ! gml.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // setup
    // int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    // int nv = (int)VEHICLES.size();
    
    gml << "graph [ hierarchic 1 directed 1" << std::endl;
    
    for (int i = 0; i < nd; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#FF0000\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    for (int i = nd; i < nd+nc; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#00FF00\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    for (int i = nd+nc; i < nd+nc+ns; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#0000FF\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    gml << "]" << std::endl;
    gml.close();
    
}

void eVRPPrintRoute(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << "r" << i << " =";
        for (size_t j = 0; j < s[i].size(); ++j) {
            switch (NODES[s[i][j]].type ) {
                case eVRPNode::depot:
                    std::cout << " d" << s[i][j];
                    break;
                case eVRPNode::customer:
                    std::cout << " c" << s[i][j];
                    break;
                case eVRPNode::service:
                    std::cout << " s" << s[i][j];
                    break;
                case eVRPNode::nodef:
                    std::cout << " n" << s[i][j];
                    break;
                default:
                    std::cout << " e" << j;
                    break;
            }
        }
        std::cout << " rdem " << sd[i].rdem;
        std::cout << " rlen " << sd[i].rlen;
        std::cout << std::endl;
    }
}

void eVRPCheckRouteData(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << "r" << i << " =";
        
        double clen = 0;
        int cdem = 0;
        
        for (size_t j = 0; j+1 < s[i].size(); ++j) {
            cdem += CUSTOMERS[ s[i][j] ].dem;
            clen += DISTANCE[ s[i][j] ][ s[i][j+1] ];
        }
        std::cout << " rdem-cdem " << sd[i].rdem - cdem;
        std::cout << " rlen-clen " << sd[i].rlen - clen;
        std::cout << std::endl;
    }
    
    double sdr = std::accumulate(sd.begin(), sd.end(), 0.0,
                                [](double qq, routeData cc) {
                                    return qq + cc.rdem;
                                });
    double sdc = std::accumulate(CUSTOMERS.begin(), CUSTOMERS.end(), 0.0,
                                 [](double qq, eVRPCustomer cc) {
                                     return qq + cc.dem;
                                 });
    std::cout << "sum-dem r-c " << sdr - sdc << std::endl;
}

void eVRPEvalSolution(Solution &s, SolutionData &sd, double &zval) {
    TRACEFUNC;
    
    (void)s;
    double sq = std::accumulate(sd.begin(), sd.end(), 0.0,
                                [](double qq, routeData cc) {
                                    return qq + cc.rlen;
                                });
    zval = sq;
}

void eVRPWriteGML(std::string f, Solution &s) {
    TRACEFUNC;
    
    std::vector <std::string> MyRGBColors;
    MyRGBColors.push_back("#FF0000");
    MyRGBColors.push_back("#00FF00");
    MyRGBColors.push_back("#0000FF");
    MyRGBColors.push_back("#FFFF00");
    MyRGBColors.push_back("#00FFFF");
    MyRGBColors.push_back("#FF00FF");
    MyRGBColors.push_back("#5E5E5E");
    MyRGBColors.push_back("#000080");
    MyRGBColors.push_back("#000000");
    MyRGBColors.push_back("#008000");
    MyRGBColors.push_back("#008080");
    MyRGBColors.push_back("#800080");
    MyRGBColors.push_back("#800000");
    MyRGBColors.push_back("#643200");
    MyRGBColors.push_back("#C86400");
    MyRGBColors.push_back("#964B00");
    
    std::ofstream gml;
    gml.open(f.c_str());
    if ( ! gml.is_open() ) {
        std::cout << "file not open" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // setup
    // int nn = (int)NODES.size();
    int nd = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::depot; } );
    int nc = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::customer; } );
    int ns = (int)std::count_if(NODES.begin(), NODES.end(), [](eVRPNode n){return n.type == eVRPNode::service; } );
    // int nv = (int)VEHICLES.size();
    
    gml << "graph [ hierarchic 1 directed 1" << std::endl;
    
    for (int i = 0; i < nd; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#FF0000\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    for (int i = nd; i < nd+nc; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#00FF00\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    for (int i = nd+nc; i < nd+nc+ns; ++i) {
        gml << "node [ id " << i
        << " graphics [ x " << NODES[i].x << " y " << NODES[i].y
        << " w 11 h 11 type \"roundrectangle\" fill \"#0000FF\"] "
        << " LabelGraphics [text \"" << i << "\" fontSize 7 ] ]" << std::endl;
    }
    
    for (int i = 0; i < (int)s.size(); ++i) {
        for (size_t j = 0; j+1 < s[i].size(); ++j) {
            gml << "edge [ source " << s[i][j] << " target " << s[i][j+1] << " graphics [ fill \""
            << MyRGBColors[i % MyRGBColors.size() ]
            << "\" width 2 targetArrow \"standard\" ] ]" << std::endl;
        }
    }
    
    gml << "]" << std::endl;
    gml.close();
}

