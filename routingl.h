#ifndef ROUTINGL_H
#define ROUTINGL_H

#include <iostream>
#include <vector>
#include <string>

void vrphl(char *input=NULL, char *output=NULL, char *initial=NULL);
extern "C" int liblkh(int argc, char *argv[]);

struct TSPNode {
    int id;
    double x;
    double y;
};

struct VRPNode {
    int id;
    double x;
    double y;
    int dem;
};

struct RoutingL {
    // TSP
    static void TSPRandomData(std::vector<TSPNode> &d, int n);
    static void TSPDistance(std::vector<TSPNode> &d, std::vector<std::vector<double > > &di);
    static void TSPWriteData(std::vector<TSPNode> &d, std::string sd, std::string sp, std::string ss);
    static void TSPCallLKH(std::string sp);
    static void TSPReadSolution(std::vector <int> &s, std::string ss);
    static void TSPWriteGraph(std::vector<TSPNode> &d, std::vector <int> &s, std::string sg);
    // VRP
    static void VRPRandomData(std::vector<VRPNode> &d, int n);
    static void VRPDistance(std::vector<VRPNode> &d, std::vector<std::vector<double > > &di);
    static void VRPWriteData(std::vector<VRPNode> &d, int c, std::string sd);
    static void VRPWriteSolution(std::vector <std::vector <int> > &s, std::string si, int n);
    static void VRPCallVRPH(std::string sp, std::string ss, std::string si);
    static void VRPReadSolution(std::vector <std::vector <int> > &s, std::string ss);
    static void VRPWriteGraph(std::vector<VRPNode> &d, std::vector <std::vector <int> > &s, std::string sg);

};

#endif // ROUTINGL_H
