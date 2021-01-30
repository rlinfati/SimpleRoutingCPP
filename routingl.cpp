#include <fstream>

#include <cstdlib>
#include <cmath>
#include <cstring>

#include "routingl.h"

void RoutingL::TSPRandomData(std::vector<TSPNode> &d, int n) {

    double RL_G = 100.0; // Number of decimals
    int RL_X = 1000; // Max X
    int RL_Y = 1000; // Max Y

    d.resize(n);

    for (size_t i = 0; i < d.size(); ++i) {
        d[i].id  = int(i+1u);
        d[i].x   = (rand()%int(RL_X*RL_G) ) / RL_G;
        d[i].y   = (rand()%int(RL_Y*RL_G) ) / RL_G;
    }
}

void RoutingL::TSPDistance(std::vector<TSPNode> &d, std::vector<std::vector<double > > &di){

    size_t n = d.size();

    di.resize(n);
    for (size_t i = 0; i < n; ++i) {
        di[i].resize(n);
        for (size_t j = 0; j < n; ++j) {
            di[i][j] = sqrt(pow(d[i].x - d[j].x,2) + pow(d[i].y - d[j].y,2));
        }
    }
}

void RoutingL::TSPWriteData(std::vector<TSPNode> &d, std::string sd, std::string sp, std::string ss) {

    std::ofstream file_in(sd.c_str());
    file_in.setf(std::ios_base::fixed);
    file_in << "NAME : " << sd << std::endl;
    file_in << "COMMENT : " << std::endl;
    file_in << "TYPE : TSP" << std::endl;
    file_in << "DIMENSION : " << d.size() << std::endl;
    file_in << "EDGE_WEIGHT_TYPE : EUC_2D" << std::endl;
    file_in << "NODE_COORD_SECTION" << std::endl;
    for (size_t i = 0; i < d.size(); ++i) {
        file_in << d[i].id
                << '\t' << d[i].x
                << '\t' << d[i].y
                << std::endl;
    }
    file_in << "EOF" << std::endl;
    file_in.close();

    std::ofstream file_par(sp.c_str());
    file_par << "PROBLEM_FILE = " << sd << std::endl;
    file_par << "TRACE_LEVEL = 0" << std::endl;
    file_par << "GAIN23 = NO" << std::endl;
    file_par << "RUNS = 1" << std::endl;
    file_par << "OUTPUT_TOUR_FILE " << ss << std::endl;
    file_par.close();
}

void RoutingL::TSPCallLKH(std::string sp) {

    char lkh_exe[] = "lkh_main";
    char lkh_param[1024];

    strcpy(lkh_param, sp.c_str());

    char *lkh_argv[] = { lkh_exe , lkh_param };

    liblkh(2 , lkh_argv);
}

void RoutingL::TSPReadSolution(std::vector <int> &s, std::string ss) {

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
    if (lkh_temp_s != "EOF") std::cout << "ERROR RoutingL::ReadTSPSolution "  << ss << std::endl;

    file_sol.close();
}

void RoutingL::TSPWriteGraph(std::vector<TSPNode> &d, std::vector <int> &s, std::string sg) {

    double GMLFactor = 10.0;
    int NDepot = 1;

    std::ofstream file_gml(sg.c_str());
    file_gml << "graph [ hierarchic 1 directed 1" << std::endl;
    for (int i = 0; i < NDepot; ++i) {
        file_gml << "node [ id " << d[i].id << " graphics [ x " << d[i].x * GMLFactor
                 << " y " << d[i].y * GMLFactor
                 << " w 11 h 11 type \"roundrectangle\" fill \"#FF6600\"] LabelGraphics [text \""
                 << d[i].id << "\" fontSize 7 ] ]" << std::endl;
    }

    for (size_t i = NDepot; i < d.size(); ++i) {
        file_gml << "node [ id " << d[i].id
                 << " graphics [ x " << d[i].x * GMLFactor
                 << " y " << d[i].y * GMLFactor
                 << " w 11 h 11 type \"roundrectangle\"] LabelGraphics [text \""
                 << d[i].id << "\" fontSize 7 ] ]" << std::endl;
    }

    std::vector<int>::iterator iter = s.begin();
    bool fin = 1;
    while (fin) {
        file_gml << "edge [ source " << *iter;
        if (++iter == s.end()){
            fin = 0;
            iter = s.begin();
        }
        file_gml << " target " << *iter
                 << " graphics [ fill \"#FF0000\" targetArrow \"standard\" ] ]"
                 << std::endl;
    }

    file_gml << "]" << std::endl;
    file_gml.close();
}

void RoutingL::VRPRandomData(std::vector<VRPNode> &d, int n) {
    double RL_G = 100.0; // Number of decimals
    int RL_X = 1000; // Max X
    int RL_Y = 1000; // Max Y

    double RL_D = 50.0; // Max Dem

    d.resize(n);

    for (size_t i = 0; i < d.size(); ++i) {
        d[i].id  = int(i+1u);
        d[i].x   = (rand()%int(RL_X*RL_G) ) / RL_G;
        d[i].y   = (rand()%int(RL_Y*RL_G) ) / RL_G;
        d[i].dem = (rand()%int(RL_D*RL_G) ) / RL_G;
    }
    d[0].dem = 0; // depot
}

void RoutingL::VRPDistance(std::vector<VRPNode> &d, std::vector<std::vector<double> > &di) {

    size_t n = d.size();

    di.resize(n);
    for (size_t i = 0; i < n; ++i) {
        di[i].resize(n);
        for (size_t j = 0; j < n; ++j) {
            di[i][j] = sqrt(pow(d[i].x - d[j].x,2) + pow(d[i].y - d[j].y,2));
        }
    }
}

void RoutingL::VRPWriteData(std::vector<VRPNode> &d, int c, std::string sd) {

    std::ofstream vrp_in( sd.c_str() );
    vrp_in.setf(std::ios_base::fixed);
    vrp_in << "NAME: " << sd << std::endl;
    vrp_in << "BEST_KNOWN: " << "+inf" << std::endl;
    vrp_in << "COMMENT: " << "comment" << std::endl;
    vrp_in << "DIMENSION: " << d.size() << std::endl;
    vrp_in << "CAPACITY: " << c << std::endl;
    vrp_in << "EDGE_WEIGHT_FORMAT: FUNCTION" << std::endl;
    vrp_in << "EDGE_WEIGHT_TYPE: EXACT_2D" << std::endl;

    vrp_in << "NODE_COORD_SECTION" << std::endl;
    for (size_t i = 0; i < d.size(); ++i) {
        vrp_in << d[i].id << '\t' << d[i].x << '\t' << d[i].y << std::endl;
    }

    vrp_in << "DEMAND_SECTION" << std::endl;
    for (size_t i = 0; i < d.size(); ++i) {
        vrp_in << d[i].id << '\t' << d[i].dem << std::endl;
    }

    vrp_in << "DEPOT_SECTION" << std::endl;
    vrp_in << "1" << std::endl;
    vrp_in << "-1" << std::endl;
    vrp_in << "EOF" << std::endl;
    vrp_in.close();

}

void RoutingL::VRPWriteSolution(std::vector<std::vector<int> > &s, std::string si, int n) {

    std::ofstream vrp_st( si.c_str() );

    vrp_st << n - 1 << " ";

    for (size_t i = 0; i < s.size() ; ++i) {
        vrp_st << -s[i][1]+1 << " ";
        for (size_t j = 2; j < s[i].size() ; ++j) {
            vrp_st << s[i][j]-1 << " ";
        }
    }

    vrp_st << 0 << std::endl;
    vrp_st << "OBJ= +inf" << std::endl;
    vrp_st << "BEST_KNOWN= +inf" << std::endl;
    vrp_st.close();

}

void RoutingL::VRPCallVRPH(std::string sp, std::string ss, std::string si) {

    vrphl(ss.empty() ? NULL : const_cast<char *> (sp.c_str()),
          ss.empty() ? NULL : const_cast<char *> (ss.c_str()),
          si.empty() ? NULL : const_cast<char *> (si.c_str()) );

}

void RoutingL::VRPReadSolution(std::vector<std::vector<int> > &s, std::string ss) {

    s.clear();
    std::ifstream vrp_sol ( ss.c_str() );

    int vrp_Ncustomer = 0;
    vrp_sol >> vrp_Ncustomer;
    for (int i = 0; i < vrp_Ncustomer ; ++i) {
        int dato = 0;
        vrp_sol >> dato;

        if (dato > 0)
            s.back().push_back( dato + 1 );
        else {
            s.push_back( std::vector<int>() );
            s.back().push_back( 1 );
            s.back().push_back( -dato + 1 );
        }
    }

    std::string tmp;

    vrp_sol >> tmp; // 0
    if (tmp != "0") std::cout << "ERROR RoutingL::VRPReadSolution 0 "  << ss << std::endl;

    vrp_sol >> tmp; // OBJ=
    vrp_sol >> tmp; // double
    vrp_sol >> tmp; // BEST_KNOWN=
    vrp_sol >> tmp; // string

    vrp_sol.close();

}

void RoutingL::VRPWriteGraph(std::vector<VRPNode> &d, std::vector<std::vector<int> > &s, std::string sg) {

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

    double GMLFactor = 10.0;
    int NDepot = 1;

    std::ofstream file_gml(sg.c_str());
    file_gml << "graph [ hierarchic 1 directed 1" << std::endl;
    for (int i = 0; i < NDepot; ++i) {
        file_gml << "node [ id " << d[i].id << " graphics [ x " << d[i].x * GMLFactor
                 << " y " << d[i].y * GMLFactor
                 << " w 11 h 11 type \"roundrectangle\" fill \"#FF6600\"] LabelGraphics [text \""
                 << d[i].id << "\" fontSize 7 ] ]" << std::endl;
    }

    for (size_t i = NDepot; i < d.size(); ++i) {
        file_gml << "node [ id " << d[i].id
                 << " graphics [ x " << d[i].x * GMLFactor
                 << " y " << d[i].y * GMLFactor
                 << " w 11 h 11 type \"roundrectangle\"] LabelGraphics [text \""
                 << d[i].id << "\" fontSize 7 ] ]" << std::endl;
    }

    for (size_t i = 0; i < s.size(); ++i) {

        std::vector<int>::iterator iter = s[i].begin();
        bool fin = 1;
        while (fin) {
            file_gml << "edge [ source " << *iter;
            if (++iter == s[i].end()){
                fin = 0;
                iter = s[i].begin();
            }
            file_gml << " target " << *iter
                     << " graphics [ fill \""
                     << MyRGBColors[i % MyRGBColors.size() ]
                     << "\" width 2 targetArrow \"standard\" ] ]"
                     << std::endl;
        }
    }

    file_gml << "]" << std::endl;
    file_gml.close();

}
