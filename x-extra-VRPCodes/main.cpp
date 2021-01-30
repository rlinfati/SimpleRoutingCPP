#include <iostream>
#include <cstdlib>
#include <vector>
#include "cpptools.hpp"
#include "evrp.h"
#include "evrp_io.hpp"
#include "evrp_initialsolution.hpp"
#include "evrp_fixnk.hpp"
#include "evrp_localsearch.hpp"
#include "evrp_vns.hpp"
#include "evrp_tabusearch.hpp"
#include "evrp_annealing.hpp"

std::vector <eVRPNode> NODES;
std::vector <eVRPDepot> DEPOTS;
std::vector <eVRPCustomer> CUSTOMERS;
std::vector <eVRPService> SERVICES;
std::vector <eVRPVehicle> VEHICLES;
std::vector <std::vector<double> > DISTANCE;

int main(int argc, char *argv[]) {
    std::cerr << "SuperRouting build " <<  __DATE__ << " " << __TIME__ << std::endl;
    std::cout.setf(std::ios_base::fixed);
    
    printarg(argc, argv);

    srand(19833009);
    
    eVRPRandomData();
    eVRPDistance();
    eVRPWrite("evrp-test.txt");
    eVRPWriteGML("evrp-test.gml");

    eVRPCleanUp();
    eVRPRead("evrp-test.txt");
    eVRPDistance();
    eVRPWrite("evrp-test2.txt");
    
    Solution s1;
    SolutionData sd1;
    eVRPInitialSolution(s1, sd1, 2);
    
    eVRPFixnk(s1, sd1);
/*
    Solution s2;
    SolutionData sd2;
    eVRPLocalSearch(s1, sd1, 11, s2, sd2);

    Solution s3;
    SolutionData sd3;
    eVRPVNS(s1, sd1, 11, s3, sd3);

    Solution s4;
    SolutionData sd4;
    eVRPTabuSearch(s1, sd1, 11, s4, sd4);
     */
    
    Solution s5;
    SolutionData sd5;
    eVRPSimAnnealing(s1, sd1, 11, s5, sd5);
    
    // TODO
    // parametro cmd argc argv - config file?
    // add e-vrp e infactibles
    
    return EXIT_SUCCESS;
}
